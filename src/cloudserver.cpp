#include <thread>
#include "cloud.h"
#include "cloudserver.h"
#include "cloudsession.h"
#include "coqueue.h"
#include "cloudlog.h"

namespace cloud {
void CloudServer::ServerRun() {
   Loop loop;
   SetLoop(loop);
   Bind(&addr_, 0);
   Listen(SOMAXCONN);
   LOG->info("server on.");
   loop.loop_run(Loop::RUN_DEFAULT);
}

void CloudServer::SetLoop(Loop& loop) {
   int r = uv_tcp_init(loop_.self(), &tcpServer_);
  // server_closed_ = 0;
   if (r) {
      ;// LOG->error("Socket creation error");
   }
}

void CloudServer::on_connect(uv_stream_t* server, int status) {
   Loop loop = ((CloudServer*)(server->data) )->GetLoop();
   printf("accpet connect\n");
   uv_stream_t* stream;
  
   if (status != 0) {
     // LOG->error("Connect error {}",uv_err_name(status));
   }
   ASSERT(status == 0) ;

   stream = (uv_stream_t*)malloc(sizeof(uv_tcp_t));
   ASSERT(stream != NULL);
   int r = uv_tcp_init(loop.self(), (uv_tcp_t*)stream);
   ASSERT(r == 0);
  
   r = uv_accept(server, stream);
   ASSERT(r == 0);
  
   SessionMgr::Instance().AddNewSession(stream);
  
   stream->data = (CloudServer*)(server->data);
   r = uv_read_start(stream, TcpHandle::alloc_cb, TcpHandle::read_cb);
   ASSERT(r == 0);
   // printf("wait for data\n");
}

CloudServer::CloudServer(struct sockaddr_in addr) {
   addr_ = addr;
}

CloudServer::~CloudServer() {
   
}

int CloudServer::Bind(const struct sockaddr_in* addr, unsigned int flags) {
   int r = uv_tcp_bind(&tcpServer_, (const struct sockaddr*) addr, flags);
   if (r) {
	    /* TODO: Error codes */
        ;// LOG->error("Bind error");
	    return 1;
   }
   return r;
}

int CloudServer::Listen(int blacklog) {
   tcpServer_.data = this;
   int r = uv_listen((uv_stream_t*)&tcpServer_, blacklog, on_connect);
   if (r) {
     /* TODO: Error codes */
     ;// LOG->error("Listen error {}",uv_err_name(r));
     return 1;
   }
   return r;
}

//close the connect socket
void CloudServer::Close(uv_stream_t* handle) {
   uv_shutdown_t* sreq;
   sreq = (uv_shutdown_t*)malloc(sizeof* sreq);
   ASSERT(0 == uv_shutdown(sreq, handle, TcpHandle::after_shutdown));
   // LOG->info("close.");
}

void CloudServer::OnMessage(CloudMessage& msg, uv_stream_t* tcp) {
   //int type = msg.GetOptionInt("type");
}

void CloudServer::Start() {
   std::thread run([this] {this->ServerRun();});
   run.detach();
}

Loop& CloudServer::GetLoop() {
   return loop_;
}
}