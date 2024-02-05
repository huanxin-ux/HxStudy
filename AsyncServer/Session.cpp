#include "Session.h"
#include <iostream>
#include <memory>
#include "LogicSystem.h"
#include "TcpServer.h"
Session::Session(IoContextProxy& ioc_proxy, TcpServer* server)
	: m_socket(ioc_proxy.GetIoContext())
	, m_server(server)
	, m_iocontext_proxy(ioc_proxy)
{
	auto uuid = boost::uuids::random_generator()();
	m_uuid = boost::uuids::to_string(uuid);

	m_iocontext_proxy.AddSocketManagerCnt();
}

Session::~Session()
{
	m_socket.close();
	
	m_iocontext_proxy.RemoveSocketManagerCnt();
	printf("delete Session address: %x\n", this);
	//std::cout << "delete Session" << std::endl;
}

tcp::socket& Session::socket()
{
	return m_socket;
}

void Session::Start()
{
	m_data.resize(max_length, 0);
	m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
		[session = this->shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
			session->HandleRead(error, bytes_transferred);
		});
}

std::string Session::GetUuid()
{
	return m_uuid;
}

void Session::SendMsg(std::string msg)
{
	std::unique_lock<std::mutex> lock(m_send_mutex);

	size_t msg_size = m_send_queue.size();

	m_send_queue.push(std::move(msg));

	if (msg_size <= 0)
	{
		boost::asio::async_write(m_socket, boost::asio::buffer(m_send_queue.front()),
			[session = this->shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
			session->HandleWrite(error, bytes_transferred);
		});
	}
	
}

void Session::HandleRead(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)//没有错误
	{
		std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
		std::cout << "service recvive: " << m_data << std::endl;
		LogicSystem::GetInstance()->PostMsg(m_uuid, std::string(m_data.data(), bytes_transferred));

		Start();
	}
	else//出现错误
	{
		std::cout << "service recvive error!!!" << std::endl;
		m_server->ClearSession(m_uuid);
	}
}

void Session::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)//没有错误
	{
		std::unique_lock<std::mutex> lock(m_send_mutex);
		if (!m_send_queue.empty())
		{
			m_send_queue.pop();
		}
		if (!m_send_queue.empty())
		{
			auto& msg = m_send_queue.front();
			boost::asio::async_write(m_socket, boost::asio::buffer(msg),
				[session = this->shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
				session->HandleWrite(error, bytes_transferred);
			});
		}
		//else
		//{
		//	std::cout << "send msg finish" << std::endl;
		//}
		
	}
	else
	{
		std::cout << "service send error!!!" << std::endl;
		m_server->ClearSession(m_uuid);
	}
}
