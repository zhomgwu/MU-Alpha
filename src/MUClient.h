#ifndef __MUCLIENT_H__
#define __MUCLIENT_H__

class MUClient {
public:
	MUClient();
	~MUClient();

public:

	bool init(int fd, int cid);

	void setFd(int fd);

	void setCid(int cid);

	void send(MUBuffer &buffer);

	void receive(MUBuffer &buffer);

	MUBuffer *getReadBuffer();
	MUBuffer *getWriteBuffer();

private:
	int _fd;
	int _cid;
	MUBuffer _readBuffer;
	MUBuffer _writeBuffer;
};

class MUClientManager {
private:
	MUClientManager();
	~MUClientManager();
public:
	
	static MUClientManager *getInstance();
	static void destroy();

	MUClient *getClientWithCid(int cid);
	
	MUClient *getClientWithFd(int fd);

	MUClient *newClient(int fd);

	void delClient(int fd);

private:

	static MUClientManager *_instance;
	int _clientId;
};

#endif //__MUCLIENT_H__
