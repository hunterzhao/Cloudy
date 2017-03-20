#include <ucontext.h>
#include <unistd.h>
#include <assert.h>
#include <memory.h>
#include <iostream>
#include <thread>
#include "schedule.h"
#include "coroutine.h"
#include "cloudprocessor.h"

namespace cloud {
Schedule::Schedule() {
    epollfd_ = epoll_create1(0);
        if (epollfd_ == -1) {
           perror("epoll_create1");
           exit(EXIT_FAILURE);
    }

    //初始化线程池
    for (int i = 0; i < NUM_THREAD_POOL; i++) {
        Processor* pro =new Processor;
        processors_.emplace_back(pro);
        std::thread run([pro]{ pro->Run();});
        run.detach();
    }
}

Schedule::~Schedule() {
    close(epollfd_);
    for (int i = 0; i < NUM_THREAD_POOL; i++) {
        delete processors_[i];
    }
}

void Schedule::YieldCo(Coroutine* co) {
    //只会让自己线程上协程挂起，所以不需要上锁
    processors_[co->pro_]->YieldCo(co);
} 

//只在任务新创建时添加一次
void Schedule::AddTask(Coroutine* co) {
	auto iter_id = co_hash_map_.find(co->id_);
	if (iter_id == co_hash_map_.end())
	    co_hash_map_[co->id_] = co;
    
    auto iter_fd = co_fd_map_.find(co->fd_);
    if (iter_fd == co_fd_map_.end()) 
        co_fd_map_[co->fd_] = co;

	if (co->status_ == COR_READY/* || co->status_ == COR_BLOCK*/)                                                                                                  
		AddMission(co);
    
    //为任务注册到epoll上
    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = co->fd_;
    if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, co->fd_, &ev) == -1) {
        perror("epoll_ctl: error");
        exit(EXIT_FAILURE);
    }
}	

//将待执行的协程添加到执行器中等待执行
void Schedule::AddMission(Coroutine* co) {
    ready_.push(co);
}

void Schedule::Loop() {
    uint64_t u;
    while(1) {
    	while(!ready_.empty()) {     
            //主线程直接执行 
            Coroutine* co = ready_.front();
            ready_.pop();
            //分发给各个执行器
            processors_[co->pro_]->Push(co);
    	}
        
        //阻塞在这里，等待下一波任务
        int nfds = epoll_wait(epollfd_, events_, MAX_EVENTS, -1);
        printf("epoll\n");
        if(nfds == -1) {
            exit(EXIT_FAILURE);
        }
        
        for(int n = 0; n < nfds; n++) {
            int fd_t = events_[n].data.fd;
            if(co_fd_map_.find(fd_t) != co_fd_map_.end()) {    
                read(fd_t, &u, sizeof(uint64_t));
                ready_.push(co_fd_map_[fd_t]);         
            }
        }
        
    }
}
} //end of namespace cloud