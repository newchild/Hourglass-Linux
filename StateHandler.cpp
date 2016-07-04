//
// Created by fabian on 04.07.16.
//


#include <wait.h>
#include "StateHandler.h"

StateHandler::StateHandler(pid_t targetProcess) {
    PID = targetProcess;
}

bool StateHandler::SaveState(std::string path) {
    user_regs_struct registers;
    int error = 0;
    if(!dumpRegisters(&registers, &error)){
        std::cerr << "Failed to dump Registers, finding solution..." << std::endl;
        printError(error);
        return false;
    }
    std::ofstream saveFile;
    saveFile.open(path);
    saveFile << "Rax:" << std::hex << registers.rax << "\n";
    saveFile << "CS:" << std::hex << registers.cs << "\n";
    saveFile << "DS:" << std::hex << registers.ds << "\n";
    saveFile << "eflags:" << std::hex << registers.eflags << "\n";
    saveFile << "ES:" << std::hex << registers.es << "\n";
    saveFile << "FS:" << std::hex << registers.fs << "\n";
    saveFile << "FS_Base:" << std::hex << registers.fs_base << "\n";
    saveFile.close();
    return true;
}

bool StateHandler::dumpRegisters(user_regs_struct * Registers, int * error) {

    if(ptrace(PTRACE_ATTACH, PID, 0, 0) == -1){
        *error = errno;
        return false;
    }
    waitpid(PID, 0, 0);
    if(ptrace(PTRACE_GETREGS, PID, 0, Registers) == -1){
        if(kill(PID, SIGCONT) != 0){
            std::cerr << "Critical Error while trying to resume the tracee!" << std::endl;
        } // Continue continuation of the program at all cost (Except when it fails (???))
        *error = errno;
        return false;
    }

    if(ptrace(PTRACE_DETACH, PID, 0, 0) == -1){
        if(kill(PID, SIGCONT) != 0){
            std::cerr << "Critical Error while trying to resume the tracee!" << std::endl;
        } // Continue continuation of the program at all cost (Except when it fails (???))
        *error = errno;
        return false;
    }
    return true;
}

void StateHandler::printError(int error) {
    switch(error){
        case EBUSY:
            std::cout << "Unable to allocate/free debug register" << std::endl;
            break;
        case EFAULT:
            std::cout << "Attempted to read/write inavlid area!" << std::endl;
            break;
        case EIO:
            std::cout << "Invalid request was made, contact author" << std::endl;
            break;
        case EPERM:
            std::cout << "Insufficient Permissions" << std::endl;
            break;
        case ESRCH:
            std::cout << "Process does not exist, is not being traced, or is not stopped" << std::endl;
            break;

    }
}





