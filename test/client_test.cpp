#include "cloudmessage.h"
#include "cloudclient.h"
#include "cloud.h"
#include "loop.h"

using namespace cloud;

int main() {
	Loop loop;
	CloudClient client;
	client.SetLoop(loop);
	struct sockaddr_in addr;
    ASSERT(0 == uv_ip4_addr("0.0.0.0", 9123, &addr));  //set server target
    client.Connect(&addr);
    loop.loop_run(Loop::RUN_DEFAULT);
}