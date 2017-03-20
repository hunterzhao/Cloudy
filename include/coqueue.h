#ifndef COQUEUE_H_
#define COQUEUE_H_

#include <map>
#include <queue>
#include <mutex>
#include <assert.h>
#include "cloudmessage.h"
#include "singleton.h"

namespace cloud {
class Coroutine;
class Block;
class Coqueue {
public:
    Coqueue();
    ~Coqueue();
    CloudMessage WaitRead(Coroutine* co);
    void WaitWrite(CloudMessage& val, Coroutine* co);
    void NoWaitWrite(CloudMessage& val);

private:
    std::queue<CloudMessage> queue_msg_;
    Block* block_write_;
    Block* block_read_;
    int size_;
    int cap_ = 5;
};

class Block;
class CoqueueMgr : public Singleton<CoqueueMgr> {
public:
    friend class Singleton<CoqueueMgr>;
    
    ~CoqueueMgr() {
        for (auto& it : coqueue_map_)
            delete it.second;
    }

    Coqueue* CreateQueue(int id) {
        std::lock_guard<CoqueueMgr> lck(*this);
        Coqueue* q = new Coqueue;
        coqueue_map_[id] = q;
        return q;
    }

    void DeleteQueue(Coqueue* coqueue) {
        std::lock_guard<CoqueueMgr> lck(*this);
        delete coqueue;
    }
    
    void SendMessage(int stageid, CloudMessage& msg, Coroutine* co) {
        std::lock_guard<CoqueueMgr> lck(*this);
        assert(msg.document_.IsObject());
        coqueue_map_[stageid]->WaitWrite(msg, co);
    }

    void SendOuterMessage(int stageid, CloudMessage& msg) {
        std::lock_guard<CoqueueMgr> lck(*this);
        coqueue_map_[stageid]->NoWaitWrite(msg);
    }

private:

    CoqueueMgr() = default;
    std::map<int, Coqueue*> coqueue_map_;
};


}
#endif //end of COQUEUE_H_