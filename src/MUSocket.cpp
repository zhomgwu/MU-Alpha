#include "MUSocket.h"
#include "MUMacro.h"

union sockaddr_all {
	struct sockaddr sa;
	struct sockaddr_in sa4;
	struct sockaddr_in6 sa6;
};

void _errorOccur(MUSocketRet *ret) {
	ret->status = MU_SR_ERROR;
	ret->errno = errno;
	ret->errstr = strerror(errno);
}

int MUSocket::listen(const char *host, int port, MUSocketRet &sr) {
	int af = strchr(host, ':') == 0 ? AF_INET : AF_INET6;
	int sockfd;
	struct addrinfo hints, *info, *p;
	char portstr[16] = {};
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = af;
	hints.ai_socktype = SOCK_STREAM;

	sprintf(portstr, "%d", port);
	if (getaddrinfo(host, portstr, &hints, &info) != 0) {
		_errorOccur(&sr);
		return -1;
	}
	memset(&sr, 0, sizeof(sr));
	for (p = info; p!=NULL; p= p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			continue;
		}
		setNonblocking(sockfd);
		setKeepAlive(sockfd);
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			goto _error:
		}
		break;
	}

	if (listen(sockfd, MU_SOCKET_BACKLOG) == -1) {
		close(sockfd);
		goto _error:
	}

	sr.status = MU_SR_SUCCESS;
	freeaddrinfo(info);
	return sockfd;
_error:
	freeaddrinfo(info);
	_errorOccur(&sr);
	return -1;
}

int MUSocket::connect(const char *host, int port, MUSocketRet &sr) {
	int sockfd;
	struct addrinfo hints, *info, *p;
	char portstr[6] = {};
	memset(&sr, 0, sizeof(sr));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	sprintf(portstr, "%d", port);
	if (getaddrinfo(host, portstr, &hints, &info) != 0) {
		_errorOccur(&sr);
		return -1;
	}
	
	for (p = info; p!=NULL; p= p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			continue;
		}
		setNonblocking(sockfd, 1);
		setKeepAlive(sockfd);
		if (connect(p->ai_family, p->ai_socktype, p->ai_protocol) == -1) {
			// 1. error; 2. non-blocking EINPROGRESS
			if (errno != EWOULDBLOCK && errno != EINPROGRESS) {
				close(sockfd);
				goto _error;
			}
			sr->status = MU_SR_CONNECTING;
		}
		break;
	}
	freeaddrinfo(info);
	return sockfd;
_error:
	_errorOccur(&sr);
	freeaddrinfo(info);
	return -1;
}

int MUSocket::accept(int fd, char *hostbuf, int len, MUSocketRet &sr) {
	memset(&sr, 0, sizeof(sr));
	sockaddr_all usa;
	int usalen = sizeof(usa);
	int connfd = accept(fd, usa.sa, usalen);
	if (connfd < 0) {
		// 1. error; 2. non-blocking EWOULDBLOCK || EAGAIN
		_errorOccur(&sr);
		return -1;
	}

	int hostlen = usa.sa.sa_family == AF_INET? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;
	// 7 = port + ':' + '\0'
	if (len >= hostlen + 7) {
		int port = usa.sa.sa_family == AF_INET ? usa.sa4.sin_port : usa.sa6.sin6_port;
		void *addr = usa.sa.sa_family == AF_INET ? usa.sa4.sin_addr : usa.sa6.sin6_addr;
		char hosttmp[INET6_ADDRSTRLEN] = {};
		if (inet_ntop(usa.sa.sa_family, addr, hosttmp, sizeof(hosttmp))) {
			sprintf(hostbuf, "%s:%d", hosttmp, port);
		}
	}
	sr.status = MU_SR_SUCCESS;
	setNonblocking(connfd);
	setKeepAlive(connfd);
	return connfd;
}

int MUSocket::read(int fd, void *data, int &len, MUSocketRet &sr) {
	int rcount;
	len = 0;
	memset(&sr, 0, sizeof(sr));
	for (;;) {
		rcount = read(fd, data+len, MU_SOCKET_MAXSIZE);
		if (rcount == 0) {
			close(fd);
			sr.status = MU_SR_READEOF;
			break;
		}
		else if (rcount < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// not buffer any more
				sr.status = MU_SR_SUCCESS;
				break;
			}
			else if (errno == EINTR) {
				// interupted by signal, continue to read
				continue;
			}
			else
			{
				// occur read error, may be some trouble in network
				close(fd);
				_errorOccur(&sr);
				break;
			}
		}
		len += rcount;
	}
	return len;
}

int MUSocket::write(int fd, void *data, int len, MUSocketRet &sr){
	int wcount = 0, wret = 0;
	memset(&sr, 0, sizeof(sr));
	for (;;) {
		wret = write(fd, data + wcount, len - wcount);
		if (wret < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// buffer cache full
				sr.status = MU_SR_WRITEFULL;
				break;
			}
			else if (errno == EINTR) {
				continue;
			}
			close(fd);
			_errorOccur(&sr);
			return -1;
		}
		wcount += wret;
		if (wcount == len) {
			sr.status = MU_SR_SUCCESS;
			break;
		}
	}
	return wcount;
}

int MUSocket::setSocketOpt(int fd, int op, int value) {

}

int MUSocket::setNonblocking(int fd, int nonblock) {
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1) {
		return -1;
	}
	// change non-block to block
	if (nonblock == 0 && flags & O_NONBLOCK) {
		flags |= ~O_NONBLOCK;
		if (fcntl(fd, F_SETFL, flags) == -1) {
			return -1;
		}
	}
	// change block to non-block
	if (nonblock != 0 && (flags & O_NONBLOCK) == 0) {
		flags |= O_NONBLOCK;
		if (fcntl(fd, F_SETFL, flags) == -1) {
			return -1;
		}	
	}
	return 0;
}

int MUSocket::setKeepAlive(int fd) {

}

