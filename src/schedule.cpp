#include <ucontext.h>
#include <assert.h>
#include <memory.h>
#include "schedule.h"
#include "coroutine.h"

namespace cloud {
void Schedule::mainfunc(uint32_t low32, uint32_t hi32) {
	uintptr_t ptr = (uintptr_t) low32 | ((uintptr_t) hi32 << 32);
	Schedule *S = reinterpret_cast<Schedule *>(ptr);
	int64_t id = S->running_;
	Coroutine* co = S->co_hash_map_[id];
	co->Run();
	delete co;
	S->co_hash_map_.erase(id);
	S->running_ = -1;
}

void Schedule::ResumeCo(int64_t id) {
	assert(-1 == running_);

	std::map<int64_t, Coroutine*>::iterator pos = co_hash_map_.find(id);
    
    // 协程hash表中没有找到，或者找到的协程内容为空
	if (pos == co_hash_map_.end()) {
		return;
	}

	Coroutine *co = pos->second;
	if (nullptr == co) {
		return;
	}

	int status = co->status_;
    switch (status) {
        case COR_READY: {
        	getcontext(&co->ctx_);
        	co->ctx_.uc_stack.ss_sp = stack_;  //所有协程的运行栈都共享调度器的这个stack！！！，并且先用高地址，再用低地址
        	co->ctx_.uc_stack.ss_size = STACK_SIZE;
        	co->ctx_.uc_link = &main_; //后继上下文
        	running_ = id;
        	co->status_ = COR_RUNNING;
        	uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
        	makecontext(&co->ctx_, (void (*)(void)) mainfunc, 2,
        		(uint32_t)ptr,
        		(uint32_t)(ptr>>32));
        	swapcontext(&main_, &co->ctx_);
        }
            break;
        case COR_BLOCK: {
        	////将协程的stack全部存到调度器中的的尾部
        	::memcpy(stack_ + STACK_SIZE - co->size_, co->stack_, co->size_);
        	running_ = id;
        	co->status_ = COR_RUNNING;
        	swapcontext(&main_, &co->ctx_);
        }
            break;
        default:
            return;
    }
}

void Schedule::SaveStack(Coroutine* co, char *top) {
	char dummy = 0; //当前位置
	assert(top - &dummy <= STACK_SIZE);
	if (co->cap_ < top - &dummy) {
		delete[] co->stack_;
		co->cap_ = top - &dummy;
		co->stack_ = new char[co->cap_];
	}
	co->size_ = top - &dummy;
	::memcpy(co->stack_, &dummy, co->size_);
}

void Schedule::YieldCo(Coroutine* co) {
    
    assert( co->id_ == running_ );
    co->status_ = COR_BLOCK;
    
    //备份
    SaveStack(co, stack_ + STACK_SIZE);   //将调度器中的的尾部的内容全部存到协程的stack里面
    running_ = -1;
    //协程把自己的栈存放到了S的stack的起始位置  切换到S main的栈空间
    swapcontext(&co->ctx_, &main_);
} 

void Schedule::AddTask(Coroutine* co) {
	auto iter = co_hash_map_.find(co->id_);
	if (iter == co_hash_map_.end())
	    co_hash_map_[co->id_] = co;

	if (co->status_ == COR_READY || co->status_ == COR_BLOCK)                                                                                                  
		ready_.push(co->id_);
}	

void Schedule::Loop() {
	while(!ready_.empty()) {
        //分发给各个执行器

        //主线程直接执行 
        int64_t id = ready_.front();
        ready_.pop();
        ResumeCo(id);
	}
}
}