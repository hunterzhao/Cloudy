#ifndef CLOUDMESSAGE_H
#define CLOUDMESSAGE_H

#include <string.h>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

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
   
   CloudMessage(CloudMessage&& msg) noexcept;
   
   CloudMessage& operator=(CloudMessage&& msg) noexcept; 

   CloudMessage& operator=(CloudMessage&);

   ~CloudMessage();

   const char* GetOptionStr(const char* key);
   int GetOptionInt(const char* key);

   void AddOption(const char* key, const char* val);
   void AddOption(const char* key, int val);

   void Encode(const char* json);

   std::string Decode();

   void SetData(const char* data);

   char* GetData() const;

   std::size_t* GetCount() const;

   std::size_t GetDataLen() const;

   void AddCount();

   void RemoveCount();

   rapidjson::Document document_;
//private: 
   char *data_ = nullptr;
   std::size_t *count_ = nullptr;
   std::size_t data_len_ = 0;
   
};
}
#endif // CLOUDMESSAGE_H
