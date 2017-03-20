#include <utility>
#include <stdio.h>
#include "coqueue.h"
#include "block.h"

namespace cloud {
Coqueue::Coqueue() : block_write_(new Block), block_read_(new Block) {
}

Coqueue::~Coqueue() {
    delete block_read_;
    delete block_write_;
}

CloudMessage Coqueue::WaitRead(Coroutine* co) {
    while (0 == size_) {
        //如果是等待读的话就是阻塞状态
        co->SetStatus(COR_BLOCK);
        block_read_->AddBlock(co);
        /* release one coroutine */
    } 
    CloudMessage msg = queue_msg_.front();
    queue_msg_.pop();
    size_--;
    if (cap_ - 1 == size_) {
        //wake the coroutine wait to write ,since the queue is not full
        block_write_->WakeBlock();
    }
  
    return msg;
}

void Coqueue::WaitWrite(CloudMessage& val, Coroutine* co) {
    while (cap_ == size_) {
        //如果是等待写的话就是阻塞
        co->SetStatus(COR_BLOCK);
        block_write_->AddBlock(co);
    } 
    //如果是直接写的话就是添加调度器的task队列中，写完继续执行
    queue_msg_.push(std::move(val));
    size_++;
    
    co->SetStatus(COR_BLOCK);
    Schedule::Instance().AddMission(co);
    // wake one coroutine block in this queue
    block_read_->WakeBlock();
    // 让出调度器
    Schedule::Instance().YieldCo(co);
}

void Coqueue::NoWaitWrite(CloudMessage& val) {
    if(cap_ == size_) cap_++;
    queue_msg_.push(std::move(val));
    size_++;
    block_read_->WakeBlock();
}

}