#include "LogicSystem.h"
#include "TcpServer.h"

LogicSystem::LogicSystem()
	: m_thread(nullptr)
	, m_isRun(true)
	, m_server(nullptr)
{

}

LogicSystem::~LogicSystem()
{
	m_isRun.store(false);
	m_condition.notify_all();
	m_thread->join();
	std::cout << "logic system delete" << std::endl;
}

void LogicSystem::run()
{
	m_thread = std::make_shared<std::thread>([this]() {
		while (m_isRun.load())
		{
			//获取消息节点
			MsgNode msg_node = GetQueueMsg();
			
			//如果获取到的消息节点无效，重新获取
			if (!msg_node.m_isvalid)
			{
				continue;
			}

			//处理消息节点
			std::cout << "deal msg: " << msg_node.m_msg << std::endl;
			if (m_server)
			{
				m_server->SendMsg(msg_node.m_uuid, msg_node.m_msg);
			}
		}
		std::cout << "LogicSystem线程退出" << std::endl;
	});
	
}

void LogicSystem::PostMsg(std::string uuid, std::string msg)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_msg_queue.push(MsgNode(std::move(uuid), std::move(msg)));
	m_condition.notify_one();
}

void LogicSystem::SetServer(std::shared_ptr<TcpServer> server)
{
	m_server = server;
}

MsgNode LogicSystem::GetQueueMsg()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	//判断是否有消息处理，若没有，等待
	if (m_msg_queue.empty())
	{
		m_condition.wait(lock);
	}

	//唤醒后需要再次判断按队列里是否有数据
	if (m_msg_queue.empty())
	{
		return MsgNode();
	}
	//获取队列首部位置
	auto msg_node = m_msg_queue.front();

	//移除队首数据
	m_msg_queue.pop();

	return msg_node;
}
