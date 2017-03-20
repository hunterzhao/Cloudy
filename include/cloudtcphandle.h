#ifndef CLOUD_TCP_HANDLE_H_
#define CLOUD_TCP_HANDLE_H_

#include <uv.h>
namespace cloud {
class CloudMessage; 
class TcpHandle {
public:
    static void read_cb(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf);
    static void write_cb(uv_write_t* req, int status);
    static void close_cb(uv_handle_t* handle);
    static void alloc_cb(uv_handle_t* handle, size_t suggested_size,
                                                uv_buf_t* buf);
	static void after_shutdown(uv_shutdown_t* req, int status);

    //Enable TCP_NODELAY, which disables Nagle’s algorithm.
    int SetNoDelay(int enable);

    //Enable / disable TCP keep-alive. delay is the initial delay in seconds, ignored when enable is zero.
    int SetKeepAlive(int enable, unsigned int delay);
    
    //Enable / disable simultaneous asynchronous accept requests that are queued by the operating system when listening for new TCP connections.
    int SetSimultaneousAccepts(int enable);
    
    static int SendMessage(uv_stream_t* dest, CloudMessage& msg);
    static int SendData(uv_stream_t* dest, const void* data, size_t data_len);
    //socket create
	TcpHandle();
    
    //release socket
	virtual ~TcpHandle();

    virtual void OnRead(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf);

    virtual void OnWrite(uv_write_t* req);

    virtual void OnMessage(CloudMessage& msg, uv_stream_t* tcp);
private:
	uv_tcp_t* handle_;
};
}
#endif