#include <string>
#include <iostream>
#include <fcntl.h>
#include "../SharedSession.h"
#include "ReceiveFile.h"
#include "../dal/SharedService.h"
#include "../CreateDirOrFile.h"
#define MAX_SIZE 65536

void ReceiveFile::Execute(SharedSession& session)
{

	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 发送者用户名
	string username;
	jis>>username;
    std::cout<<username<<std::endl;

	//发送的文件目录
	string dir;
	jis>>dir;

	//发送的文件路径全名
	string fileName;
	jis>>fileName;
	std::cout<<fileName<<std::endl;

	//标志位
	int flag;
	jis>>flag;
	std::cout<<"flag"<<flag<<std::endl;

	//文件内容
	char content[MAX_SIZE];
	memset(content, 0, MAX_SIZE * sizeof(char));
	uint32 len = jis.Extract(content);
	std::cout<<len<<std::endl;
	int fd;
	if (flag == 0)
	{
		if (is_dir_exist(("/up_load_resource/" + username + "/"  + dir+ "/").c_str()) == 0)
		{
			//创建文件
			fd = open(("/up_load_resource/" + username + "/" + fileName).c_str(), O_RDWR | O_APPEND | O_CREAT, S_IWUSR|S_IRUSR);
			cout<<"fd" << fd <<endl;
			if (fd != -1)
			{
				if(write(fd, content, len) == -1)
					return;
				std::cout<<"write successful"<<endl;
				close(fd);
			}
			else {
				std::cout<<"open failed"<<std::endl;
				return;
			}
			
		}
		else
		{
			//创建目录
			create_multi_dir(("/up_load_resource/" + username + "/" + dir+"/").c_str());
			//创建文件
			fd = open(("/up_load_resource/" + username + "/" + fileName).c_str(), O_RDWR | O_APPEND | O_CREAT ,S_IRUSR | S_IWUSR);
			cout<<"fd" << fd <<endl;
			if(write(fd, content, len) == -1)
			{

				close(fd);
				return;
			}
				close(fd);

		}
	}
	else if (flag > 0)
	{
			//创建文件
			fd = open(("/up_load_resource/" + username + "/" + fileName).c_str(), O_RDWR | O_APPEND | O_CREAT);
			cout<<"fd" << fd <<endl;
			if(write(fd, content, len) == -1)
			{
				close(fd);
				
				return;
			}

				close(fd);
	}

	else if (flag == -1)
	{
		SharedService dao;
		dao.UpLoadFile(fileName, username);
		close(fd);
	}
}

