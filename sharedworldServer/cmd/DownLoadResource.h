#ifndef _DOWNLOAD_RESOURCE_H_
#define _DOWNLOAD_RESOURCE_H_

#include "Transaction.h"

// �û�����������Ϣ���������
class DownLoadResource : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif 
