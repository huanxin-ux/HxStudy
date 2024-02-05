// AsyncServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "TcpServer.h"
#include "LogicSystem.h"


int main()
{
	try
	{
		
		boost::asio::io_context ioc;
		std::shared_ptr<TcpServer> server(new TcpServer(ioc, 8888));
		LogicSystem::GetInstance()->SetServer(server);

		std::thread thread([&ioc, server] {		
			ioc.run();
			std::cout << "io线程退出" << std::endl;
			});


		LogicSystem::GetInstance()->run();
		while (true)
		{
			std::string msg;
			std::cin >> msg;
			if (msg == "exit")
			{
				//LogicSystem::GetInstance()->PostMsg("","exit");
				ioc.stop();
				thread.join();			
				return 0;
			}
		}
		
	}
	catch (const std::exception& e)
	{
		std::cerr << "exception: " << e.what() << std::endl;
	}

	
	
}


