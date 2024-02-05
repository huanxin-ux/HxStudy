#include "IoContextPool.h"
#include <map>
IoContextProxy& IocontextPool::GetIoContextProxy()
{
	//int-m_ioContexts中iocontext管理的socket个数  int-m_ioContexts下标
	std::map<int, int>cnt_index;
	for (int i = 0; i < m_ioContextProxys.size(); i++)
	{
		cnt_index[m_ioContextProxys[i].GetSocketNum()] = i;
	}
	auto iter = cnt_index.begin();

	return m_ioContextProxys.at(iter->second);
}

IocontextPool::IocontextPool(int thread_num /*= std::thread::hardware_concurrency()*/)
	: m_ioContextProxys(thread_num)
	, m_workers(thread_num)
{
	for (size_t i = 0; i < thread_num; i++)
	{
		//初始化work,为了iocontext在没有请求时也不会退出
		m_workers[i] = std::make_unique<boost::asio::io_context::work>(m_ioContextProxys[i].GetIoContext());

		//初始化线程
		m_threads.emplace_back([this, i]() {
			m_ioContextProxys[i].run();
		});
	}

	std::cout << "io收发线程数: " << thread_num << std::endl;
}

IocontextPool::~IocontextPool()
{
	for (auto& work_ptr : m_workers)
	{
		work_ptr.reset();
	}

	for (auto& io_context_proxy : m_ioContextProxys)
	{
		io_context_proxy.stop();
	}

	for (auto& thread : m_threads)
	{
		thread.join();
	}

	std::cout << "IocontextPool delete" << std::endl;
}
