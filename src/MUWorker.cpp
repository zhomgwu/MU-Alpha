#include "MUWorker.h"

MUWorker::MUWorker() {

}

MUWorker::~MUWorker() {

}

bool MUWorker::init() {
	return true;
}

void MUWorker::runWorker() {
	WorkerMessageList *msgList = _msgPool->popMessage();
	if (!msgList) {
		int owner = msgList->getOwner();
		WorkerMessage * message = msgList->popHead();
		while (message) {
			_dispatcher->dispatchMessage(owner, message->buffer);
			message = msgList->popHead();
		}
		msgList = _msgPool->popMessage();
	}
}


MUWorkerManager::MUWorkerManager() {

}

MUWorkerManager::~MUWorkerManager() {

}
	
MUWorkerManager* MUWorkerManager::getInstance() {
	if (_instance == NULL) {
		_instance = new MUWorkerManager;
	}
	return _instance;
}
	
void MUWorkerManager::destroy() {
	if (_instance != NULL) {
		delete _instance;
		_instance = NULL;
	}
}

void MUWorkerManager::newWorker() {
	MUWorker *worker = new MUWorker;
	worker->init();
	_workers.push_back(worker);
}

MUWorker *MUWorkerManager::getWorker() {
	if (_workers.size() == 0) {
		return NULL;
	}
	// always return first one (after may not)
	for (std::vector<MUWorker* >::iterator iter = _workers.begin();
		iter != _workers.end(); ++iter) {
		if (*iter != NULL) {
			return *iter;
		}
	}
}
