#include <string>
#include "cloudmessage.h"

namespace cloud {
CloudMessage::CloudMessage() : count_(new std::size_t(0)) {

}

CloudMessage::CloudMessage(const char* str) : count_(new std::size_t(1)) {
    document_.SetObject();
    size_t len = strlen(str)+1;
    data_ = new char[len];
    ::memset(data_, '0', len);
    ::memcpy(data_, str, len);
}

CloudMessage::CloudMessage(const CloudMessage& msg)
    : data_(msg.GetData()), count_(msg.GetCount()), data_len_(msg.GetDataLen())
{
    AddCount();
}

CloudMessage::CloudMessage(CloudMessage&& msg) noexcept
    : data_(msg.GetData()), count_(msg.GetCount()), data_len_(msg.GetDataLen())
{
    //移动源对象会在移动后被销毁，所以必须加AddCount
    AddCount();
    // msg.GetData() = nullptr;
    // msg.GetCount() = nullptr;
}

CloudMessage& CloudMessage::operator=(CloudMessage&& msg) noexcept 
{
    if (this != &msg) {
        RemoveCount();
        data_ = msg.GetData();
        data_len_ = msg.GetDataLen();
        count_ = msg.GetCount();
        AddCount();
        // msg.GetData() = nullptr;
        // msg.GetCount() = nullptr;
    }
    return *this;
}

CloudMessage& CloudMessage::operator=(CloudMessage& msg) {
    msg.AddCount();
    RemoveCount();
    data_ = msg.GetData();
    data_len_ = msg.GetDataLen();
    count_ = msg.GetCount();
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

void CloudMessage::SetData(const char* data, size_t data_len) {
    data_len_ = data_len + 1;
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
    --*count_;
    if(*count_ == 0) {
        if (nullptr != data_)
            delete[] data_;
        if (nullptr != count_)
            delete count_; 
    }
} 
}

// error: passing ‘const’ as ‘this’ argument of  discards qualifiers [-fpermissive]