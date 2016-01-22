#ifndef _DOWNLOAD_RESOURCE_H_
#define _DOWNLOAD_RESOURCE_H_

#include "Transaction.h"

// 用户发送聊天信息到公告面板
class DownLoadResource : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif 
