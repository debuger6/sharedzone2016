#ifndef _SEND_RESOURCE_H_
#define _SEND_RESOURCE_H_

#include "Transaction.h"

// 用户发送聊天信息到公告面板
class SendResource : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif 
