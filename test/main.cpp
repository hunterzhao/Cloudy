#include <iostream>
#include <assert.h>
#include "coqueue.h"
#include "coroutine.h"
#include "cloudmessage.h"
#include "schedule.h"
#include "coqueue.h"
#include "cloudserver.h"
#include "cloudsession.h"

using namespace cloud;

static const int64_t kStageAID = 111; 
static const int64_t kStageBID = 112; 

class TestServer : public CloudServer {
public:
    TestServer(struct sockaddr_in addr) : CloudServer(addr) {}

    void OnMessage(CloudMessage& msg, uv_stream_t* tcp) {
        CoqueueMgr::Instance().SendOuterMessage(111, msg);
        session_t session_id = SessionMgr::Instance().GetSessionID(tcp);
        //SessionMgr::Instance().SendMessage(session_id, msg);
    }
};

class StageA : public Coroutine {
public:
    int OnStart() override {
        
        CloudMessage msg; 
        msg.AddOption("data","world");
        assert(msg.document_.IsObject());
        CoqueueMgr::Instance().SendMessage(kStageBID, msg, this);
        std::cout<<"A:message already send"<<std::endl;
        
        return 0;
    }

    int OnEvent(CloudMessage& msg) override {
        
        std::cout<<"A:"<<msg.GetOptionStr("data")<<std::endl;
        // LOG->info("message get {}", msg->data);
        //CloudMessage msg2;
        //msg2.SetData("hello");     
       // SessionMgr::Instance().SendMessage(, msg2);
        //std::cout<<"B:message already send"<<std::endl;
        return 0;
    }

    int OnStop() override {
        return 0;
    }
};

class StageB : public Coroutine {
public:
    int OnStart() override {
        return 0;
    }

    int OnEvent(CloudMessage& msg) override {
        std::cout<<"B"<<msg.GetOptionStr("data")<<std::endl;
        
        CloudMessage msg2;
        msg2.AddOption("data","hello");    
        CoqueueMgr::Instance().SendMessage(kStageAID, msg2, this);
        std::cout<<"B:message already send"<<std::endl;
        
        return 0;
    }

    int OnStop() override {
        return 0;
    }
};

int main() {    
    //启动server
    struct sockaddr_in sin = {0};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9123);
    TestServer server(sin);
    CloudServer* testServer = &server;
    testServer->Start();
    
    //配置业务stage
    StageB* stageB = new StageB;
    stageB->SetId(kStageBID);
    stageB->SetQueue(CoqueueMgr::Instance().CreateQueue(kStageBID));
    Schedule::Instance().AddTask(stageB);
    
    StageA* stageA = new StageA;
    stageA->SetId(kStageAID);
    stageA->SetQueue(CoqueueMgr::Instance().CreateQueue(kStageAID));
    Schedule::Instance().AddTask(stageA);
    //CoqueueMgr::Instance().SendMessage(kStageAID, msg, stageB);

    Schedule::Instance().Loop();
    delete stageA;
    delete stageB;
}
