#ifndef LOOP_H_
#define LOOP_H_

#include <uv.h>

namespace cloud {

class Loop {
public:

  typedef enum {
  	//Runs the event loop until there are no more active and referenced handles or requests. 
    RUN_DEFAULT = 0,

    //Poll for i/o once. Note that this function blocks if there are no pending callbacks. 
    RUN_ONCE,

    //Poll for i/o once but don’t block if there are no pending callbacks.
    RUN_NOWAIT
  } loop_mode;

  //Initializes uv_loop_t structure loop_.
  Loop();

  //Releases all internal loop resources.
  ~Loop();
  
  uv_loop_t* self();
  // runs the event loop. It will act differently depending on the specified mode:
  int loop_run(loop_mode mode);
  
  //Returns non-zero if there are active handles or request in the loop.
  int loop_alive();

  //Stop the event loop, causing uv_run() to end as soon as possible. 
  void loop_stop();

private:
  uv_loop_t *loop_ = nullptr;
};
} //namesapce flow
#endif // FLOW_FRAMEWORK_FLOW_LOOP_H