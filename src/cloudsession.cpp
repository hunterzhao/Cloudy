#include <mutex>
#include "cloudsession.h"
#include "cloudmessage.h"
#include "cloudtcphandle.h"

namespace cloud {
 session_t SessionMgr::AddNewSession(uv_stream_t* tcp) {
    std::lock_guard<SessionMgr> lck(*this);
    // auto it = sessionMap_.find(tcp);
    // if (it != sessionMap_.end()) return it->second;
    int session_id = maxNumber_++;
	  sessionMap_[session_id] = tcp;
    streamMap_[tcp] = session_id;
	  return session_id;
}

uv_stream_t* SessionMgr::GetSession(session_t session_id) {
  std::lock_guard<SessionMgr> lck(*this);
  auto it = sessionMap_.find(session_id);
  if (it == sessionMap_.end()) {
     printf("connection doesn't exist\n");
     exit(1);
  }
  return sessionMap_[session_id];
}

session_t SessionMgr::GetSessionID(uv_stream_t* stream) {
  std::lock_guard<SessionMgr> lck(*this);
  auto it = streamMap_.find(stream);
  if (it == streamMap_.end()) {
     printf("connection doesn't exist");
     exit(1);
  }
  return streamMap_[stream];
}

int SessionMgr::RemoveSession(session_t session_id) {
  std::lock_guard<SessionMgr> lck(*this);
	auto it = sessionMap_.find(session_id);
	if (it == sessionMap_.end())
	  return -1;
	sessionMap_.erase(it);
	  return 0;
}

int SessionMgr::SendMessage(session_t session_id, CloudMessage& msg) {
  std::lock_guard<SessionMgr> lck(*this);
  auto it = sessionMap_.find(session_id);
  if (it == sessionMap_.end())
    return -1;
  TcpHandle::SendMessage(it->second, msg);
    return 0;
}

} //namespace cloud