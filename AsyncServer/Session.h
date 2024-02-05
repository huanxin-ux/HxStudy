#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <queue>

#include "IoContextProxy.h"
/*lambda: 编译期生成lambda函数，系统会将lambda优化成内联函数，
* std::bind 运行时生成函数对象，分配内存
* lambda相比于std::bind效率更高
*/
class TcpServer;



using boost::asio::ip::tcp;
class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(IoContextProxy& ioc_proxy, TcpServer* server);
	~Session();
	//返回通信socket
	tcp::socket& socket();
	//开始接收数据
	void Start();
	//获取UUID
	std::string GetUuid();
	//发送数据
	void SendMsg(std::string msg);
private:
	//读数据回调
	void HandleRead(const boost::system::error_code& error,
		size_t bytes_transferred);
	//写数据回调
	void HandleWrite(const boost::system::error_code& error,
		size_t bytes_transferred);
private:
	tcp::socket m_socket;
	enum {max_length = 1024};
	std::string m_data;
	std::string m_uuid;
	TcpServer* m_server;
	std::mutex m_send_mutex;
	std::queue<std::string> m_send_queue;
	IoContextProxy& m_iocontext_proxy;
};

