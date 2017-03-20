#include <memory.h>
#include <assert.h>
#include "cloudprocessor.h"
#include "schedule.h"
#include "coroutine.h"

namespace cloud {

void Processor::mainfunc(uint32_t low32, uint32_t hi32) {
	uintptr_t ptr = (uintptr_t) low32 | ((uintptr_t) hi32 << 32);
	Processor *S = reinterpret_cast<Processor *>(ptr);
	Coroutine* co = S->running_;
	co->Run();
	delete co;
	S->running_ = nullptr;
}

void Processor::ResumeCo(Coroutine* co) {
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
        	running_ = co;
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
        	running_ = co;
        	co->status_ = COR_RUNNING;
        	swapcontext(&main_, &co->ctx_);
        }
            break;
        default:
            return;
    }
}

void Processor::SaveStack(Coroutine* co, char *top) {
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

void Processor::YieldCo(Coroutine* co) {
    co->status_ = COR_BLOCK;
    
    //备份
    SaveStack(co, stack_ + STACK_SIZE);   //将调度器中的的尾部的内容全部存到协程的stack里面
    running_ = nullptr;
    //协程把自己的栈存放到了S的stack的起始位置  切换到S main的栈空间
    swapcontext(&co->ctx_, &main_);
}

void Processor::Push(Coroutine* co) {
    printf("wait for lock5");
	std::lock_guard<std::mutex> lk(mut_);
    printf("got5 lock\n");
    ready_.push(co);
    cond_.notify_one();
    printf("release5 lock\n");
}

Coroutine* Processor::WaitAndPop() {
    printf("wait for lock4");
    std::unique_lock<std::mutex> lk(mut_);
    printf("got4 lock\n");
    cond_.wait(lk, [this] { return !ready_.empty(); });
    Coroutine* co = ready_.front();
    ready_.pop();
    printf("release4 lock\n");
    return co;
}

void Processor::Run() {
    
    for(;;) {
        Coroutine* co = WaitAndPop();
        ResumeCo(co);
    }
}

}