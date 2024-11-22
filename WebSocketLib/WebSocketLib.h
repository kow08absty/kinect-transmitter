#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


namespace WebSocketLib {
	typedef websocketpp::server<websocketpp::config::asio> server;
	using websocketpp::lib::placeholders::_1;
	using websocketpp::lib::placeholders::_2;
	using websocketpp::lib::bind;

	class Server {
	private:
		server websock_server;
		std::vector<websocketpp::connection_hdl> connections;

		static void OnOpen(std::vector<websocketpp::connection_hdl>* connections, websocketpp::connection_hdl hdl);

	public:
		void BroadCastMessage(std::string message);
		void BroadCastMessage(char* message);
		void BroadCastMessage(const char* message);

		void Init(int listen_port = 50008);

		void Stop();
	};
}