#include "coroutine.h"
#include "cloudmessage.h"
#include "coqueue.h"
#include <iostream>
namespace cloud {
Coroutine::Coroutine() 
   :id_(-1), cap_(0), size_(0), status_(COR_READY), stack_(nullptr) {}

Coroutine::~Coroutine() {
    if (nullptr != stack_) {
        delete[] stack_;
    }
    if (nullptr != queue_) {
        CoqueueMgr::Instance().DeleteQueue(queue_);
    }
}

void Coroutine::SetQueue(Coqueue* queue) {
	queue_ = queue;
}


void Coroutine::Run() {
    int ret = OnStart();
	
	if (0 != ret) {
		//LOG->error("start error");
	}

    while(queue_) { 
        CloudMessage msg = queue_->WaitRead(this);
        if (OnEvent(msg) < 0)
        	break;
    }

    ret = OnStop();
    
    if (0 != ret) {
		//LOG->error("stop error");
	}
}
}

