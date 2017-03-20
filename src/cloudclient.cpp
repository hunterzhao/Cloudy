#include "cloudclient.h"
#include "cloud.h"
#include "cloudtcphandle.h"
#include "cloudmessage.h"

namespace cloud {
void CloudClient::OnWrite(uv_write_t* req) {
	printf("wrote.\n");
}

void CloudClient::connect_cb(uv_connect_t* connection, int status) {
	printf("connected.\n");
	uv_stream_t* stream = connection->handle;
	CloudClient* client = ((CloudClient*)connection->data);
	stream->data = client;
	uv_read_start(stream, TcpHandle::alloc_cb, TcpHandle::read_cb); 
	CloudMessage msg;
 //   std::string s = "i am a message from network.";  
    msg.AddOption("data","i am a message from network.");    
	client->SendMessage(msg);
	//client->OnConnected();
	//msg->FreeData();
}

void CloudClient::SetLoop(Loop& loop) {
	loop_ = loop;
    int r = uv_tcp_init(loop.self(), &tcpClient_);
	if (r) {
	    /* TODO: Error codes */
	    // LOG->error("Socket creation error");
	}
	connect_ = (uv_connect_t*)malloc(sizeof(uv_connect_t));
}

CloudClient::CloudClient() {
	
}

CloudClient::~CloudClient() {
    if (connect_ != nullptr) {
        free(connect_);
        connect_ = nullptr;
        // LOG->warn("socket free in destructor.");
    }
    else {
    	// LOG->warn("socket already free.");
    }
}

int CloudClient::Connect(const struct sockaddr_in* addr){
	connect_->data = this;
    return uv_tcp_connect(connect_, &tcpClient_,  (const struct sockaddr*)addr, connect_cb);
}

int CloudClient::SendMessage(CloudMessage& msg) {
	
    return connect_ == nullptr ? -1 : TcpHandle::SendMessage(connect_->handle, msg);
} 

int CloudClient::SendData(const char* data, size_t datalen) {
	
    return connect_ == nullptr ? -1 : TcpHandle::SendData(connect_->handle, data, datalen);
} 

void CloudClient::Close(uv_stream_t* handle) {
    uv_shutdown_t* sreq;
    sreq = (uv_shutdown_t*)malloc(sizeof* sreq);
    ASSERT(0 == uv_shutdown(sreq, handle, after_shutdown));
}

void CloudClient::OnMessage(CloudMessage& msg, uv_stream_t* tcp) {
	// printf("%s\n",msg.GetData());
 //    CloudMessage msg2;
 // //   std::string s = "i am a message from network.";  
 //    msg2.SetData("i am a message from network.");
	// SendMessage(msg2);
}

void CloudClient::OnConnected() {}//provide the interface for user code
void CloudClient::OnDisConnected() {}
}