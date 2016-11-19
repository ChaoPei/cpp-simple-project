//
// server_base.cpp
// web server
//

namespace web_server {
    
    /**
     * 启动时将所有的资源添加到资源池vector
     * 默认资源放在vector末尾
     *
     * 请求时首先处理能够找到的匹配路径, 使用这些匹配路径的资源来处理请求
     * 当请求找不到匹配的路径时, 使用默认资源来应答
     */
    template <typename socket_type>
    void ServerBase<socket_type>::start() {
        
        std::cout << "\n***Start Web Server..." << std::endl;

        for(auto it = resource.begin(); it != resource.end(); ++it) {
            all_resources.push_back(it);
        }

        for(auto it = default_resource.begin(); it != default_resource.end(); ++it) {
            all_resources.push_back(it);
        }

        // 调用socket的连接方式, 启动子类的accept()
        accept();
        
        for(size_t c = 1; c < num_threads; ++c){
            // emplace_back用作类对象提高效率
            // lambda表达式
            // 以m_io_service.run()开启一个新线程
            std::cout << "start a new thread, thread nums: " << c << std::endl;
            threads.emplace_back([this](){ m_io_service.run();});
        }

        // 主线程
        m_io_service.run();

        // 等待其他线程结束
        for(auto &t:threads)
            t.join();
    }



    /**
     * 处理请求和应答
     * 请求形式socket为封装的HTTP或者HTTPS
     *
     * 取出socket中的内容, 保存到stream中, 然后解析stream, 将信息保存到Request中
     */
    template <typename socket_type>
    void ServerBase<socket_type>::process_request_and_respond(std::shared_ptr<socket_type> socket) const{
        
        // 创建读缓存
        // read_buff被推导为一个std::shared_ptr<>
        auto read_buff = std::make_shared<boost::asio::streambuf>();
        
        
        // 读取数据
        // lambda匿名函数在读取完成之后被调用, 用作数据处理
        boost::asio::async_read_until(*socket, *read_buff, "\r\n\r\n", 
                
            [this, socket, read_buff](const boost::system::error_code &ec, size_t bytes_transferred){
            
                if(!ec) {
                
                    size_t total = read_buff->size();
                
                    // 转换到istream
                    std::istream stream(read_buff.get());

                    // 解析stream, 保存到request
                    auto request = std::make_shared<Request>();
                    *request = parse_request(stream);
    
                    size_t num_additional_bytes = total - bytes_transferred;
                    

                    // 如果有Content-Length字段
                    if(request->header.count("Content-Length") > 0) {
                        boost::asio::async_read(*socket, *read_buff,
                            // str to unsigned long long, 将字符串解析为整数 
                            boost::asio::transfer_exactly(std::stoull(request->header["Content-Length"]) - num_additional_bytes),
                            // handler function
                            [this, socket, read_buff, request](const boost::system::error_code& ec, size_t bytes_tranferred) {
                                
                            if(!ec) {
                                // 将指针作为istream 对象存储到read_buff中
                                request->content = std::shared_ptr<std::istream>(new std::istream(read_buff.get()));
                                respond(socket, request);
                                }
                            }
                        );
                    }
                    else {
                        respond(socket, request);
                    }
                }
            }
        );
    }




    /**
     * 解析stream信息, 保存到Request对象中
     *
     */
    template <typename socket_type>
    Request ServerBase<socket_type>::parse_request(std::istream& stream) const {
        Request request;

        // 使用正则表达式解析表头
        // 获取请求方法(GET/POST)、请求路径以及版本
        std::regex reg_exp("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
        std::smatch sub_match;

        // 从表头第一行解析方法、路径和版本
        std::string line;
        getline(stream, line);
        line.pop_back();        // 去掉换行符
        if(std::regex_match(line, sub_match, reg_exp)) {
            request.method         = sub_match[1];
            request.path         = sub_match[2];
            request.http_version = sub_match[3];

            std::cout << "Request info: method: " << request.method << "  path: " << request.path << " http version: " << request.http_version;
            
            // 继续解析头部其他信息并存入request的header中
            bool matched;
            reg_exp = "^([^:]*): ?(.*)$";
            do {
                getline(stream, line);
                line.pop_back();
                matched = std::regex_match(line, sub_match, reg_exp);
                if(matched) {
                    request.header[sub_match[1]] = sub_match[2];
                }
            }
            while(matched == true);
        }

        return request;
    }



    /**
     * 应答请求
     */
    template <typename socket_type>
    void ServerBase<socket_type>::respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const {
        //
        // 对请求路径进行匹配查找, 然后生成响应
        for(auto res_it: all_resources) {
            std::regex reg_exp(res_it->first);
            std::smatch sm_res;
            // 查找匹配路径
            if(std::regex_match(request->path, sm_res, reg_exp)) {
                // 查找对应请求方法
                if(res_it->second.count(request->method) > 0) {
                    request->path_match = move(sm_res);
                    
                    // 调用处理函数生成response
                    auto write_buff = std::make_shared<boost::asio::streambuf>();
                    std::ostream response(write_buff.get());
                    res_it->second[request->method](response, *request);

                    boost::asio::async_write(*socket, *write_buff,
                        [this, socket, request, write_buff](const boost::system::error_code& ec, size_t bytes_transferred) {
                        
                        // HTTP1.1持久性连接
                        if(!ec && stof(request->http_version) > 1.05) {
                            process_request_and_respond(socket);
                            }
                        });

                    return;
                }
            }
        }
    }


}



