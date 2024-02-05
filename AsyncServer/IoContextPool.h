#pragma once
#include <thread>
#include <boost/asio.hpp>

#include "SingleT.hpp"
#include "IoContextProxy.h"

using WorkPtr = std::unique_ptr<boost::asio::io_context::work>;

class IocontextPool : public SingleT<IocontextPool>
{
public:
	IoContextProxy& GetIoContextProxy();

	~IocontextPool();
private:
	friend SingleT<IocontextPool>;
	IocontextPool(int thread_num = std::thread::hardware_concurrency());
	
	std::vector<WorkPtr> m_workers;
	std::vector<IoContextProxy> m_ioContextProxys;
	std::vector<std::thread> m_threads;	
};
