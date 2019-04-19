// Network io base interface

#ifndef BASE_NETWORK_NIO_H_
#define BASE_NETWORK_NIO_H_

#include <string>
#include "base/base_types.h"

#include <sys/types.h>
#include <assert.h>
#include <errno.h>

#if defined(OS_WIN)
#include <winsock2.h>
typedef int socklen_t;
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
typedef int SOCKET;
#define INVALID_SOCKET    -1
#define SOCKET_ERROR      -1
#endif  // OS_WIN

#if defined(WITH_LIBEVENT)
#if defined(OS_WIN)
#include "event.h"
#else
#include <event.h>
#endif  // OS_WIN
#ifndef _WINSOCKAPI_
#define SO_MAX_MSG_SIZE    0x2003
inline int closesocket(int socket) { return close(socket); }
#endif  // _WINSOCKAPI_
#else
#endif  // WITH_LIBEVENT

namespace nbase
{

typedef uint32_t event_t;
enum
{
	EREAD  = 0x02,
	EWRITE = 0x04
};

/*
 *  Purpose     Proxy setting
 */
struct ProxySet
{
    uint32_t    type_;     // PROXYTYPE_NOPROXY / PROXYTYPE_SOCKS4 / PROXYTYPE_SOCKS4A / PROXYTYPE_SOCKS5 /
                           // PROXYTYPE_HTTP11 / PROXYTYPE_SOCKS5_UDP
    std::string host_;     // proxy host
    uint16_t    port_;     // proxy port
    std::string username_; // username
    std::string password_; // password
};

enum ProxyType
{
    kProxyTypeNone = 0,
    kProxyTypeSocks4,
    kProxyTypeSocks4A,
    kProxyTypeSocks5,
    kProxyTypeHttp11,
    kProxyTypeSocks5UDP
};
struct ProxySetting : protected ProxySet
{
	ProxySetting(){
		Clear();
	}
	virtual ~ProxySetting(){}
	void SetType(ProxyType type){ type_ = type; }
	ProxyType GetType() const{ return (ProxyType)(type_);	 }
	void SetHost(const std::string& host) { host_ = host; }
	std::string GetHost() const { return host_; }
	void SetPort(uint16_t port) { port_ = port; }
	uint16_t GetPort() const { return port_; }
	void SetUserName(const std::string& username) { username_ = username; }
	std::string GetUserName() const { return username_; }
	void SetPassowrd(const std::string& password) { password_ = password; }
	std::string GetPassowrd() const { return password_; }
	bool IsEmpty() const { return type_ == ProxyType::kProxyTypeNone; }
	void Clear()
	{
		type_ = ProxyType::kProxyTypeNone;
		host_.clear();
		port_ = 0;
		username_.clear();
		password_.clear();
	}
};
/*
 *  Purpose     Network IO binary input interface
 */
struct NIOBinaryInputInterface
{
    virtual int  Read(void *buf, size_t size) = 0;
};

/*
 *  Purpose     Network IO binary output interface
 */
struct NIOBinaryOutputInterface
{
    virtual int  Write(const void *data, size_t size) = 0;
};

/*
 *  Purpose     Network IO inputable interface
 */
struct NIOInputAbleInterface
{
    virtual void OnRead() = 0;
};

/*
 *  Purpose     Network IO outputable interface
 */
struct NIOOutputAbleInterface
{
    virtual void OnWrite() = 0;
};

/*
 *  Purpose     Network IO closeable interface
 */
struct NIOCloseAbleInterface
{
    virtual void OnClose() = 0;
};

/*
 *  Purpose     Network IO connectable interface
 */
struct NIOConnectAbleInterface
{
    virtual void OnConnect() = 0;
};

/*
 *  Purpose     Network IO acceptable interface
 */
struct NIOServerAbleInterface
{
    virtual void OnAccept(SOCKET fd) = 0;
};

/*
 *  Purpose     Network IO connection interface
 */
struct NIOConnectionInterface : public NIOInputAbleInterface,
                                public NIOOutputAbleInterface ,
                                public NIOCloseAbleInterface
{
};

/*
 *  Purpose     Network IO client interface
 */
struct NIOClientInterface : public NIOConnectionInterface,
                            public NIOConnectAbleInterface
{
};

/*
 *  Purpose     Network IO binary stream interface
 */
struct NIOBinaryStreamInterface : public NIOBinaryInputInterface,
                                  public NIOBinaryOutputInterface
{
} ;

/*
 *  Purpose     Network IO binary client interface
 */
struct NIOBinaryClientInterface : public NIOBinaryStreamInterface,
                                  public NIOClientInterface
{
    virtual void Bind(NIOClientInterface *client) = 0;
};

/*
 *  Purpose     Network IO binary connection interface
 */
struct NIOBinaryConnectionInterface : public NIOBinaryStreamInterface,
                                      public NIOConnectionInterface
{
    virtual void Bind(NIOConnectionInterface *connection) = 0;
};

}     //    namespace nbase


inline int socket_error()
{
#if defined(OS_WIN)
    return ::WSAGetLastError();
#else
    return errno;
#endif
}

inline bool would_block()
{
	register int err = socket_error();
#if defined(OS_WIN)
	if (err == WSAEWOULDBLOCK || err == WSAEINPROGRESS)
		return true;
#else
	if (err == EWOULDBLOCK || err == EINPROGRESS || err == EAGAIN)
		return true;
#endif
	//DEFLOG(nbase::LogInterface::LV_PRO, __FILE__, __LINE__, "[nbase::would_block] err_code:%d", err);
	return false;
}

inline void set_block(SOCKET s, bool b)
{
#if defined(OS_WIN)
      unsigned long w = b ? 0 : 1;
      ioctlsocket(s, FIONBIO, &w);
#else
      int val = fcntl(s, F_GETFL, 0);
      if (b)
      {
		  if (val & O_NONBLOCK)
			  fcntl(s, F_SETFL,val & (~O_NONBLOCK));
      }
      else
      {
		  if (!(val & O_NONBLOCK))
			  fcntl(s, F_SETFL, val | O_NONBLOCK);
      }
#endif
}

inline void reuse_address(SOCKET s)
{
	int v = 1;
	int r = ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&v, sizeof(v));
	if (r != 0)
	{
		int err = socket_error();
		// use trace here?
		printf("reuse_addres occur error, err_code:%d\n", err);
	}
}

inline void allow_broadcast(SOCKET s)
{
	int v = 1;
	int r = ::setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char *)&v, sizeof(v));
	if (r != 0)
	{
		int err = socket_error();
		// use trace here?
		printf("allow_broadcast occur error, err_code:%d\n", err);
	}
}

#endif  // BASE_NETWORK_NIO_H_


