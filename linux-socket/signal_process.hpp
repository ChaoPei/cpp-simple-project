#ifndef _SIGNAL_PROCESS_HPP

#define _SIGNAL_PROCESS_HPP

#include <iostream>
#include <stdlib.h>

void sig_process(int signo) {
    std::cout << "Catch a SIGINT signal" << std::endl;
    _exit(0);
}

void sig_pipe(int signo) {
    std::cout << "Catch a SIGPIPE signal" << std::endl;

    // TODO: release pipe resource

    _exit(0);
}

#endif // _SIGNAL_PROCESS_HPP