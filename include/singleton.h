#ifndef SINGLETON_H_
#define SINGLETON_H_
#include "spinlock.h"
namespace cloud {
template<typename T>
class Singleton : public SpinLock {
public:
   static T& Instance() {
   	   static T t;
   	   return t;
   }

protected:
   Singleton() = default;
   Singleton(const Singleton&) = delete;
   Singleton& operator=(const Singleton&) = delete;
};
}
#endif