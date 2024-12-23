/**
 * @file WebSocketLib.h
 * @brief websocketpp を簡単に扱えるようにしたラッパークラスの定義
 * @author Kohei OKUMURA
 * @date 2024/11/22
 */

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


namespace WebSocketLib {
	typedef websocketpp::server<websocketpp::config::asio> server;
	using websocketpp::lib::placeholders::_1;
	using websocketpp::lib::placeholders::_2;
	using websocketpp::lib::bind;

	/// <summary>
	/// サーバ用
	/// </summary>
	class Server {
	private:
		server websock_server;
		std::vector<websocketpp::connection_hdl> connections;
		std::thread service;

		static void OnOpen(std::vector<websocketpp::connection_hdl>* connections, websocketpp::connection_hdl hdl);

	public:
		/// <summary>
		/// 現在接続しているすべてのクライアントに向けてメッセージを送信
		/// </summary>
		/// <param name="message">メッセージ</param>
		void BroadCastMessage(std::string message);
		void BroadCastMessage(char* message);
		void BroadCastMessage(const char* message);

		/// <summary>
		/// サーバを初期化してポートを占領する
		/// </summary>
		/// <param name="listen_port">待受ポート</param>
		void Run(int listen_port = 50008);

		/// <summary>
		/// サーバを止める
		/// </summary>
		void Stop();
	};
}