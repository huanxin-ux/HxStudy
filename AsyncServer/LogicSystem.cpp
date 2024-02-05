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
			//��ȡ��Ϣ�ڵ�
			MsgNode msg_node = GetQueueMsg();
			
			//�����ȡ������Ϣ�ڵ���Ч�����»�ȡ
			if (!msg_node.m_isvalid)
			{
				continue;
			}

			//������Ϣ�ڵ�
			std::cout << "deal msg: " << msg_node.m_msg << std::endl;
			if (m_server)
			{
				m_server->SendMsg(msg_node.m_uuid, msg_node.m_msg);
			}
		}
		std::cout << "LogicSystem�߳��˳�" << std::endl;
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

	//�ж��Ƿ�����Ϣ������û�У��ȴ�
	if (m_msg_queue.empty())
	{
		m_condition.wait(lock);
	}

	//���Ѻ���Ҫ�ٴ��жϰ��������Ƿ�������
	if (m_msg_queue.empty())
	{
		return MsgNode();
	}
	//��ȡ�����ײ�λ��
	auto msg_node = m_msg_queue.front();

	//�Ƴ���������
	m_msg_queue.pop();

	return msg_node;
}
