#include "cloudlog.h"

namespace cloud {

static CloudLog& fowlog = CloudLog::Instance();  //创建对象，返回引用
std::shared_ptr<spdlog::logger> CloudLog::cloud_log;  //声明

CloudLog::level_enum CloudLog::Trace    = spdlog::level::trace;
CloudLog::level_enum CloudLog::Debug    = spdlog::level::debug;
CloudLog::level_enum CloudLog::Info     = spdlog::level::info;
CloudLog::level_enum CloudLog::Warm     = spdlog::level::warn;
CloudLog::level_enum CloudLog::Err      = spdlog::level::err;
CloudLog::level_enum CloudLog::Critical = spdlog::level::critical;
CloudLog::level_enum CloudLog::Off      = spdlog::level::off;

}