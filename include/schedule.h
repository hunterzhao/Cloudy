#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <sys/epoll.h>
#include <queue>
#include <map>
#include <vector>
#include <thread>

#define MAX_EVENTS 20
#define NUM_THREAD_POOL 4

namespace cloud {
class Coroutine;
class Processor;
class Schedule {
public:
	static Schedule& Instance() {
		static Schedule sche;
        return sche;
	}

   ~Schedule();
   void YieldCo(Coroutine* co);
   void AddTask(Coroutine* co);
   void AddMission(Coroutine* co);
   void Loop();
    
private:
   Schedule();
   Schedule(const Schedule&) = delete;
   Schedule& operator=(const Schedule&) = delete;
   std::queue<Coroutine*> ready_;
   std::map<int64_t, Coroutine*> co_hash_map_;
   std::map<int, Coroutine*> co_fd_map_;
   std::vector<Processor*> processors_;
   int64_t running_ = -1;
   int epollfd_;
   struct epoll_event events_[MAX_EVENTS];
};
}
#endif