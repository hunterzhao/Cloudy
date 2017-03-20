#ifndef CLOUD_H
#define CLOUD_H

#include <memory>
#include <stdio.h>
namespace cloud {
#define CLOUD_VERSION 0.0             
 
#define ASSERT(expr)                                      \
 do {                                                     \
  if (!(expr)) {                                          \
    fprintf(stderr,                                       \
            "Assertion failed in %s on line %d: %s\n",    \
            __FILE__,                                     \
            __LINE__,                                     \
            #expr);                                       \
    abort();                                              \
  }                                                       \
 } while (0)


} //namespace flow
#endif //FLOW_H