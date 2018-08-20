#include "chr.h"

int Socket(int family, int type, int protocol)
{
	int	    fd;

	if ((fd = socket(family, type, protocol)) < 0)
		err_sys("socket error");
	return fd;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
		err_sys("bind error");
}

void Listen(int fd, int backlog)
{
	if (listen(fd, backlog) < 0)
	    err_sys("listen error");
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)
		err_sys("connect error");
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int		n;

	if ((n = accept(fd, sa, salenptr)) < 0)
			err_sys("accept error");
	return n;
}

int Kqueue(void)
{
	int     ret;

	if ((ret = kqueue()) < 0)
		err_sys("kqueue error");
	return ret;
}

int Kevent(int kq, const struct kevent *changelist, int nchanges,
        struct kevent *eventlist, int nevents, const struct timespec *timeout)
{
	int     ret;

	if ((ret = kevent(kq, changelist, nchanges, eventlist, nevents, timeout)) < 0)
		err_sys("kevent error");
	return ret;
}

int Select(int maxfdpl, fd_set *rset, fd_set *wset, fd_set *eset, 
        struct timeval *timeout)
{
    int     fd;

    if ((fd = select(maxfdpl, rset, wset, eset, timeout)) < 0)
        err_sys("select error");
    return fd;
}

char *Fgets(char *ptr, int n, FILE *stream)
{
	char    *rptr;

	if ((rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
		err_sys("fgets error");
	return rptr;
}

ssize_t readn(int fd, void *vptr, size_t nsize);
ssize_t writen(int fd, const void *vptr, size_t nsize);

ssize_t Readn(int fd, void *vptr, size_t nsize)
{
    ssize_t     n;

    if ((n = readn(fd, vptr, nsize)) < 0)
        err_sys("read error");
    return n;
}

ssize_t Writen(int fd, const void *vptr, size_t nsize)
{
    ssize_t     n;

    if ((n = writen(fd, vptr, nsize)) < 0)
        err_sys("write error");
    return n;
}
