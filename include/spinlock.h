#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include <atomic>

namespace cloud {
class SpinLock {
  std::atomic_flag flag;
public:
  SpinLock() : flag(ATOMIC_FLAG_INIT) { }
  void lock() {
    while (flag.test_and_set(std::memory_order_acquire));
  }
  void unlock() {
    flag.clear(std::memory_order_release);
  }
};
}
#endif