#include "SharedServer.h"
#include "SharedSession.h"
#include "Types.h"
#include <iostream>
using namespace std;

#include <muduo/base/Logging.h>

#include <boost/bind.hpp>

// using namespace muduo;
// using namespace muduo::net;

SharedServer::SharedServer(muduo::net::EventLoop* loop,
		                       const muduo::net::InetAddress& listenAddr)
	  : loop_(loop),
	    server_(loop, listenAddr, "SharedServer")
{
	 server_.setConnectionCallback(
			        boost::bind(&SharedServer::onConnection, this, _1));
	 server_.setMessageCallback(
				    boost::bind(&SharedServer::onMessage, this, _1, _2, _3));
}

void SharedServer::start()
{
	 server_.start();
}

//��TcpConnection::connectEstablished�л���øú������������ӽ�����ɺ���á�
void SharedServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
	 if (conn->connected())
     {
		 conn->setContext(SharedSession(conns_, conn));	// TcpConnection��һ��SharedSession��
	 }
	 else
	 {
		 cout<<"disconnecting...."<<endl;
		 SharedSession* ss = boost::any_cast<SharedSession>(conn->getMutableContext()); //return context
		 ss->removeActiveUser();
		 muduo::net::Buffer response;
		 muduo::net::Buffer response1;
		 response.append(ss->GetJos().Data(), ss->GetJos().Length());
		 response1 = response;
		 ss->removeConn(conn);
		 map<string, muduo::net::TcpConnectionPtr>::iterator mIter;
	     for (mIter = conns_.begin(); mIter != conns_.end(); mIter++ )
		 {
			mIter->second->send(&response);
			response = response1;
		}

	 }
}

void SharedServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
		                           muduo::net::Buffer* buf, muduo::Timestamp time)
{
	while (buf->readableBytes() >= kHeaderLen)
	{
		const void* data = buf->peek();
		const RequestHead* rh = static_cast<const RequestHead*>(data);
	    uint16 len = muduo::net::sockets::networkToHost16(rh->len); 
		if (buf->readableBytes() >= len + kHeaderLen)  // �ﵽһ����������Ϣ
		{
			SharedSession* ss = boost::any_cast<SharedSession>(conn->getMutableContext()); //return context
			ss->SetData(buf->peek(), kHeaderLen + len);
			ss->Process();
  				
			if (ss->GetCmd() == CMD_REGISTER)
            {
				muduo::net::Buffer response;
				response.append(ss->GetJos().Data(), ss->GetJos().Length());
				ss->Clear();
				conn->send(&response);

			}

			if (ss->GetCmd() == CMD_LOGIN)
			{
				muduo::net::Buffer response;
				muduo::net::Buffer response1;
				muduo::net::Buffer responseTemp;
				response.append(ss->GetJos().Data(), ss->GetJos().Length());
				response1.append(ss->GetJosres().Data(), ss->GetJosres().Length());
				responseTemp = response1;
				ss->Clear();
				
				conn->send(&response);
				map<string, muduo::net::TcpConnectionPtr>::iterator mIter;
				for (mIter = conns_.begin(); mIter != conns_.end(); mIter++ )
				{
					if (mIter->second != conn)
					{
						mIter->second->send(&response1);
					}
					response1 = responseTemp;
				}
			}
			if (ss->GetCmd()==CMD_SEND_CONTENT)
			{
				muduo::net::Buffer response;
				muduo::net::Buffer response1;
				response.append(ss->GetJos().Data(), ss->GetJos().Length());
				response1 = response;
				ss->Clear();
				map<string, muduo::net::TcpConnectionPtr>::iterator mIter;
				for (mIter = conns_.begin(); mIter != conns_.end(); mIter++ )
				{
					if (mIter->second != conn)
					{
						mIter->second->send(&response);
					}
					response  = response1;
				}

			}

			if (ss->GetCmd() == CMD_GET_RESOURCE)
			{
				muduo::net::Buffer response;
				response.append(ss->GetJos().Data(), ss->GetJos().Length());
				ss->Clear();
				conn->send(&response);
			}
			buf->retrieve(kHeaderLen+len);
		}

		else{
			break;
		}
	}
}


