#ifndef CLOUDMESSAGE_H
#define CLOUDMESSAGE_H

#include <string.h>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace cloud {
class CloudMessage {
public:
   enum {
      Subscribe,
      Publish,
      Request
   };
   CloudMessage();

   CloudMessage(const char* str);

   CloudMessage(CloudMessage&);

   CloudMessage& operator=(CloudMessage&);

   ~CloudMessage();

   const char* GetOptionStr(const char* key);
   int GetOptionInt(const char* key);

   void AddOption(const char* key, const char* val);
   void AddOption(const char* key, int val);

   void Encode(const char* json);

   std::string Decode();

   void SetData(const char* data, size_t data_len);

   char* GetData();

   std::size_t* GetCount();

   std::size_t GetDataLen();

   void AddCount();

   void RemoveCount();

private: 
   char *data_ = nullptr;
   std::size_t *count_ = 0;
   std::size_t data_len_ = 0;
   rapidjson::Document document_;
};
}
#endif // CLOUDMESSAGE_H
