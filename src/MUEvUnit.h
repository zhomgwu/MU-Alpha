#ifndef __MUEVUNIT_H__
#define __MUEVUNIT_H__

struct MUEvUnit {
	int fd;
	int mask;
	int type;
	MUEvUnit *next;
};

class MUEvUnitHash {
public:
	MUEvUnitHash();
	~MUEvUnitHash();

public:

	bool initUnitHash(int max);

	bool insertUnit(int fd, int mask, int type);

	void removeUnit(int fd);

	bool isfullUnitHash();

	MUEvUnit* getUnit(int fd);

private:
	int _cap;
	int _count;
	int _capMask;
	MUEvUnit *_units;
	MUEvUnit **_unitHash;
};

#endif //__MUEVUNIT_H__
