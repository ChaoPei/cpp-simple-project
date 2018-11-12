#ifndef _TCP_STREAM_
#define _TCP_STREAM_

#include <iostream>

namespace tcpstream {
    
    /**
     * Simple stream buffer for TCP socket
     */ 
    class TcpStreamBuf : public std::streambuf {
    public:
        TcpStreamBuf(int socket, size_t bufSize);
        ~TcpStreamBuf();

        // auto get input from external device when buffer is empty, and then get a char and transform to int from buffer.
        int underflow();
        // auto flush buffer when buffer area is filled, and then put c to buffer.
        int overflow(int c);
        // flush buffer to external device
        int sync();

    private:
        const size_t m_bufSize;     // buffer size
        int m_socket;   //  tcp socket
        char* m_pbuf;     // output stream buffer
        char* m_gbuf;     // input stream buffer
    };

    /**
     * Simple stream for TCP socket
     */
    class TcpStream : public std::iostream {
    public:
        TcpStream(int socket, size_t bufSize);
        ~TcpStream();
    
    private:
        int m_socket;
        const size_t m_bufSize;
    };
}

#endif  // _TCP_STREAM_
