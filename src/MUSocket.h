#ifndef __MUSOCKET_H__
#define __MUSOCKET_H__

#define MU_SR_ERROR -1
#define MU_SR_SUCCESS 0
#define MU_SR_CONNECTING 1
#define MU_SR_READEOF 2
#define MU_SR_WRITEFULL 3

// this structure is used for socket operation. if socket operation 
// doesn't occur error, status set value MU_SR_SUCCESS. otherwise,
// status set value MU_SR_ERROR, and errno will be appropriate,
// and errstr assign for human-readable string. MU_SR_CONNECTING 
// will be set to status when connect to server. MU_SR_READEOF 
// will be set when read socket.
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
	// set kernel socket option
	static int setSocketOpt(int fd, int op, int value);
	// set socket fd blocking or non-block, nonblock = 0 means block, otherwise nonblock
	static int setNonblocking(int fd, int nonblock);
	// set socket fd keep alive time
	static int setKeepAlive(int fd);
};

#endif //__MUSOCKET_H__
