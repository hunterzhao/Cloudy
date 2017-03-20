#ifndef CLOUDPROCESSOR_H
#define CLOUDPROCESSOR_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <ucontext.h>

#define STACK_SIZE (20 * 1024 * 1024)

namespace cloud {
class Coroutine;
class Processor {
public:
   static void mainfunc(uint32_t low32, uint32_t hi32);
   void ResumeCo(Coroutine* co);
   void SaveStack(Coroutine* co, char *top);
   void YieldCo(Coroutine* co);
   void Run();
   void Push(Coroutine* co);
   Coroutine* WaitAndPop();

private:
   std::queue<Coroutine*> ready_;
   mutable std::mutex mut_ ;
   std::condition_variable cond_;
   Coroutine* running_ = nullptr;
   ucontext_t main_;
   char stack_[STACK_SIZE];
};
}//end of namespace cloud
#endif