#ifndef _RECEIVE_FILE_H_
#define _RECEIVE_FILE_H_

#include "Transaction.h"

// 用户发送聊天信息到公告面板
class ReceiveFile : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
private:
	int fd;
};



#endif 
