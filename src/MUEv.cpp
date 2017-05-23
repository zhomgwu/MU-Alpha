#include "MUEv.h"
#include "MUEvUnit.h"

MUEv::MUEv()
: _eventSize(0)
, _events(NULL)
, _fireEvents(NULL)
, _pollState(NULL) {
}

MUEv::~MUEv() {
}

bool MUEv::init(int size) {
	_eventSize = size;
	_events = new MUEvUnitHash;
	if (_events->initUnitHash(size)) {
		delete _events;
		return false;
	}

	_fireEvents = new MUFireEvent[size];
	for (int i = 0; i < size; ++i) {
		_fireEvents[i].fd = MU_FD_INVALID;
		_fireEvents[i].mask = 0;
	}

#ifdef __LINUX__
	// epoll
	_pollState = new MUEpoll();
#elif __SOLARI__ || __MAC__
	// kqueue
#else
	// select
#endif
	
	if (!_pollState->init(size)) {
		delete _events;
		delete []_fireEvents;
		return false;
	}
	return true;
}

void MUEv::release() {
	if (_events) {
		delete _events;
		_events = NULL;
	}
	if (_fireEvents) {
		delete []_fireEvents;
		_fireEvents = NULL;
	}
	if (_pollState) {
		delete _pollStates;
		_pollState = NULL;
	}
}

bool MUEv::add(int fd, int mask, int type) {
	MUEvUnit * eu = _events->getUnit(fd);
	if (eu == NULL) {
		_events->insert(fd, mask, type);
		_pollState->addPoll(fd, EV_OP_NONE, mask);
	}
	else
	{
		_pollState->addPoll(fd, eu->mask, mask);
		eu->mask |= mask;
	}
	return true;
}

bool MUEv::del(int fd, int mask) {
	MUEvUnit * eu = _events->getUnit(fd);
	if (eu == NULL) {
		return false;
	}
	else {
		_pollState->delPoll(fd, eu->mask);
		eu->mask &= ~mask;
		if (eu->mask == EV_OP_NONE) {
			_events->remove(fd);
		}
	}
}

void MUEv::process() {
	// process del events
	MUDelEvent *dev = _delEvents;
	while (dev) {
		MUDelEvent *tmp = dev; 
		del(dev->fe.fd, dev->fe.mask);
		dev = dev->next;
		delete tmp;
	}

	int nfds = _pollState->waitPoll(_fireEvents, 0);
	for (int i = 0; i < nfds; ++i) {
		MUFireEvent &fe = _fireEvents[i];
		MUEvUnit *eu = _events->getUnit(fe.fd);
		assert(eu != NULL);

		MUSockRet ret;
		if (eu->type == EV_SOCKET_LISTEN) {
			// need accept
			char hostbuf[INET6_ADDRSTRLEN] = {};	
			int clientfd = MUSocket::accept(eu->fd, hostbuf, INET6_ADDRSTRLEN, ret);
			if (ret.status == MU_SR_SUCCESS) {
				// new client
				add->add(clientfd, EV_OP_READABLE | EV_OP_WRITABLE, EV_SOCKET_ACCEPT));
				MUClientManager::getInstance()->newClient(clientfd);
			}
			else {
				// error, print accept error

			}
		}
		else if (eu->type == EV_SOCKET_ACCEPT) {
			MUClient *client = MUClientManager::getInstance()->getClientWithFd(eu->fd);
			// client must be not NULL
			if (NULL == client) {
				continue;
			}
			// need read or write to client
			if (eu->mask & EV_OP_READABLE != 0) {
				int rlen = 0;
				// read buffer size greater than 1460
				MUBuffer buffer;
				rlen = MUSocket::read(eu->fd, buffer, ret);
				if (ret.status == MU_SR_SUCCESS) {
					// insert to bufferlist
					client->receive(buffer);
				}
				else {
					// read error, remove from _events
					pushDelEvent(eu->fd);
				}
			}
			else if (eu->mask & EV_OP_WRITABLE != 0) {
				// (note: maybe need lock WriteBuffer)
				WorkerMessageList *wbufferList = client->getWriteBuffer();
				// write
				while (!wbufferList->isEmpty()) {
					WorkerMessage *sendMsg = wbufferList->popHead();
					int wlen = 0;
					wlen = MUSocket::write(eu->fd, sendMsg->buffer, ret);
					if (ret.status == MU_SR_SUCCESS) {
						// send success
					}
					else if (ret.status == MU_SR_WRITEFULL) {
						// shift write buffer
						sendMsg->buffer->shift(wlen);
						break;
					}
					else {
						// write error, remove fd from _events
						pushDelEvent(eu->fd);
						break;
					}
				}
			}
		}
		else if (eu->type == EV_SOCKET_CONNECT) {
			// have response from other server
			
		}
		else {
			// exception happen
		}
	}
}

void MUEv::pushDelEvent(int fd) {
	MUDelEvent *dev = new MUDelEvent;
	dev->fe.fd = fd;
	dev->fe.mask = EV_OP_READABLE | EV_OP_WRITABLE;
	dev->next = NULL;
	if (_delEvents == NULL) {
		_delEvents = dev;
	}
	else {
		_delEvents->next = dev;
	}
}
