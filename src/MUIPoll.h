#ifndef __MUPOLLSTATE_H__
#define __MUPOLLSTATE_H__

struct MUFireEvent;
class MUIPoll {
public:
	MUIPoll() {}
	virtual ~MUIPoll() {}

public:

	bool createPoll(int size) = 0;

	void releasePoll() = 0;

	bool addPoll(int fd, int omask, int mask) = 0;

	bool delPoll(int fd, int omask, int mask) = 0;

	int waitPoll(MUFireEvent *fevs, int millisec) = 0;
};

#endif //__MUPOLLSTATE_H__
