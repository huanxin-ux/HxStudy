#pragma once
#include <map>
#include "Session.h"
class TcpServer
{
public:
	TcpServer(boost::asio::io_context& ioc, short port);
	~TcpServer();
	//清除断掉的连接
	void ClearSession(std::string uuid);

	//发送数据
	void SendMsg(std::string uuid, std::string msg);
private:
	//开始接收连接
	void Start_accept();
	//连接建立的回调
	void handle_accept(std::shared_ptr<Session>,
		const boost::system::error_code& error);
private:
	boost::asio::io_context& m_ioc;
	tcp::acceptor m_acceptor;
	std::map<std::string, std::shared_ptr<Session>> m_sessions;
};

