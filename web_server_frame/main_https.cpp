
//
// main_https.cpp
// web_server main 
//

#include "server_base.hpp"
#include "server_https.hpp"

using namespace web_server;

int main() {
	Server<HTTPS> https_server(1234, 4, "server.crt", "server.key");
	start_server<Server<HTTPS> >(https_server);

	return 0;
}




