#ifndef __MUEVUNIT_H__
#define __MUEVUNIT_H__

struct MUEvUnit {
	int fd;
	int mask;
	int type;
	NetMessageList *readBuffer;
	NetMessageList *writeBuffer;
	MUEvUnit *next;
};

class MUEvUnitHash {
public:
	MUEvUnitHash();
	~MUEvUnitHash();

public:

	bool init(int max);

	bool insert(int fd, int mask, int type);

	void remove(int fd);

	bool isfull();

	MUEvUnit* getUnit(int fd);

private:
	int _cap;
	int _count;
	int _capMask;
	MUEvUnit *_units;
	MUEvUnit **_unitHash;
};

#endif //__MUEVUNIT_H__
