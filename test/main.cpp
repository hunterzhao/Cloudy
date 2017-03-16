#include <iostream>
#include "coqueue.h"
#include "coroutine.h"
#include "cloudmessage.h"
#include "schedule.h"
#include "coqueue.h"

using namespace cloud;

static const int64_t kStageAID = 111; 
static const int64_t kStageBID = 112; 

class StageA : public Coroutine {
public:
    int OnStart() override {
        CloudMessage msg;
        msg.SetData("hello", 6);     
        CoqueueMgr::Instance().SendMessage(kStageBID, msg, this);
        std::cout<<"A:message already send"<<std::endl;
        return 0;
    }

    int OnEvent(CloudMessage& msg) override {
        std::cout<<"A:"<<msg.GetData()<<std::endl;
        // LOG->info("message get {}", msg->data);
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

        std::cout<<"B"<<msg.GetData()<<std::endl;
        // LOG->info("message get {}", msg->data);

        CloudMessage msg2;
        msg2.SetData("world", 6);     
        CoqueueMgr::Instance().SendMessage(kStageAID, msg2, this);
        std::cout<<"B:message already send"<<std::endl;
        return 0;
    }

    int OnStop() override {
        return 0;
    }
};

int main() {

    StageA* stageA = new StageA;
    stageA->SetId(kStageAID);
    stageA->SetQueue(CoqueueMgr::Instance().CreateQueue(kStageAID));
    Schedule::Instance().AddTask(stageA);
    
    StageB* stageB = new StageB;
    stageB->SetId(kStageBID);
    stageB->SetQueue(CoqueueMgr::Instance().CreateQueue(kStageBID));
    Schedule::Instance().AddTask(stageB);
    
   
    //CoqueueMgr::Instance().SendMessage(kStageAID, msg, stageB);

    Schedule::Instance().Loop();
    delete stageA;
    delete stageB;

//    TestServer *server = new TestServer();
//    QtpManager::Main().AddMember(server);

//    struct sockaddr_in sin = {0};
//    sin.sin_family = AF_INET;
//    sin.sin_port = htons(PORT);
    
//    if (server->Bind((sockaddr *) &sin, sizeof(sin)) != 0) {
//      LOG(error) << "hi";
//      return -1;
//    }
}
