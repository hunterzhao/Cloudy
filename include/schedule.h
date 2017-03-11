#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <queue>
#include <map>

#define STACK_SIZE (20 * 1024 * 1024)

namespace cloud {
class Coroutine;
class Schedule {
public:
	static Schedule& Instance() {
		static Schedule sche;
        return sche;
	}
   static void mainfunc(uint32_t low32, uint32_t hi32);
   void ResumeCo(int64_t id);
   void AddTask(Coroutine* co);
   void SaveStack(Coroutine* co, char *top);
   void YieldCo(Coroutine* co);
   void Loop();

private:
   Schedule() {}
   Schedule(const Schedule&) = delete;
   Schedule& operator=(const Schedule&) = delete;
   std::queue<int64_t> ready_;
   std::map<int64_t, Coroutine*> co_hash_map_;
   char stack_[STACK_SIZE];
   ucontext_t main_;
   int64_t running_ = -1;
};
}
#endif