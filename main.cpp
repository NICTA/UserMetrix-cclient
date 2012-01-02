#include <iostream>
#include "UserMetrix.h"


int main (int argc, char * const argv[]) {
    // insert code here...
	UM_setCanSendLogs(UM_true);
	UM_init(1, "");
	UM_event("test", __FILE__);
//	UM_error("error", __FILE__);
    std::cout << "Hello, World!\n";
	
	UM_shutdown();
    return 0;
}