#ifndef __MUEPOLL_H__
#define __MUEPOLL_H__

#include "MUIPoll.h"

class MUEpoll : public MUIPoll {
public:
	MUEpoll();
	~MUEpoll();

public:

	bool createPoll(int size);

	void releasePoll();

	bool addPoll(int fd, int omask, int mask);

	bool delPoll(int fd, int omask, int mask);

	int waitPoll(MUFireEvent *fevs, int millsec);

private:
	int _epfd;
	int _maxEpollEvents;
	epoll_event *_epEvents;
};

#endif //__MUEPOLL_H__
