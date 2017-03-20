#include <iostream>
#include "loop.h"

namespace cloud {
Loop::Loop() {
	if(loop_) {
	   uv_loop_init(loop_);
    } else {
    	loop_ = uv_default_loop();
    }
} 

Loop::~Loop() {
	int ret = uv_loop_close(loop_);
    printf("happy ending of loop\n");
	// if (ret & UV_EBUSY) {
 //        printf("loop is busy executing\n");
	// }
}

uv_loop_t* Loop::self() {
    return loop_;
}

int Loop::loop_run(loop_mode mode) {
    switch(mode) {
    	case 0: {
    	   int ret = uv_run(loop_, UV_RUN_DEFAULT);
    	   if (ret != 0)
                ;// LOG->error("loop stop still exist active request or handle");
    	   break;
    	}
    	case 1: {
    	   int ret = uv_run(loop_, UV_RUN_ONCE);
    	   if (ret == 0)
                ; // LOG->info("loop stop and no request or handle");
    	   else
                ;// LOG->error("more callback are expectd, u should run the loop again");
    	   break;
    	}
    	case 2: {
    	   int ret = uv_run(loop_, UV_RUN_NOWAIT);
    	   if (ret == 0) 
                ;// LOG->info("loop stop and no request or handle");
    	   else
    	   		;// LOG->info("more callback are expectd, u should run the loop again");
    	   break;
    	}
    	default: {

        }
    	   // LOG(error) << "wrong loop mode";
    }
    return 0;
}

int Loop::loop_alive() {
    return uv_loop_alive(loop_);
}

void Loop::loop_stop() {
	uv_stop(loop_);
}	
}