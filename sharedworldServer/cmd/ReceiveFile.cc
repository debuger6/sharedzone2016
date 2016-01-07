#include <string>
#include <iostream>
#include "../SharedSession.h"
#include "ReceiveFile.h"
#include "../dal/SharedService.h"
#include "../CreateDirOrFile.h"

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
	std::cout<<dir<<std::endl;

	//发送的文件路径全名
	string fileName;
	jis>>fileName;
	std::cout<<fileName<<std::endl;

	//标志位
	int flag;
	jis>>flag;
	std::cout<<flag<<std::endl;

	//文件内容
	string content;
	jis>>content;
	std::cout<<"ReceiveFile go..."<<std::endl;
}

