#ifndef __MU_MSGDISPATCHER_H__
#define __MU_MSGDISPATCHER_H__


class MUMessageDispatcher {
public:
	MUMessageDispatcher() {}
	virtual ~MUMessageDispatcher() {}

public:

	virtual void registerService(int key, MUService *service);

	virtual void dispatchMessage();

protected:

	MUService *_services;
};

#endif //__MU_MSGDISPATCHER_H__
