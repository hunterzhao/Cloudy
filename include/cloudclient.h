#ifndef CLOUDCLIENT_H_
#define CLOUDCLIENT_H_

#include <uv.h>
#include "cloudtcphandle.h"
#include "loop.h"

namespace cloud {
class CloudMessage;
class CloudClient : public TcpHandle {
public:
    static void connect_cb(uv_connect_t* connection, int status);
    
    void SetLoop(Loop& loop);

    void OnWrite(uv_write_t* req);
    
    CloudClient();
    
    virtual ~CloudClient();
    
    int Connect(const struct sockaddr_in* addr);

    int SendMessage(CloudMessage& msg); 

    int SendData(const char* data, size_t datalen);
    
    void Close(uv_stream_t* handle);
    
    virtual void OnMessage(CloudMessage& msg, uv_stream_t* tcp);
    virtual void OnConnected();//provide the interface for user code
    virtual void OnDisConnected();
private:
    uv_tcp_t tcpClient_; 
	uv_connect_t* connect_; 
	Loop loop_;
};
}
#endif