#ifndef COROUTINE_H_
#define COROUTINE_H_

#include <stdint.h>
#include <ucontext.h>
#include <mutex>

namespace cloud {
enum {
	COR_READY,
	COR_RUNNING,
	COR_BLOCK,
    COR_STOP
};
class Coqueue;
class CloudMessage;
class Coroutine {
	friend class Schedule;
	friend class Processor;
public:
	Coroutine();

    virtual ~Coroutine();    

    void SetId(int64_t id) { id_ = id;}

	int64_t GetId() { return id_;}

    void SetQueue(Coqueue* queue);

    int  GetStatus() { return status_;}

    void SetStatus(int status) {status_ = status;}

    int GetFd() { return fd_;}
    
    void SetPro(int pro) { pro_ = pro;}

    int GetPro() { return pro_;}

    virtual int OnStart() {return 0;}
	
	virtual int OnEvent(CloudMessage& msg) = 0;
    
    virtual int OnStop() { return 0;}
	
	virtual void Run();

private:
	int64_t id_;
	int cap_;
	int size_;
	int status_;	
	char* stack_;
	ucontext_t ctx_;
	Coqueue* queue_;
	int fd_;
	int pro_;
    
};
}
#endif

