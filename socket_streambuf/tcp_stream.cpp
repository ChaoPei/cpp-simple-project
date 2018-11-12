#include "tcp_stream.h"

#include <unistd.h>
#include <cassert>
#include <cstddef>

#include <sys/socket.h>

tcpstream::TcpStreamBuf::TcpStreamBuf(int socket, size_t bufSize) : m_bufSize(bufSize), m_socket(socket) {
    assert(bufSize > 0);
    m_gbuf = new char[bufSize];
    m_pbuf = new char[bufSize];

    std::streambuf::setp(m_pbuf, m_pbuf + bufSize);   // set points for output buffer
    std::streambuf::setg(m_gbuf, m_gbuf, m_gbuf);     // set points for input buffer
}

tcpstream::TcpStreamBuf::~TcpStreamBuf() {
    if (m_pbuf != nullptr) {
        delete m_pbuf;
        m_pbuf == nullptr;
    }

    if (m_gbuf != nullptr) {
        delete m_gbuf;
        m_gbuf == nullptr;
    }
}

int tcpstream::TcpStreamBuf::sync() {
    int sent = 0;
    int total = std::streambuf::pptr() - std::streambuf::pbase();
    while(sent < total) {
        // tcp socket send
        int ret = send(m_socket, std::streambuf::pbase() + sent, total - sent, 0);
        if (ret > 0) {
            sent += ret;
        } 
        else {
            return -1;
        }
    }
    // reset the points of output buffer 
    std::streambuf::setp(std::streambuf::pbase(), std::streambuf::pbase() + m_bufSize);
    // reset the pptr of streambuf
    std::streambuf::pbump(0);

    return 0;
}

int tcpstream::TcpStreamBuf::overflow(int c) {
    if (-1 == sync()) {
        return traits_type::eof();
    }
    else {
        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            std::streambuf::sputc(traits_type::to_char_type(c));
        }

        return traits_type::not_eof(c);
    }
}

int tcpstream::TcpStreamBuf::underflow() {
    // receive data from tcp socket
    int ret = recv(m_socket, std::streambuf::eback(), m_bufSize, 0);
    if (ret > 0) {
        // reset the points for input buffer
        std::streambuf::setg(std::streambuf::eback(), std::streambuf::eback(), std::streambuf::eback() + ret);
        return traits_type::to_int_type(*std::streambuf::gptr());
    }
    else {
        return traits_type::eof();
    }
    return 1;
}

tcpstream::TcpStream::TcpStream(int socket, size_t bufSize) : 
    std::iostream(new TcpStreamBuf(socket, bufSize)), m_socket(socket), m_bufSize(bufSize) {}

tcpstream::TcpStream::~TcpStream() {
    delete std::iostream::rdbuf();
}



