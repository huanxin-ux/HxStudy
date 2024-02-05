#pragma once
#include "SingleT.hpp"
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>

class TcpServer;

class MsgNode
{
public:
	MsgNode()
		: m_uuid("")
		, m_msg("")
		, m_isvalid(false)
	{

	}
	MsgNode(std::string&& uuid, std::string&& msg)
		: m_uuid(uuid)
		, m_msg(msg)
		, m_isvalid(true)
	{

	}

	std::string m_uuid;
	std::string m_msg;
	bool m_isvalid;
};

class LogicSystem : public SingleT<LogicSystem>
{
	friend class SingleT<LogicSystem>;
	LogicSystem();
public:
	~LogicSystem();
	//运行逻辑线程
	void run();
	//数据入逻辑线程队列
	void PostMsg(std::string uuid, std::string msg);
	//设置服务器
	void SetServer(std::shared_ptr<TcpServer> server);
	//获取队列元素
	MsgNode GetQueueMsg();
private:
	std::shared_ptr<std::thread> m_thread;
	std::atomic_bool m_isRun;
	std::queue<MsgNode> m_msg_queue;
	std::mutex m_mutex;
	std::mutex m_read_mutex;
	std::condition_variable m_condition;
	std::shared_ptr<TcpServer> m_server;
};

