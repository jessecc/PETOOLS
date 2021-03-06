// Link.cpp: implementation of the CLink class.
//
//////////////////////////////////////////////////////////////////////

#include"../PELib.h"
#include <Windows.h>
#include "CLink.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLink::CLink()
{
	m_head = NULL;
	m_last = NULL;
	m_curpos = NULL;
	m_prepos = NULL;
}

CLink::~CLink()
{
	RemoveAll();
}

T_Link* CLink::GetHead()
{
	return m_head;
}

T_Link* CLink::GetLast()
{
	return m_last;
}

T_Link* CLink::GetCurPos()
{
	return m_curpos;
}

void CLink::AddTail(void* data)
{
	T_Link	*nextl = new T_Link;
	memset(nextl,0,sizeof(T_Link));//确保为0

	//nextl->npc = new NPC;
	nextl->next = NULL;
	if(m_head == NULL)
	{
		m_head = nextl;
		m_last = nextl;
		nextl->data = data;
		//memcpy(nextl->npc, npc, sizeof(NPC));
	}
	else
	{
		m_last->next = nextl;
		m_last = nextl;
		nextl->data = data;
		//memcpy(nextl->npc, npc, sizeof(NPC));
	}
}

void CLink::RemoveAll()
{
	T_Link *pos = m_head;
	
	m_head = NULL;
	m_last = NULL;
	m_curpos = NULL;
	m_prepos = NULL;

	if(pos == NULL)
		return;
	do
	{
		delete pos->data;
		T_Link *tmp = pos->next;
		delete pos;
		pos = tmp;
	}
	while(pos != NULL);
}

void CLink::Remove()
{
	if(m_curpos != NULL)
	{
		if(m_curpos == m_head)	//删除头指针
		{
			m_head = m_curpos->next;
			delete m_curpos->data;
			if(m_head == NULL)
			{
				m_last = NULL;
				m_curpos = NULL;
				m_prepos = NULL;				
			}
		}
		else if(m_curpos == m_last)//删除尾指针
		{
			m_last = m_prepos;
			m_last->next = NULL;
			//delete m_curpos->data;
		}
		else	//删除
		{
			m_prepos->next = m_curpos->next;
			delete m_curpos->data;
			GetNext();//删除后让当前链等于下一个
			delete m_prepos;//然后释放自己链(GetNext里保存的)
		}
	}
}

T_Link* CLink::GetNext()
{
	m_prepos = m_curpos;
	if(m_curpos != NULL)
		m_curpos = m_curpos->next;

	return m_curpos;
}

T_Link* CLink::GetLink()
{
	m_curpos = m_head;
	m_prepos = m_curpos;

	return m_curpos;
}
  