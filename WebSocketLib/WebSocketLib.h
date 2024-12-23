/**
 * @file WebSocketLib.h
 * @brief websocketpp ���ȒP�Ɉ�����悤�ɂ������b�p�[�N���X�̒�`
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
	/// �T�[�o�p
	/// </summary>
	class Server {
	private:
		server websock_server;
		std::vector<websocketpp::connection_hdl> connections;
		std::thread service;

		static void OnOpen(std::vector<websocketpp::connection_hdl>* connections, websocketpp::connection_hdl hdl);

	public:
		/// <summary>
		/// ���ݐڑ����Ă��邷�ׂẴN���C�A���g�Ɍ����ă��b�Z�[�W�𑗐M
		/// </summary>
		/// <param name="message">���b�Z�[�W</param>
		void BroadCastMessage(std::string message);
		void BroadCastMessage(char* message);
		void BroadCastMessage(const char* message);

		/// <summary>
		/// �T�[�o�����������ă|�[�g���̂���
		/// </summary>
		/// <param name="listen_port">�Ҏ�|�[�g</param>
		void Run(int listen_port = 50008);

		/// <summary>
		/// �T�[�o���~�߂�
		/// </summary>
		void Stop();
	};
}