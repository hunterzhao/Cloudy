#ifndef BLOCK_H_
#define BLOCK_H_

#include <queue>
#include <assert.h>
#include <unistd.h>
#include "coroutine.h"
#include "schedule.h"

namespace cloud {
class Block {
public:
   void WakeBlock() { 	   
       /* add to task list in schedule */
     if (!block_queue_.empty()) {
   	   Coroutine* co = block_queue_.front();
       assert(co->GetStatus() == COR_BLOCK);
   	   block_queue_.pop();
       uint64_t u = 1;
       write(co->GetFd(), &u, sizeof(uint64_t));
       // printf("\npush for epoll\n");
   	   //Schedule::Instance().AddTask(co);
     }
   }
   void AddBlock(Coroutine* co) {
   	   /* add to task list in schedule */
       block_queue_.push(co);
   }

private:
   std::queue<Coroutine*> block_queue_;
};
}
#endif