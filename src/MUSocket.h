#ifndef __MUSOCKET_H__
#define __MUSOCKET_H__

#define MU_SR_ERROR -1
#define MU_SR_SUCCESS 0
#define MU_SR_CONNECTING 1
#define MU_SR_READEOF 2
#define MU_SR_WRITEFULL 3

// This structure is used for socket operation. If socket operation 
// doesn't occur error will return status MU_SR_SUCCESS. Otherwise 
// status set value MU_SR_ERROR, and errno will be appropriate,
// and errstr assign as human-readable string. MU_SR_CONNECTING 
// will be set to status when connect to server use nonblocking. 
// MU_SR_READEOF will be set when read socket return 0.
struct MUSockRet {
	int status;
	int errno;
	char *errstr;
};

class MUSocket {
public:
	// return listen fd
	static int listen(const char *host, int port, MUSocketRet &sr);
	// return connect fd
	static int connect(const char *host, int port, MUSocketRet &sr);
	// return a client socket fd, hostbuf will stuff the client ip, len indicate hostbuf size
	static int accept(int fd, char *hostbuf, int len, MUSocketRet &sr);
	// return read data length
	static int read(int fd, void *data, int &len, MUSocketRet &sr);
	// return write data length
	static int write(int fd, void *data, int len, MUSocketRet &sr);
	// set socket fd blocking or non-block, nonblock = 0 means block, otherwise nonblock
	static int setNonblocking(int fd, int nonblock);
	// set socket fd keep alive time
	static int setKeepAlive(int fd, int millisec);
};

#endif //__MUSOCKET_H__
