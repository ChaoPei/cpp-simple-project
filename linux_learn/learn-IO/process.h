#ifndef _PROCESS_H

#define _PROCESS_H

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <signal.h>



// 用于内存管理
static struct iovec *vs=NULL, *vc=NULL;

// socket process
void process_conn_client(int s);
void process_conn_server(int s);


// signal process
void sig_process(int signo);
void sig_pipe(int signo);

#endif  /*  _PROCESS_H */
