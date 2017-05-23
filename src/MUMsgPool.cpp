#include "MUMsgPool.h"

WorkerMessageList::WorkerMessageList() {
}

WorkerMessageList::~WorkerMessageList() {
}

void WorkerMessageList::init(int sign) {
	_sign = sign;
	_head = NULL;
	_tail = NULL;
}

int WorkerMessageList::getOwner() {
	return _sign;
}

bool WorkerMessageList::isEmpty() {
	return _head == NULL;
}

void WorkerMessageList::pushFront(WorkerMessage *msg) {
	if (msg == NULL) {
		return;
	}
	if (isEmpty()) {
		_head = _tail = msg;
	}
	else {
		msg->next = _head;
		_head = msg;
	}
}

void WorkerMessageList::pushBack(WorkerMessage *msg) {
	if (msg == NULL) {
		return;
	}
	if (isEmpty()) {
		_head = _tail = msg;
	}
	else {
		_tail->next = msg;
		_tail = msg;
	}
}

WorkerMessage *WorkerMessageList::popHead() {
	if (isEmpty()) {
		return NULL;
	}
	WorkerMessage *tmp = _head;
	_head = _head->next;
	return tmp;
}

MessagePool::MessagePool()
:_head(NULL)
,_tail(NULL) {

}

MessagePool::~MessagePool() {
}

bool MessagePool::isEmpty() {
	return _head == NULL;
}

void MessagePool::pushMessage(WorkerMessageList *list) {
	if (list == NULL) {
		return;
	}
	if (isEmpty()) {
		_head = _tail = list;
	}
	else {
		_tail->next = list;
		_tail = list;
	}
	_listLength++;
}

WorkerMessageList *MessagePool::popMessage() {
	if (isEmpty()) {
		return NULL;
	}
	WorkerMessageList *tmp = _head;
	_head = _head->next;
	_listLength--;
	return tmp;
}
