#pragma once
#include <map>
#include "Session.h"
class TcpServer
{
public:
	TcpServer(boost::asio::io_context& ioc, short port);
	~TcpServer();
	//����ϵ�������
	void ClearSession(std::string uuid);

	//��������
	void SendMsg(std::string uuid, std::string msg);
private:
	//��ʼ��������
	void Start_accept();
	//���ӽ����Ļص�
	void handle_accept(std::shared_ptr<Session>,
		const boost::system::error_code& error);
private:
	boost::asio::io_context& m_ioc;
	tcp::acceptor m_acceptor;
	std::map<std::string, std::shared_ptr<Session>> m_sessions;
};

