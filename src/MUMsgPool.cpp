#include "MUMsgPool.h"

NetMessageList::NetMessageList() {
}

NetMessageList::~NetMessageList() {
}

void NetMessageList::init(int fd) {
	_fd = fd;
	_head = NULL;
	_tail = NULL;
}

bool NetMessageList::isEmpty() {
	return _head == NULL;
}

void NetMessageList::pushFront(NetMessage *msg) {
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

void NetMessageList::pushBack(NetMessage *msg) {
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

NetMessage *NetMessageList::popHead() {
	if (isEmpty()) {
		return NULL;
	}
	NetMessage *tmp = _head;
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

void MessagePool::pushMessage(NetMessageList *list) {
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

NetMessageList *MessagePool::popMessage() {
	if (isEmpty()) {
		return NULL;
	}
	NetMessageList *tmp = _head;
	_head = _head->next;
	_listLength--;
	return tmp;
}
