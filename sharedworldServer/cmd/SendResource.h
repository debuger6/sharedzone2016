#ifndef _SEND_RESOURCE_H_
#define _SEND_RESOURCE_H_

#include "Transaction.h"

// �û�����������Ϣ���������
class SendResource : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif 
