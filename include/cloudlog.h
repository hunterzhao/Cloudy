#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <memory>
#include "spdlog/spdlog.h"
#include "singleton.h"

namespace cloud {
class CloudLog : public Singleton<CloudLog> {
public:
	friend class Singleton<CloudLog>;
    typedef spdlog::level::level_enum level_enum ;
    
    static level_enum Trace;
    static level_enum Debug;
    static level_enum Info;
    static level_enum Warm;
    static level_enum Err;
    static level_enum Critical;
    static level_enum Off;

    static void SetLevel(level_enum lev) {
    	spdlog::set_level(lev);
    }
   
    ~CloudLog() {
    	spdlog::drop_all();
    }
     
    static std::shared_ptr<spdlog::logger> cloud_log;

private:
    CloudLog() {
        cloud_log = spdlog::stdout_color_mt("console");
        //fow_log = spdlog::daily_logger_mt("app", "../log", 0, 0);
    }
};

} // end of namespace

#define LOG cloud::CloudLog::cloud_log
#define LOG_TRY try
#define LOG_CATCH catch (spdlog::spdlog_ex& ex) {      \
    std::cout << "Log failed: " << ex.what() << std::endl;}   

#endif //LOG_H