#include "MUEpoll.h"

MUEpoll::MUEpoll()
: _epfd(0)
, _maxEpollEvents(0) 
, _epEvents(NULL) {
}

MUEpoll::~MUEpoll() {
}

void _clearEpollEvent(epoll_event *ev) {
	ev->data.fd = MU_FD_INVALID;
	ev->events = 0;
}

bool MUEpoll::createPoll(int size) {
	_maxEpollEvents = size;
	
	_epfd = epoll_create(size);
	if (_epfd == -1) {
		return false;
	}
	
	_epEvents = new epoll_event[size];
	for (int i = 0; i < size; ++i) {
		_clearEpollEvent(&_epEvents[i]);
	}
	return true;
}

void MUEpoll::releasePoll() {
	if (_epfd != 0) {
		close(_epfd);
		delete _epEvents;	
	}
}

bool MUEpoll::addPoll(int fd, int omask, int mask) {
	int op = 0;
	epoll_event ev = {0};
	ev.data.fd = fd;
	
	op = omask == EV_OP_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

	if (mask & EV_OP_READABLE) 
		ev.events |= EPOLL_IN;
	if (mask & EV_OP_WRITABLE)
		ev.events |= EPOLL_OUT;
	
	if (epoll_ctl(_epfd, op, fd, &ev) == -1) {
		return false;	
	}
	return true;
}

bool MUEpoll::delPoll(int fd, int omask, int mask) {
	int op = 0;
	epoll_event ev = {0};
	ev.data.fd = fd;

	if ((omask & ~mask) == AE_NONE) {
		op = EPOLL_CTL_DEL;
	}
	else {
		op = EPOLL_CTL_MOD;
	}
	
	if (mask & EV_OP_READABLE) 
		ev.events &= ~EPOLL_IN;
	if (mask & EV_OP_WRITABLE)
		ev.events &= ~EPOLL_OUT;

	if (epoll_ctl(_epfd, op, fd, &ev) == -1) {
		return false;	
	}
	return true;
}

int MUEpoll::waitPoll(MUFireEvent *fevs, int millsec) {
	if (millsec < -1) {
		// -1 means block wait until one event happen at least.
		millsec = -1;
	}
	int nfds = epoll_wait(_epfd, _epEvents, _maxEpollEvents, millsec);
	for (int i = 0; i < nfds; ++i) {
		epoll_event &ev = _epEvents[i];
		fevs[i].fd = ev.data.fd;
		if (ev.events & EPOLL_IN) 
			fevs[i].mask |= EV_OP_READABLE;
		if (ev.events & EPOLL_OUT)
			fevs[i].mask |= EV_OP_WRITABLE;
		// may be don't necessary
		_clearEpollEvent(&ev);
	}
	return nfds;
}
