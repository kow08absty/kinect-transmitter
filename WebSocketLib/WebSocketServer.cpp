// WebSocketServer.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "WebSocketLib.h"

void WebSocketLib::Server::OnOpen(std::vector<websocketpp::connection_hdl>* connections, websocketpp::connection_hdl hdl) {
	connections->push_back(hdl);
}

void WebSocketLib::Server::BroadCastMessage(std::string message) {
	for (auto ite = connections.begin(); ite != connections.end(); ++ite) {
		websock_server.send(*ite, message, websocketpp::frame::opcode::TEXT);
	}
}

void WebSocketLib::Server::BroadCastMessage(char* message) {
	std::string str = message;
	this->BroadCastMessage(str);
}

void WebSocketLib::Server::BroadCastMessage(const char* message) {
	std::string str = message;
	this->BroadCastMessage(str);
}

void WebSocketLib::Server::Init(int listen_port) {
	try {
		websock_server.set_access_channels(websocketpp::log::alevel::all);
		websock_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

		printf("-> server init\n");
		websock_server.init_asio();
		websock_server.set_open_handler(bind(&OnOpen, &connections, _1));
		printf("-> listen :%d\n", listen_port);
		websock_server.listen(listen_port);
		websock_server.start_accept();

		printf("\n::: start server :: : \n\n");
		websock_server.run();
	} catch (websocketpp::exception const& e) {
		std::cout << "Failed to run server because: " << e.what() << std::endl;
	}
}

void WebSocketLib::Server::Stop() {
	websock_server.stop();
}
