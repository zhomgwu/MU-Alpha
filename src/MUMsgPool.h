#ifndef __MU_MSGPOOL_H__
#define __MU_MSGPOOL_H__

struct MUMessage {
	void *data;
	int length;
	MUMessage *next;
};

struct MUMsgList {
	int fd;
	MUMessage *head;
	MUMessage *tail;
};

struct MUGlobalMsgList {
	MUMsgList *current;
	MUGlobalMsgList *next;
};

class MUMsgPool {
private:
	MUMsgPool();
	~MUMsgPool();

public:
	
	static MUMsgPool *getInstance();

	static void destroy();

	void pushMsg(MUMsgList *list);

	MUMsgList *popMsg();

private:
	static MUMsgPool *_instance;
	static MUGlobalMsgList *_head;
	static MUGlobalMsgList *_tail;
};

#endif //__MU_MSGPOOL_H__
