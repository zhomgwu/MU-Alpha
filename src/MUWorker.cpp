#include "MUWorker.h"

MUWorker::MUWorker() {

}

MUWorker::~MUWorker() {

}

bool MUWorker::init() {
	_isPause = false;
	return true;
}

bool MUWorker::isPause() {
	return _isPause;
}

void MUWorker::dispatchMessage() {
	NetMessageList *headList = _msgPool->popMessage();
	if (!headList) {
		NetMessage * headList
	}
		
}

void MUWorker::runWorker() {
	if (!_isPause) {
		dispatchMessage();
	}
}

