//
// main_http.cpp
// web_server main 
//

#include "server_base.hpp"
#include "server_http.hpp"

using namespace web_server;

int main() {
	Server<HTTP> http_server(1234, 4);
	start_server<Server<HTTP> >(http_server);

	return 0;
}




