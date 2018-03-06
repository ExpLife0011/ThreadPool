#pragma once

#include <set>
#include <list>
#include "Thread.h"

class CThreadPoolExecutor
{
public:
	CThreadPoolExecutor(void);
	virtual ~CThreadPoolExecutor(void);

	/*
	  ��ʼ���̳߳أ�����minThreads���߳�
	*/
	bool Init(unsigned int minThreads,unsigned int maxThreads,unsigned int maxPendingTaskse);

	/*
	  �������
	  ִ�����������������б�û����������������뵽�����б�����true
	  ����ǰ�����б����ˣ�����ǰ�߳�����С������߳��������������߳�ִ�д����񣬷���true
	  ����ǰ�����б����ˣ�����ǰ�߳�������������߳���������ʧ�����񣬷���false
	  ����ֵ���ɹ�true��ʧ��false
	*/
	bool Execute(Runnable* pRunnable);
	
	/*
	  ��ֹ�̳߳أ�����ֹ�������Ȼ��ȴ������б�Ϊ�գ�
	  ������С�߳�����Ϊ0���ȴ�ֱ���߳�����ӢΪ�գ�
	  ����������е�����
	*/
	void Terminate();

	/*
	  �����̳߳��е�ǰ���߳�����
	*/

	unsigned int GetThreadPoolSize();

private:
	/*
	   ��ȡ�����б��е�����������Ϊ�գ�����NULL
	*/
	Runnable* GetTask();
	static unsigned int WINAPI StaticThreadFunc(void * arg);
private:
	class CWork:public CThread
	{
	public:
		CWork(CThreadPoolExecutor* pThreadPool,Runnable* pFirstTask = NULL);
		virtual ~CWork();
		void Run();
	private:
		CThreadPoolExecutor * m_pThreadPool;
		Runnable* m_pFirstTask;
		volatile bool m_bRun;
	};

	typedef std::set<CWork *>ThreadPool;
	typedef std::list<Runnable *> Tasks;
	typedef ThreadPool::iterator ThreadPoolItr;

	ThreadPool m_ThreadPool;
	ThreadPool m_TrashThread;
	Tasks m_Tasks;

	CRITICAL_SECTION m_csTasksLock;
	CRITICAL_SECTION m_csThreadPoolLock;

	volatile bool m_bRun;
	volatile bool m_bEnableInsertTask;
	volatile unsigned int m_minThreads;
	// ��࿪���߳���
	volatile unsigned int m_maxThreads;
	// ����������������
	volatile unsigned int m_maxPendingTasks;
};
