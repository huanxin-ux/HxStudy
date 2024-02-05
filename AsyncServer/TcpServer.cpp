#include "TcpServer.h"
#include <iostream>
#include "IoContextPool.h"
TcpServer::TcpServer(boost::asio::io_context& ioc, short port)
	: m_ioc(ioc)
	, m_acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{
	Start_accept();
}

TcpServer::~TcpServer()
{
	std::cout << "TcpServer delete" << std::endl;
}

void TcpServer::ClearSession(std::string uuid)
{
	m_sessions.erase(uuid);
}

void TcpServer::SendMsg(std::string uuid, std::string msg)
{
	if (m_sessions.count(uuid) <= 0)
	{
		return;
	}
	m_sessions[uuid]->SendMsg(std::move(msg));
}

void TcpServer::Start_accept()
{
	//从服务池中取出一个iocontext用于和客户端通信
	auto& ioc_proxy = IocontextPool::GetInstance()->GetIoContextProxy();
	std::shared_ptr<Session> new_session = std::make_shared<Session>(ioc_proxy, this);
	m_acceptor.async_accept(new_session->socket(), [this, new_session](const boost::system::error_code& error) {
		handle_accept(new_session, error);
		});
	
}

void TcpServer::handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error)
{
	if (!error)
	{
		//
		std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
		new_session->Start();
		m_sessions.insert(std::make_pair(new_session->GetUuid(), new_session));
	}
	else
	{
		std::cout << "service acceptor error!!!" << std::endl;
	}
	Start_accept();
}
