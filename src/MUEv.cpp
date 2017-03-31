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

bool MUEv::createEv(int size) {
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
	
	if (!_pollState->createEv(size)) {
		delete _events;
		delete []_fireEvents;
		return false;
	}
	return true;
}

void MUEv::releaseEv() {
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

bool MUEv::addEv(int fd, int mask, int type) {
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

bool MUEv::delEv(int fd, int mask) {
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

bool MUEv::processEv() {
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
				addEv->addEv(clientfd, EV_OP_READABLE | EV_OP_WRITABLE, EV_SOCKET_ACCEPT));
			}
			else {
				// error

			}
		}
		else if (eu->type == EV_SOCKET_ACCEPT) {
			// need read or write to client
			if (eu->mask & EV_OP_READABLE != 0) {
				int rlen = 0;
				// read buffer cache greater than 1460
				MUBuffer buffer;
				rlen = MUSocket::read(eu->fd, buffer, ret);
				if (ret.status == MU_SR_SUCCESS) {
					// insert to bufferlist

				}
				else {
					// error remove from _events

				}
			}
			else if (eu->mask & EV_OP_WRITABLE != 0) {
				// write
				while (!eu->writeBuffer->isEmpty()) {
					int wlen = 0;
					NetMessage *sendData = eu->writeBuffer->popHead();
					wlen = MUSocket::write(eu->fd, sendData->buffer, ret);
					if (ret.status == MU_SR_SUCCESS) {
						// send success
					}
					else if (ret.status == MU_SR_WRITEFULL) {
						// shift write buffer

						break;
					}
					else {
						// remove from _events

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
	return true;
}
