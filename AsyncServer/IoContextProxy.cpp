#include "IoContextProxy.h"

IoContextProxy::IoContextProxy()
	: m_socket_count(0)
{

}

void IoContextProxy::AddSocketManagerCnt()
{
	m_socket_count++;
}

void IoContextProxy::RemoveSocketManagerCnt()
{
	if (m_socket_count > 0)
	{
		m_socket_count--;
	}
}

int IoContextProxy::GetSocketNum()
{
	return m_socket_count;
}

boost::asio::io_context& IoContextProxy::GetIoContext()
{
	return m_iocontext;
}

void IoContextProxy::run()
{
	m_iocontext.run();
}

void IoContextProxy::stop()
{
	m_iocontext.stop();
}


