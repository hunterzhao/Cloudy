#include <mutex>
#include "cloudsession.h"
#include "cloudmessage.h"
#include "cloudtcphandle.h"

namespace cloud {
 session_t SessionMgr::AddNewSession(uv_stream_t* tcp) {
  printf("wait for lock6");
    std::lock_guard<SessionMgr> lck(*this);
    printf("got6 lock\n");
    // auto it = sessionMap_.find(tcp);
    // if (it != sessionMap_.end()) return it->second;
    int session_id = maxNumber_++;
	  sessionMap_[session_id] = tcp;
    streamMap_[tcp] = session_id;
    printf("release6 lock\n");
	  return session_id;
}

uv_stream_t* SessionMgr::GetSession(session_t session_id) {
  printf("wait for lock7");
  std::lock_guard<SessionMgr> lck(*this);
  printf("got7 lock\n");
  auto it = sessionMap_.find(session_id);
  if (it == sessionMap_.end()) {
     printf("connection doesn't exist\n");
     exit(1);
  }
  printf("release7 lock\n");
  return sessionMap_[session_id];
}

session_t SessionMgr::GetSessionID(uv_stream_t* stream) {
  printf("wait for lock8");
  std::lock_guard<SessionMgr> lck(*this);
printf("got8 lock\n");
  auto it = streamMap_.find(stream);
  if (it == streamMap_.end()) {
     printf("connection doesn't exist");
     exit(1);
  }
  printf("release8 lock\n");
  return streamMap_[stream];
}

int SessionMgr::RemoveSession(session_t session_id) {
  printf("wait for lock9");
  std::lock_guard<SessionMgr> lck(*this);
  printf("got9 lock\n");
	auto it = sessionMap_.find(session_id);
	if (it == sessionMap_.end())
	  return -1;
	sessionMap_.erase(it);
  printf("release9 lock\n");
	  return 0;
}

int SessionMgr::SendMessage(session_t session_id, CloudMessage& msg) {
  printf("wait for lock10");
  std::lock_guard<SessionMgr> lck(*this);
  printf("got10 lock\n");
  auto it = sessionMap_.find(session_id);
  if (it == sessionMap_.end())
    return -1;
  TcpHandle::SendMessage(it->second, msg);
  printf("release10 lock\n");
    return 0;
}

} //namespace cloud