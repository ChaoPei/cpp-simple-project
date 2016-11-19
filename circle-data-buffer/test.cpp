#include "CircleDataBuffer.hpp"
#include <iostream>
#include <thread>
#include <unistd.h>     // sleep、usleep

using namespace std;

void print_buff(CircleDataBuffer* bufptr) {
    cout << "----------------------" << endl;
    cout << "Free size: " << bufptr->getFreeSize() << endl;
    cout << "Read cursor: " << bufptr->getReadCursor() << endl;
    cout << "Write cursor: " << bufptr->getWriteCursor() << endl;
    cout << "----------------------" << endl;
}


int main() {
    size_t bufsize = 1024*1000;
    
    unsigned char *dst = (unsigned char*)malloc(bufsize);
    if(!dst) {
        cout << "malloc dst error!" << endl;
        return -1;
    }
    
    unsigned char *src = (unsigned char*)malloc(bufsize);
    if(!src) {
        cout << "malloc src error!" << endl;
        return -1;
    }

    for(int i = 0; i < bufsize; ++i) {
        *(src+i) = '*';
    }


    CircleDataBuffer* cdb_ptr = new CircleDataBuffer(bufsize);
    
    // print_buff(cdb_ptr);
    // cdb_ptr->writeBuff(src, 512);
    // print_buff(cdb_ptr);
    // cdb_ptr->readBuff(dst, 128);
    // print_buff(cdb_ptr);

    // cdb_ptr->writeBuff(dst, 600);
    // print_buff(cdb_ptr);
    
    cdb_ptr->reset();
    print_buff(cdb_ptr);


    thread wt(mem_fn(&CircleDataBuffer::writeBuff), cdb_ptr, src, 512*1000);
    usleep(1);
    print_buff(cdb_ptr);
    
    thread rt(mem_fn(&CircleDataBuffer::readBuff), cdb_ptr, dst, 480*1000);
    usleep(1);
    
    print_buff(cdb_ptr);
    
    wt.join();
    rt.join();

    print_buff(cdb_ptr);
    
    cout << "\ntest complete" << endl;

    return 0;

}

