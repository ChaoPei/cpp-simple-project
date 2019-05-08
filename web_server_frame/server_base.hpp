/**
 * server_base.hpp
 * web server
 * 
 */

#ifndef _SERVER_BASE_HPP
#define _SERVER_BASE_HPP

#include <boost/asio.hpp>

#include <regex>
#include <thread>
#include <unordered_map>

namespace web_server {
    
    // 定义请求的结构
    struct Request {
        // 请求方法、路径、HTTP版本
        std::string method, path, http_version;

        // 请求内容
        std::shared_ptr<std::istream> content;

        // 请求头部
        std::unordered_map<std::string, std::string> header;

        // 路径匹配结果
        std::smatch path_match;
    };


    // 资源类型
    typedef std::map<std::string, std::unordered_map<std::string,
            std::function<void(std::ostream&, Request&)> > > resource_type;


    //socket_type为HTTP或者HTTPS
    template <typename socket_type>
    class ServerBase {
    public:
        // 拥有的资源和默认资源
        resource_type resource;
        resource_type default_resource;

        // 构造服务器
        ServerBase(unsigned short port, size_t num_threads = 1) :
            endpoint(boost::asio::ip::tcp::v4(), port),
            acceptor(m_io_service, endpoint),
            num_threads(num_threads) {}
        
        // 启动服务器
        void start();

    protected:
        /**
         * io_service: asio通过io_service来调度
         * endponit:   构成TCP通信的一个端点, 包括IP、端口、协议版本等信息
         * acceptor:   监听器, 在指定端口上等待连接
         */
        boost::asio::io_service m_io_service;
        boost::asio::ip::tcp::endpoint endpoint;
        boost::asio::ip::tcp::acceptor acceptor;

        // 服务器线程池
        size_t num_threads;
        std::vector<std::thread> threads;
        
        // 资源存储
        std::vector<resource_type::iterator> all_resources;


        // 不同服务器实现不同的accept, 建立不同的连接
        virtual void accept() {}

        // 处理请求和应答
        void process_request_and_respond(std::shared_ptr<socket_type> socket) const;
        
        // 解析request信息
        Request parse_request(std::istream& stream) const;
        
        // 应答请求: 建立连接、找到处理方法、生成response
        void respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const;
        
    };


    template <typename socket_type>
    class Server: public ServerBase<socket_type> {};

}


// 分离编译
#include "server_base.cpp"


#endif /* _SERVER_BASE_HPP */

