#ifndef __MUSERVER_H__
#define __MUSERVER_H__

class MUServer {
public:
	MUServer();
	~MUServer();

public:

	bool initServer();

	bool startServer();

	bool exitServer();
};

#endif //__MUSERVER_H__
