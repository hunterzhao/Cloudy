#include <string>
#include <stdio.h>
#include "cloudmessage.h"

namespace cloud {
CloudMessage::CloudMessage() : count_(new std::size_t(1)) {
    document_.SetObject();
}

CloudMessage::CloudMessage(const char* str) : count_(new std::size_t(1)) {
    document_.SetObject();
    size_t len = strlen(str)+1;
    data_ = new char[len];
    ::memset(data_, '0', len);
    ::memcpy(data_, str, len);
}

CloudMessage::CloudMessage(CloudMessage& msg)
    : data_(msg.data_), count_(msg.count_), data_len_(msg.data_len_)
{ 
    AddCount();
    document_.CopyFrom(msg.document_, document_.GetAllocator());
}

CloudMessage::CloudMessage(CloudMessage&& msg) noexcept
    : data_(msg.data_), count_(msg.count_), data_len_(msg.data_len_)
{
    //移动源对象会在移动后被销毁，所以必须加AddCount
    AddCount();
    document_.SetObject();
    document_.CopyFrom(msg.document_, document_.GetAllocator());

    msg.data_ = nullptr;
    msg.count_ = nullptr;
}

CloudMessage& CloudMessage::operator=(CloudMessage&& msg) noexcept 
{
    if (this != &msg) {
        RemoveCount();
        data_ = msg.GetData();
        data_len_ = msg.GetDataLen();
        count_ = msg.GetCount();
        document_.SetObject();
        document_.CopyFrom(msg.document_, document_.GetAllocator());
        AddCount();
        msg.data_ = nullptr;
        msg.count_ = nullptr;
    }
    return *this;
}

CloudMessage& CloudMessage::operator=(CloudMessage& msg) {
    msg.AddCount();
    RemoveCount();
    data_ = msg.GetData();
    data_len_ = msg.GetDataLen();
    count_ = msg.GetCount();
    document_.SetObject();
    document_.CopyFrom(msg.document_, document_.GetAllocator());
    assert(document_.IsObject());
    return *this;
}

CloudMessage::~CloudMessage() {
    RemoveCount();
}

const char* CloudMessage::GetOptionStr(const char* key) {
    return document_[key].GetString();
}

int CloudMessage::GetOptionInt(const char* key) {
    return document_[key].GetInt();
}

void CloudMessage::AddOption(const char* key, const char* val) {
    document_.AddMember(rapidjson::Value(key, document_.GetAllocator()),
    rapidjson::Value(val, document_.GetAllocator()), document_.GetAllocator());
}

void CloudMessage::AddOption(const char* key, int val) {
    document_.AddMember(rapidjson::Value(key, document_.GetAllocator()),
    rapidjson::Value(val), document_.GetAllocator());
}

void CloudMessage::Encode(const char* json) {
    if (document_.Parse(json).HasParseError()) {
       std::cout << "encode error" << std::endl;
    }
}

std::string CloudMessage::Decode() {
    rapidjson::StringBuffer decode;
    rapidjson::Writer<rapidjson::StringBuffer> writer(decode);
    document_.Accept(writer);
    return decode.GetString();
}

void CloudMessage::SetData(const char* data) {
    data_len_ = strlen(data) + 1;
    if (nullptr != data_ )
       delete data_;

    data_ = new char[data_len_];
    ::memset(data_, 0, data_len_);
    ::memcpy(data_, data, data_len_);
    *count_ = 1;
}

char* CloudMessage::GetData() const {
    return data_;
}

std::size_t* CloudMessage::GetCount() const {
    return count_;
}

std::size_t CloudMessage::GetDataLen() const {
    return data_len_;
}

void CloudMessage::AddCount() {
    ++*count_;
}

void CloudMessage::RemoveCount() {
    if(nullptr == count_)
        return;
    --*count_;
    if(*count_ <= 0) {
        if (nullptr != data_) {
            delete[] data_;
            data_ = nullptr;
        }
       
        delete count_; 
        count_ = nullptr;
        
    }
} 
}

// error: passing ‘const’ as ‘this’ argument of  discards qualifiers [-fpermissive]