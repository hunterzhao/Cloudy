#include <sys/eventfd.h>
#include <unistd.h>
#include <stdio.h>
#include "coroutine.h"
#include "cloudmessage.h"
#include "coqueue.h"
#include "schedule.h"

#define handle_error(msg) \
    do { perror(msg); exit(1); } while (0)

namespace cloud {
Coroutine::Coroutine() 
   :id_(-1), cap_(0), size_(0), status_(COR_READY), stack_(nullptr) {
    if ( (fd_ = eventfd(0, EFD_NONBLOCK)) == -1 )
             handle_error("eventfd failed");
}

Coroutine::~Coroutine() {
    if (nullptr != stack_) {
        delete[] stack_;
    }
    if (nullptr != queue_) {
        CoqueueMgr::Instance().DeleteQueue(queue_);
    }

    close(fd_);
}

void Coroutine::SetQueue(Coqueue* queue) {
	queue_ = queue;
}


void Coroutine::Run() {
    int ret = OnStart();
	
	if (0 != ret) {
		//LOG->error("start error");
	}
   // 让出调度器，等待下次的消息唤醒
    printf("suspend");
    Schedule::Instance().YieldCo(this);
    printf("active");
    
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

