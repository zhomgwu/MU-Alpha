#ifndef __MUSERVER_H__
#define __MUSERVER_H__

class MUServer {
public:
	MUServer();
	~MUServer();

public:
	// create server and initiate it
	bool createServer();
	// load kinds of configuration 
	bool preloadService();
	// load kinds of service, include lua, c, c++ and so on
	bool loadService();

};

#endif //__MUSERVER_H__
