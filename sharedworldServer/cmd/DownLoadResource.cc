#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "DownLoadResource.h"
#include <string>
#include "../SharedSession.h"
#include <iostream>
#include "../dal/SharedService.h"
#include "CodeConverter.h"

#define MAX_SIZE	6553
using namespace std;



void* thread_func(void* arg)
{
	SharedSession *session = (SharedSession*)arg;
	JInStream jis(session->GetRequestPack()->buf, session->GetRequestPack()->head.len);
	uint16 cmd = session->GetCmd();

	// 资源所属用户
	string username;
	jis>>username;

	string filepath;
	jis>>filepath;

	string allFilePath = "/up_load_resource/" + username + "/" + filepath; 
	cout<<allFilePath<<endl;
    JOutStream& jos = session->GetJos();
	jos.Clear();
	//读取文件内容并发送给客户端
	int fd = open(allFilePath.c_str(), O_RDONLY | O_CREAT, S_IRUSR);
	char buffer[MAX_SIZE];
	if ( fd == -1)
	{
		cout<<"open file failed"<<endl;
		jos<<"end";

		muduo::net::Buffer response;
		response.append(jos.Data(), jos.Length());
		session->conn_->send(&response);
		jos.Clear();
		pthread_exit(NULL);
	}

	else
	{
		bzero(buffer, MAX_SIZE);
		uint32 len = 0;
		while ((len = read(fd, buffer, MAX_SIZE)) > 0)
		{
			cout<<"len"<<len<<endl;
			jos.AppendWithLen(buffer, len);
			//发送文件内容
			muduo::net::Buffer response;
			response.append(jos.Data(), jos.Length());

			session->conn_->send(&response);
			jos.Clear();
			memset(buffer, 0, MAX_SIZE * sizeof(char));
		}
		close(fd);
		string content = "end";
		jos.AppendWithLen(content.c_str(), content.length());

			//发送文件内容
			muduo::net::Buffer response;
			response.append(jos.Data(), jos.Length());

			session->conn_->send(&response);
		
	}

	pthread_exit(NULL);
}

void DownLoadResource::Execute(SharedSession& session)
{
	pthread_t tid;
    if (pthread_create(&tid, NULL, thread_func, (void*)&session)!=0)
	{
		cout<<"create thread error"<<endl;
	}

}
