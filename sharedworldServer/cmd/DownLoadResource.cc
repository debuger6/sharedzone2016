#include "DownLoadResource.h"
#include <string>
#include "../SharedSession.h"
#include <iostream>
#include "../dal/SharedService.h"
#include "CodeConverter.h"
#define MAX_SIZE	65536
using namespace std;

void SendResource::Execute(SharedSession& session)
{

	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 资源所属用户
	string username;
	jis>>username;

	string filepath;
	jis>>filepath;

	string allFilePath = "/up_load_resource/" + username + "/" + filepath; 

	MD5 md5;
	int16 error_code = 0;
	char error_msg[31] = {0};

	JOutStream& jos = session.GetJos();

	// 包头命令
	jos<<cmd;
	size_t lengthPos = jos.Length();
	jos.Skip(2);
	// 包头cnt、seq、error_code、error_msg
	uint16 cnt = 0;
	uint16 seq = 0;
	jos<<cnt<<seq<<error_code;
	jos.WriteBytes(error_msg, 30);
	
	//读取文件内容并发送给客户端
	FILE *fp = fopen(allFilePath, "r");
	char buffer[MAX_SIZE];
	if (fp == NULL)
	{
		jos<<"";

		size_t tailPos = jos.Length();
		jos.Reposition(lengthPos);
		jos<<static_cast<uint16>(tailPos + 8 - sizeof(ResponseHead)); // 包体长度 + 包尾长度
		// 包尾
		jos.Reposition(tailPos);
		 // 计算包尾
		unsigned char hash[16];
		md5.MD5Make(hash, (const unsigned char*)jos.Data(), jos.Length());
		for (int i=0; i<8; ++i)
		{
			 hash[i] = hash[i] ^ hash[i+8];
			 hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
		}
		jos.WriteBytes(hash, 8);

		muduo::net::Buffer response;
		response.append(jos.Data(), jos.Length());
		session.conn_->send(&response);
		session.conn_->connectDestroyed();
		jos.Clear();
		return;
	}

	else
	{
		bzero(buffer, BUFFER_SIZE);
		int len = 0;
		while ((len = fread(buffer, sizeof(char), MAX_SIZE, fp)) > 0)
		{
			string content(buffer, len);
			jos<<content;


			size_t tailPos = jos.Length();
			jos.Reposition(lengthPos);
			jos<<static_cast<uint16>(tailPos + 8 - sizeof(ResponseHead)); // 包体长度 + 包尾长度
			// 包尾
			jos.Reposition(tailPos);
			 // 计算包尾
			unsigned char hash[16];
			md5.MD5Make(hash, (const unsigned char*)jos.Data(), jos.Length());
			for (int i=0; i<8; ++i)
			{
				 hash[i] = hash[i] ^ hash[i+8];
				 hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
			}
			jos.WriteBytes(hash, 8);
			//发送文件内容
			muduo::net::Buffer response;
			response.append(jos.Data(), len);
			session.conn_->send(&response);

			memset(buffer, 0, MAX_SIZE * sizeof(char));

            
        	// 包头命令
			jos<<cmd;
			lengthPos = jos.Length();
			jos.Skip(2);
			jos<<cnt<<seq<<error_code;
			jos.WriteBytes(error_msg, 30);

		}
		
		session.conn_->connectDestroyed();
	}

}

