#ifndef COQUEUE_H_
#define COQUEUE_H_

#include <map>
#include <queue>
namespace cloud {
class Coroutine;
class CloudMessage;
class Block;
class Coqueue {
public:
    Coqueue();
    ~Coqueue();
    CloudMessage* WaitRead(Coroutine* co);
    void WaitWrite(CloudMessage* val, Coroutine* co);

private:
    std::queue<CloudMessage*> queue_msg_;
    Block* block_write_;
    Block* block_read_;
    int size_;
    int cap_ = 5;
};

class Block;
class CoqueueMgr {
public:
    static CoqueueMgr& Instance() {
    	static CoqueueMgr mgr_; //it's thread-safe in c++11
        return mgr_;
    }

    Coqueue* CreateQueue(int id) {
        Coqueue* q = new Coqueue;
        coqueue_map_[id] = q;
        return q;
    }
    
    void SendMessage(int stageid, CloudMessage* msg, Coroutine* co) {
        coqueue_map_[stageid]->WaitWrite(msg, co);
    }

private:
    CoqueueMgr() {}
    CoqueueMgr(const CoqueueMgr&) = delete;
    CoqueueMgr& operator=(const CoqueueMgr&) = delete;

    std::map<int, Coqueue*> coqueue_map_;
};


}
#endif //end of COQUEUE_H_