#ifndef CLOUDSERVER_H_
#define CLOUDSERVER_H_

#include "cloudmessage.h"
#include "cloudtcphandle.h"
#include "loop.h"

namespace cloud {
class CloudServer : public TcpHandle {
public:
	void ServerRun();

	void SetLoop(Loop& loop);
//static void after_write(uv_write_t* req, int status);
	static void on_connect(uv_stream_t* server, int status);

	CloudServer(struct sockaddr_in addr);

	virtual ~CloudServer();
    
    void Start();

	int Bind(const struct sockaddr_in* addr, unsigned int flags);

	int Listen(int blacklog);
    
    void Close(uv_stream_t* handle);
    
    virtual void OnMessage(CloudMessage& msg, uv_stream_t* tcp);

    Loop& GetLoop();
    
    struct sockaddr_in addr_;
private:
    uv_tcp_t tcpServer_;
    Loop loop_; 
    int server_closed_;
};
}
#endif