#ifndef __MU_WORKER_H__
#define __MU_WORKER_H__

class MUWorker {
public:
	MUWorker();
	~MUWorker();

public:

	bool init();

	void runWorker();

private:

	MessagePool *_msgPool;
	MessageDispatcher *_msgDispatcher;
};

#endif //__MU_WORKER_H__

