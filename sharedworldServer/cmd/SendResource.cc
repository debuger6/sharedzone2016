#include "SendResource.h"
#include <string>
#include "../SharedSession.h"
#include <iostream>
#include "../dal/SharedService.h"
#include "CodeConverter.h"
using namespace std;

void SendResource::Execute(SharedSession& session)
{
	CodeConverter codeConvert = CodeConverter("utf-8", "gb2312"); 

	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 资源所属用户
	string username;
	jis>>username;



	

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
	jos<<userName;
	SharedService dao;
	dao.RequestResource(username, jos);
	jos<<"end";
	size_t tailPos = jos.Length();
	jos.Reposition(lengthPos);
	jos<<(uint16)(tailPos + 8 - sizeof(ResponseHead));


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

	//session.Send(jos.Data(), jos.Length());
	
}

