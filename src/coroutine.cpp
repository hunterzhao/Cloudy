#include "coroutine.h"
#include "cloudmessage.h"
#include "coqueue.h"

namespace cloud {
Coroutine::Coroutine() 
   :id_(-1), cap_(0), size_(0), status_(COR_READY), stack_(nullptr) {}

Coroutine::~Coroutine() {}

void Coroutine::SetQueue(Coqueue* queue) {
	queue_ = queue;
}


void Coroutine::Run() {
    int ret = OnStart();
	
	if (0 != ret) {
		//LOG->error("start error");
	}

    while(queue_) { 
        CloudMessage* msg = queue_->WaitRead(this);
        if (nullptr == msg) continue;
        if (OnEvent(msg) < 0)
        	break;
    }

    ret = OnStop();
    
    if (0 != ret) {
		//LOG->error("stop error");
	}
}
}

