#include "coqueue.h"
#include "block.h"
namespace cloud {
Coqueue::Coqueue() : block_write_(new Block), block_read_(new Block) {
}

Coqueue::~Coqueue() {
    delete block_read_;
    delete block_write_;
}

CloudMessage* Coqueue::WaitRead(Coroutine* co) {
    while (0 == size_) {
    	block_read_->AddBlock(co);
    	/* release one coroutine */
    } 
    CloudMessage* msg = queue_msg_.front();
    queue_msg_.pop();
    size_--;
    if (cap_ - 1 == size_) {
    	//wake the coroutine wait to write ,since the queue is not full
    	block_write_->WakeBlock();
    }
  
    return msg;
}

void Coqueue::WaitWrite(CloudMessage* val, Coroutine* co) {
    while (cap_ == size_) {
    	block_write_->AddBlock(co);
    } 
    
	queue_msg_.push(val);
	size_++;
	// wake the coroutine block in block_read_
    //block_read_->WakeBlock();
    Schedule::Instance().YieldCo(co);
}

}