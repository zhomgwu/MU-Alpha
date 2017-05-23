#ifndef __MUEV_H__
#define __MUEV_H__

#define EV_MAX_SIZE 65535

#define EV_OP_NONE 0
#define EV_OP_READABLE 1
#define EV_OP_WRITABLE 2

enum _SOCKETTYPE {
	EV_SOCKET_LISTEN = 0,	// listening fd	
	EV_SOCKET_ACCEPT,		// accept from client connect
	EV_SOCKET_CONNECT,		// connect to other server
};	

struct MUFireEvent {
	int fd;
	int mask;
};

struct MUDelEvent {
	MUFireEvent fe;
	MUDelEvent *next;
};

class MUEvUnitHash;
class MUFireEventList;
class MUIPoll;
class MUEv {
public:
	MUEv();
	~MUev();

public:

	bool init(int size = EV_MAX_SIZE);

	void release();

	bool add(int fd, int mask, int type);

	bool del(int fd, int mask);

	void process();

	void pushDelEvent(int fd);

private:

	int _eventSize;
	MUEvUnitHash *_events;
	MUFireEvent *_fireEvents;
	MUDelEvent *_delEvents;
	MUIPoll *_pollState;
};

#endif //__MUEV_H__
