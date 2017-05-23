#ifndef __MUMSGDISPATCHER_H__
#define __MUMSGDISPATCHER_H__


class MUMessageDispatcher {
public:
	MUMessageDispatcher() {}
	virtual ~MUMessageDispatcher() {}

public:

	virtual MUBuffer *getPackage(WorkerMessageList *msgList);

	virtual void dispatchMessage(int owner, MUBuffer *buffer);

	//virtual void registerService(int key, MUService *service);

protected:

	MUService *_services;
};

#endif //__MUMSGDISPATCHER_H__
