//
// Created by fabian on 04.07.16.
//

#ifndef HOURGLASS_LINUX_STATEHANDLER_H
#define HOURGLASS_LINUX_STATEHANDLER_H

#include <iostream>
#include <signal.h>
#include <string>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <elf.h>
#include <fstream>

class StateHandler {
public:
    StateHandler(pid_t targetProcess);
    bool SaveState(std::string path);

private:
    pid_t PID;
    bool dumpRegisters(user_regs_struct * Registers, int * error);
    void printError(int error);
};


#endif //HOURGLASS_LINUX_STATEHANDLER_H
