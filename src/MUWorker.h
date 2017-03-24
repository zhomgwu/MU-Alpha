#ifndef __MU_WORKER_H__
#define __MU_WORKER_H__

class MUWorker {
public:
	MUWorker();
	~MUWorker();

public:

	bool init();

	bool isPause();

	void dispatchMessage();

	void runWorker();

private:

	bool _isPause;
	MessagePool *_msgPool;
	MessageDispatcher *_msgDispatcher;
};

#endif //__MU_WORKER_H__

