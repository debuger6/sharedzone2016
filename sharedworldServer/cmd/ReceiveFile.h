#ifndef _RECEIVE_FILE_H_
#define _RECEIVE_FILE_H_

#include "Transaction.h"

// �û�����������Ϣ���������
class ReceiveFile : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
private:
	int fd;
};



#endif 
