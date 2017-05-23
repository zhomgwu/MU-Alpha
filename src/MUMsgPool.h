/*
MessagePool save all message from network, WORKER will process
the message. MessagePool may be exist single or equality WORKERs.
*/
#ifndef __MUMSGPOOL_H__
#define __MUMSGPOOL_H__

struct WorkerMessage {
	MUBuffer buffer;
	WorkerMessage *next;
};

class WorkerMessageList {
public:
	WorkerMessageList();
	~WorkerMessageList();

public:

	void init(int sign);

	int getOwner();

	bool isEmpty();

	void pushFront(WorkerMessage *msg);

	void pushBack(WorkerMessage *msg);

	WorkerMessage *popHead();

private:
	int _sign;
	WorkerMessage *_head;
	WorkerMessage *_tail;
};

struct PoolMessageList {
	WorkerMessageList *netMsgList;
	PoolMessageList *next;
};

class MessagePool {
public:
	MessagePool();
	~MessagePool();

public:

	bool isEmpty();
	
	void pushMessage(WorkerMessageList *list);

	WorkerMessageList *popMessage();

private:
	int _listLength;
	PoolMessageList *_head;
	PoolMessageList *_tail;
};

#endif //__MUMSGPOOL_H__
