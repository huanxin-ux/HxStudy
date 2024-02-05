#pragma once
#include <boost/asio.hpp>
class IoContextProxy
{
public:
	IoContextProxy();
	void AddSocketManagerCnt();
	void RemoveSocketManagerCnt();
	int GetSocketNum();

	boost::asio::io_context& GetIoContext();
	void run();
	void stop();
private:
	int m_socket_count;
	boost::asio::io_context m_iocontext;
};