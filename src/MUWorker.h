#ifndef __MUWORKER_H__
#define __MUWORKER_H__

#include <vector>

class MUWorker {
public:
	MUWorker();
	~MUWorker();

public:

	bool init();

	void runWorker();

	void pushMessage(WorkerMessageList *msgList);

private:
	MUMsgDispatcher *_dispatcher;
	MessagePool *_msgPool;
};

class MUWorkerManager {
public:
	MUWorkerManager()
	~MUWorkerManager();
public:
	
	static MUWorkerManager* getInstance();
	
	static void destroy();

	void newWorker();

	MUWorker *getWorker();

private: 

	static MUWorkerManager *_instance;
	std::vector<MUWorker* > _workers;
};

#endif //__MUWORKER_H__

