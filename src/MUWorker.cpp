#include "MUWorker.h"

MUWorker::MUWorker() {

}

MUWorker::~MUWorker() {

}

bool MUWorker::init() {
	_isPause = false;
	return true;
}

void MUWorker::runWorker() {
	NetMessageList *headList = _msgPool->popMessage();
	if (!headList) {
		NetMessage * message = headList->popHead();
		

	}
}

