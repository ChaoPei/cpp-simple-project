// 
// server_http.cpp
// web server
//

#ifndef _SERVER_HTTP_HPP
#define _SERVER_HTTP_HPP


#include "server_base.hpp"
#include "handler.hpp"

namespace web_server {
	
    typedef boost::asio::ip::tcp::socket HTTP;
	
	// 模板特化
	// Server类是模板类, 将其特化为Server<HTTP>类, 继承了ServerBase类
	// 特化的是Server不是ServerBase
    template<>
	class Server<HTTP>: public ServerBase<HTTP> {
	public:
		Server(unsigned short port, size_t num_threads=1) :
			ServerBase<HTTP>::ServerBase(port, num_threads) {}

	private:
		// 实现acceptor方法
		void accept() {
                // 为当前连接创建一个socket
                auto socket = std::make_shared<HTTP>(m_io_service);
                acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
                        accept();
                        if(!ec)
                            process_request_and_respond(socket);
                    });
        }
	};

}

#endif 

