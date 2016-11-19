//
// handler.cpp
// web server
//

#include "server_base.hpp"
#include <fstream>

using namespace std;
using namespace web_server;

template<typename SERVER_TYPE>
void start_server(SERVER_TYPE &server) {

	/**
	 * 注册处理函数
	 * 处理访问/string的POST请求, 返回POST字符串
	 */
	server.resource["^/string/?$"]["POST"] = [](ostream& response, Request& request) {
		// 从istream中获取字符串
		stringstream ss;
		*(request.content) >> ss.rdbuf();
		string content = ss.str();

		response << "HTTP/1.1 200 OK\r\nContent-Length: " 
			<< content.size() << "\r\n\r\n" << content;
	};


	/**
	 * 注册处理函数
	 * 处理访问/info的GET请求, 返回请求的信息
	 */
	server.resource["^/info/?$"]["GET"] = [](ostream& response, Request& request) {
		stringstream content_stream;
		
		// 通过request的信息构建response
		content_stream << "<h1>Request: </h1>";
		content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "<br>";

		for(auto& header: request.header) {
			content_stream << header.first << ": " << header.second << "<br>";
		}

		// 设置content_stream输出位置
		content_stream.seekp(0, ios::end);		// 全部
		
		// 获得content_stream的输出长度(使用content.tellp()获得)
		response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() 
			<< "\r\n\r\n" << content_stream.rdbuf();
	};

	
	/**
	 * 注册处理函数
	 * 处理访问/match/[字母+数字组成的字符串]的GET请求, 返回match后的字符串
	 */
	server.resource["^/match/([0-9a-zA-Z]+)/?$"]["GET"] = [](ostream& response, Request& request){
		string number = request.path_match[1];
		response << "HTTP/1.1 200 OK\r\nContent-Length: " << number.length() 
			<< "\r\n\r\n" << number;
	};



	/**
	 * 注册处理函数
	 * 处理默认GET请求, 如果其他匹配不成功, 则调用这个处理函数
	 * 应答是web目录下的index.html文件
	 */
	server.default_resource["^/?(.*)$"]["GET"] = [](ostream& response, Request& request){
		string filename = "web/";

		string path = request.path_match[1];

		// 防止使用".."来访问web/目录以外的内容
		size_t last_pos = path.rfind(".");		// 找到最后一个'.'
		size_t current_pos = 0;
		size_t pos;
		while((pos=path.find('.', current_pos)) != string::npos && pos != last_pos) {
			// 如果找到了'.'而且不是last_pos位置, 将其删除
			current_pos = pos;
			path.erase(pos, 1);
			last_pos--;
		}

		filename += path;
		ifstream ifs;

		// 简单的平台无关的文件或目录检查
		if(filename.find('.') == string::npos) {
			if(filename[filename.length() - 1] != '/')
				filename += '/';
			filename += "index.html";
		}
		
		ifs.open(filename, ifstream::in);

		if(ifs) {
			ifs.seekg(0, ios::end);
			size_t length = ifs.tellg();

			ifs.seekg(0, ios::beg);

			// 文件内容拷贝到response-stream中, 不应该用于大型文件
			response << "HTTP/1.1 200 OK\r\nContent-Length: " << length 
				<< "\r\n\r\n" << ifs.rdbuf();

			ifs.close();
		}
		else {
			string content = "Could not open file" + filename;
			response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.size() << "\r\n\r\n" << content;
		}
	};


	// 运行服务器
	server.start();
}

