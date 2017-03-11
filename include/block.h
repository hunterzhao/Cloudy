#ifndef BLOCK_H_
#define BLOCK_H_

#include <queue>
#include <assert.h>
#include "coroutine.h"
#include "schedule.h"

namespace cloud {
class Block {
public:
   void WakeBlock() { 	   
       /* add to task list in schedule */
    if(!block_queue_.empty()) {
   	   Coroutine* co = block_queue_.front();
       assert(co->GetStatus() == COR_BLOCK);
   	   co->SetStatus(COR_READY);
   	   block_queue_.pop();
   	   Schedule::Instance().AddTask(co);
     }
   }
   void AddBlock(Coroutine* co) {
   	   /* add to task list in schedule */
   	   assert(co->GetStatus() == COR_RUNNING);
       block_queue_.push(co);
       Schedule::Instance().YieldCo(co);
   }

private:
   std::queue<Coroutine*> block_queue_;
};
}
#endif