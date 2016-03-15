#include "TransactionManager.h"
#include "cmd/UserLogin.h"
#include "cmd/UserRegister.h"
#include "cmd/SendChatContent.h"
#include "cmd/ReceiveFile.h"
#include "cmd/SendResource.h"
#include "cmd/DownLoadResource.h"


TransactionManager::TransactionManager()
{
	m_actions[CMD_LOGIN] = new UserLogin;
	m_actions[CMD_REGISTER] = new UserRegister;
	m_actions[CMD_SEND_CONTENT] = new SendChatContent;
	m_actions[CMD_UPLOAD_FILE_RESOURCE] = new ReceiveFile;
	m_actions[CMD_GET_RESOURCE] = new SendResource;
	m_actions[CMD_DOWNLOAD_RESOURCE] = new DownLoadResource;
}

TransactionManager::~TransactionManager()
{
	for (map<uint16, Transaction*>::iterator it=m_actions.begin();
		it!=m_actions.end();++it)
	{
			delete it->second;
	}
}

bool TransactionManager::DoAction(SharedSession& session)
{
	uint16 cmd = session.GetCmd();
	std::cout<<"cmd  "<<cmd<<std::endl;
	if (m_actions.find(cmd) != m_actions.end())
	{
		m_actions[cmd]->Execute(session);
		return true;
	}

	return false;
}
