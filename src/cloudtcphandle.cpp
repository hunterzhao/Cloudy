#include <string>
#include "cloud.h"
#include "cloudtcphandle.h"
#include "cloudmessage.h"

namespace cloud {
void TcpHandle::read_cb(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf) {
    TcpHandle* handle = (TcpHandle*)(tcp->data);
    // LOG->info("data coming");
    if (nread < 0) {
	    /* Error or EOF */
	    ASSERT(nread == UV_EOF);
        // LOG->info("end of file");
	    free(buf->base);
	    uv_shutdown_t* sreq = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
	    ASSERT(0 == uv_shutdown(sreq, tcp, after_shutdown));
        //free(tcp); //the opposite point end the connect,free the stream
	    return;
    }

    if (nread == 0) {
	    /* Everything OK, but nothing read. */
        // LOG->info("nothing read");
	    free(buf->base);
	    return;
    }

    handle->OnRead(tcp, nread, buf);
}

void TcpHandle::write_cb(uv_write_t* req, int status) {
    // LOG->info("wrote.");
    free(req);
    req = nullptr;
    //char* msgdata = (char*)(req->data);
    //free(msgdata);
}

void TcpHandle::close_cb(uv_handle_t* handle) {
    // if (handle != nullptr) {
    //     free(handle);
    //     handle = nullptr;
    // }
    // LOG->info("wrote.");
}

void TcpHandle::alloc_cb(uv_handle_t* handle, size_t suggested_size,
                                                uv_buf_t* buf) {
   buf->base = (char*)malloc(suggested_size * sizeof(char));
   if (NULL == buf->base)
      exit(1);
   memset(buf->base, 0, suggested_size * sizeof(char));
   buf->len = suggested_size;
}

void TcpHandle::after_shutdown(uv_shutdown_t* req, int status) {
   uv_close((uv_handle_t*) req->handle, TcpHandle::close_cb);
   free(req);
   req = nullptr;
}

TcpHandle::TcpHandle() {

}

TcpHandle::~TcpHandle() {

}

int TcpHandle::SetNoDelay(int enable) {
	return uv_tcp_nodelay(handle_, enable);
}

int TcpHandle::SetKeepAlive(int enable, unsigned int delay) {
	return uv_tcp_keepalive(handle_, enable, delay);
}

int TcpHandle::SetSimultaneousAccepts(int enable) {
    return uv_tcp_simultaneous_accepts(handle_, enable);
}

int TcpHandle::SendMessage(uv_stream_t* dest, CloudMessage& msg) {
    std::string data = msg.Decode();
    //std::string data = msg.GetData();
    size_t datalen = data.size() + 1;
    uv_buf_t buffer[] = {
        {.base = (char*)data.c_str(), .len = datalen}
    };
    uv_write_t* request = (uv_write_t*)malloc(sizeof(uv_write_t));
    uv_write(request, dest, buffer, 1, write_cb);
    return 0;
}

int TcpHandle::SendData(uv_stream_t* dest, const void* data, size_t data_len) {
    uv_buf_t buffer[] = {
        {.base = (char*)data, .len = data_len}
    };
    uv_write_t* request = (uv_write_t*)malloc(sizeof(uv_write_t));
    uv_write(request, dest, buffer, 1, write_cb);
    return 0;
}

void TcpHandle::OnRead(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf) {
    /*
    * receive data and decode
    */
    // LOG->info("data is {}",buf->base);
    //lock?
    CloudMessage msg;
    msg.Encode(buf->base); //encode the json string
    free(buf->base);
	OnMessage(msg, tcp);
    //lock?
}

void TcpHandle::OnWrite(uv_write_t* req) {
	/**/
    // LOG->info("TcpHandle::OnWrite");
}

void TcpHandle::OnMessage(CloudMessage& msg, uv_stream_t* tcp) {
    // LOG->info("TcpHandle::OnMessage");
}	
}