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
	
	static MessagePool *getInstance();

	static void destroy();

	bool isEmpty();
	
	void pushMessage(NetMessageList *list);

	NetMessageList *popMessage();

private:
	static MessagePool *_instance;
	PoolMessageList *_head;
	PoolMessageList *_tail;
};

#endif //__MU_MSGPOOL_H__
