#include "MUEvUnit.h"

MUEvUnitHash::MUEvUnitHash()
: _cap(0)
, _count(0)
, _capMask(0)
, _units(NULL)
, _unitHash(NULL) {
}

MUEvUnitHash::~MUEvUnitHash() {
}

void _clearEvUnit(MUEvUnit *eu) {
	eu->fd = -1;
	eu->mask = 0;
	eu->type = 0;
	eu->next = NULL;
}

bool MUEvUnitHash::initUnitHash(int max) {
	_cap = 1024;
	while (_cap < max) {
		_cap = _cap << 1;
	}

	_count = 0;
	_capMask = _cap - 1;
	// allocate memory storage for elements
	_units = new MUEvUnit[_cap];
	for (int i = 0; i < _cap; ++i) {
		_clearEvUnit(&_units[i]);
	}

	_unitHash = new MUEvUnit*[_cap];
	memset(_unitHash, 0, sizeof(MUEvUnit*) *_cap);
	return true;
}

bool MUEvUnitHash::insertUnit(int fd, int mask, int type) {
	if (isfullUnitHash()) {
		return false;
	}
	// get element in storage
	MUEvUnit *eu = NULL;
	for (int i = 0; i < _cap; ++i) {
		int loc = (fd + i) % _cap;
		if (_units[loc].fd == -1) {
			eu = &_units[loc];
			break;
		}
	}
	// insert to list head
	int index = fd & _capMask;
	euh = _unitHash[index];
	if (euh) {
		eu->next = euh;
	}
	_unitHash[index] = eu;
	++count;
	return true;
}

void MUEvUnitHash::removeUnit(int fd) {
	MUEvUnit *euh = NULL;
	int index = fd & _capMask;
	euh = &_unitHash[index];
	if (euh == NULL) {
		return;
	}
	if (euh->fd == fd) {
		_unitHash = euh->next;
		_clearEvUnit(euh);
		--count;
		return;
	}

	while (euh->next) {
		if (euh->next->fd == fd) {
			MUEvUnit *eu = euh->next;
			euh->next = euh->next->next;
			_clearEvUnit(eu);
			--count;
			break;
		}
		euh = euh->next;
	}
}

bool MUEvUnitHash::isfullUnitHash() {
	return _cap == _count;
}

MUEvUnit* MUEvUnitHash::getUnit(int fd) {
	int index = fd & _capMask;
	euh = &_unitHash[index];
	if (euh == NULL) {
		return NULL;
	}
	if (euh->fd == fd) {
		return euh;
	}
	while(euh->next) {
		if (euh->next->fd == fd) {
			return euh->next;
		}
	}
	return NULL;
}
