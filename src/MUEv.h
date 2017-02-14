#ifndef __MUEV_H__
#define __MUEV_H__

#define EV_MAX_SIZE 65535

#define EV_OP_NONE 0
#define EV_OP_READABLE 1
#define EV_OP_WRITABLE 2

#define EV_SOCKET_LISTEN 1		// listening fd
#define EV_SOCKET_ACCEPT 2		// accept from client connect
#define EV_SOCKET_CONNECT 3		// connect to other server

struct MUFireEvent {
	int fd;
	int mask;
};

class MUEvUnitHash;
class MUFireEventList;
class MUIPoll;
class MUEv {
public:
	MUEv();
	~MUev();

public:

	bool createEv(int size = EV_MAX_SIZE);

	void releaseEv();

	bool addEv(int fd, int mask, int type);

	bool delEv(int fd, int mask);

	bool processEv();

private:

	int _eventSize;
	MUEvUnitHash *_events;
	MUFireEvent *_fireEvents;
	MUIPoll *_pollState;
};

#endif //__MUEV_H__
