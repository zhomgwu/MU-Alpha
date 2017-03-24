/*
MessagePool save all message from network, WORKER will process
the message. MessagePool may be exist single or equality WORKERs.
*/
#ifndef __MU_MSGPOOL_H__
#define __MU_MSGPOOL_H__

struct NetMessage {
	void *data;
	int length;
	NetMessage *next;
};

class NetMessageList {
public:
	NetMessageList();
	~NetMessageList();

public:

	void init(int fd);

	bool isEmpty();

	void pushFront(NetMessage *msg);

	void pushBack(NetMessage *msg);

	NetMessage *popHead();

private:
	int _fd;
	NetMessage *_head;
	NetMessage *_tail;
};

struct PoolMessageList {
	NetMessageList *netMsgList;
	PoolMessageList *next;
};

class MessagePool {
private:
	MessagePool();
	~MessagePool();

public:

	bool isEmpty();
	
	void pushMessage(NetMessageList *list);

	NetMessageList *popMessage();

private:
	PoolMessageList *_head;
	PoolMessageList *_tail;
};

#endif //__MU_MSGPOOL_H__
