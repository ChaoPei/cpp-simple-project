//
// server_https.hpp
// web server
// 
// libssl-dev

#ifndef _SERVER_HTTPS_HPP
#define _SERVER_HTTPS_HPP

#include "server_http.hpp"
#include <boost/asio/ssl.hpp>

namespace web_server {

	typedef boost::asio::ssl::stream<HTTP> HTTPS;

	template<>
	class Server<HTTPS> : public ServerBase<HTTPS> {
	public:
		// https服务器多了证书文件和私钥文件
		Server(unsigned short port, size_t num_threads, const std::string &cert_file, const std::string& private_key_file):
			ServerBase<HTTPS>::ServerBase(port, num_threads), 
			context(boost::asio::ssl::context::sslv23) {
			
				// 使用证书文件
				context.use_certificate_chain_file(cert_file);
				// 使用私钥文件, 指定文件格式为pem
				context.use_private_key_file(private_key_file, boost::asio::ssl::context::pem);
			}


	private:
		// https需要定义一个ssl context对象
		boost::asio::ssl::context context;

		// https服务器对socket中IO流进行加密
		void accept() {

			auto socket = std::make_shared<HTTPS>(m_io_service, context);

			acceptor.async_accept(
				(*socket).lowest_layer(),
				[this, socket](const boost::system::error_code& ec) {
					// 立即启动并接收一个新的连接
					accept();

					// 处理socket
					if(!ec) {
						(*socket).async_handshake(boost::asio::ssl::stream_base::server,
								[this, socket](const boost::system::error_code& ec) {
									if(!ec)
										process_request_and_respond(socket);
								});
					}

				});
			}
	};

}


#endif  /* _SERVER_HTTPS_HPP */

