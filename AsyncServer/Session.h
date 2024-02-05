#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <queue>

#include "IoContextProxy.h"
/*lambda: ����������lambda������ϵͳ�Ὣlambda�Ż�������������
* std::bind ����ʱ���ɺ������󣬷����ڴ�
* lambda�����std::bindЧ�ʸ���
*/
class TcpServer;



using boost::asio::ip::tcp;
class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(IoContextProxy& ioc_proxy, TcpServer* server);
	~Session();
	//����ͨ��socket
	tcp::socket& socket();
	//��ʼ��������
	void Start();
	//��ȡUUID
	std::string GetUuid();
	//��������
	void SendMsg(std::string msg);
private:
	//�����ݻص�
	void HandleRead(const boost::system::error_code& error,
		size_t bytes_transferred);
	//д���ݻص�
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

