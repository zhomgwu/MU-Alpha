#include "MUClient.h"

MUClient::MUClient() {
}

MUClient::~MUClient() {
}

bool MUClient::init(int fd, int cid) {
	_fd = fd;
	_cid = cid;
}

void MUClient::setFd(int fd) {
	_fd = fd;
}

void MUClient::setCid(int cid) {
	_cid = cid;
}

void MUClient::send(MUBuffer &buffer) {
	int wlen = 0;
	// socket buffer cache was full, wait to send
	if (_writeBuffer.length() > 0) {
		_writeBuffer.write(buffer.buffer(), buffer.length());
	}
	else {
		MUSocketRet sr;
		wlen = MUSocket::write(_fd, buffer, sr);
		if (sr.status != MU_SR_SUCCESS) {
			if (sr.status == MU_SR_WRITEFULL) {
				// shift wrote length
				buffer.shift(wlen);
				// save in _writeBuffer
				_writeBuffer.write(buffer.buffer(), buffer.length())
			}
			else {
				// error and delete event

			}
		}	
	}
}

WorkerMessage *_newWorkerMessage(MUBuffer &buffer) {
	WorkerMessage *workerMsg = new WorkerMessage;
	workerMsg->buffer = buffer;
	workerMsg->next = NULL;
	return workerMsg;
}

void _freeWorkerMessage(WorkerMessage *workerMsg) {
	delete workerMsg;
}

void _processMessage(MUBuffer &buffer) {
	// get one package
	if (buffer.length() < 2) {
		return;
	}
	unsigned short len = 0;
	buffer.read(len);
	if (buffer.length() >= len) {
		// dispatch to a worker
		MUWorker *worker = MUWorkerManager::getInstance()->getWorker();
		if (worker != NULL) {
			MUBuffer pb;
			pb.write(buffer.buffer(), len);
			buffer.shift(len);
			
			WorkerMessage *procMsg = _newWorkerMessage(pb);
			worker->pushMessage(procMsg);
		}	
	}
}

void MUClient::receive(MUBuffer &buffer) {
	_readBuffer.write(buffer.buffer(), buffer.length());
	_processMessage(_readBuffer);
}

WorkerMessageList *MUClient::getReadBuffer() {
	return _readBuffer;
}

WorkerMessageList *MUClient::getWriteBuffer() {
	return _writeBuffer;
}
