#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "DownLoadResource.h"
#include <string>
#include "../SharedSession.h"
#include <iostream>
#include "../dal/SharedService.h"
#include "CodeConverter.h"

#define MAX_SIZE	65536
using namespace std;

void DownLoadResource::Execute(SharedSession& session)
{

	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 资源所属用户
	string username;
	jis>>username;

	string filepath;
	jis>>filepath;

	string allFilePath = "/up_load_resource/" + username + "/" + filepath; 
	cout<<allFilePath<<endl;
    JOutStream& jos = session.GetJos();
	jos.Clear();
	//读取文件内容并发送给客户端
	
	int fd = open(allFilePath.c_str(), O_RDONLY);
//	int fd = open("/up_load_resource/admin/C\:/Users/Administrator/Desktop/mongo.txt", O_RDONLY);
	std::cout<<"/up_load_resource/admin/C\:/Users/Administrator/Desktop/mongo.txt"<<endl;
	char buffer[MAX_SIZE];
	if ( fd == -1)
	{
		cout<<"open file failed"<<endl;
		jos<<"end";

		muduo::net::Buffer response;
		response.append(jos.Data(), jos.Length());
		session.conn_->send(&response);
		jos.Clear();
		return;
	}

	else
	{
		bzero(buffer, MAX_SIZE);
		uint32 len = 0;
		while ((len = read(fd, buffer, MAX_SIZE)) > 0)
		{
			std::vector<char> bf = jos.GetBuffer();
			string content(buffer, len);
			cout<<content<<endl;
			jos<<content;
			//发送文件内容
			muduo::net::Buffer response;
			response.append(jos.Data(), jos.Length());

			session.conn_->send(&response);
			jos.Clear();
			memset(buffer, 0, MAX_SIZE * sizeof(char));
		}
		close(fd);
		jos<<"end";

			//发送文件内容
			muduo::net::Buffer response;
			response.append(jos.Data(), jos.Length());

			session.conn_->send(&response);
		
	}

}

