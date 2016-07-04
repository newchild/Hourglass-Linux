
#include <zconf.h>
#include "StateHandler.h"

bool isRoot();

int main() {

    if(!isRoot()){
        std::cout << "Please run Hourglass as root!" << std::endl;
        return 1;
    }
    int pid = 0;
    std::cout << "Please enter the PID of the target" << std::endl;
    std::cin >> pid;
    StateHandler t(pid);
    if(!t.SaveState("registers.txt")){
        std::cerr << "Critical error while Saving current state" << std::endl;
        return 2;
    }
    return 0;
}

bool isRoot(){
    return geteuid() == 0;
}