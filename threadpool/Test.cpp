
#define  WIN32_LEAN_AND_MEAN

#include "Mutex.h"
#include "Semaphore.h"
#include "Thread.h"
#include "ThreadPoolExecutor.h"


// �����߳���
#define  TEST_TREAD_COUNT 20
// ÿ�������߳�����ӵ�������
#define  TASK_COUNT 100

static int g_totalcount = 0;
static int g_seq = 0;

// ����g_seq
CRITICAL_SECTION  g_csThreadCode;

// ����������Դ
Mutex g_runmutex; 

// ��������ã��˳�ѭ�����൱���Ը�ȫ�ֲ�������
Semaphore g_semFinishFlag;


class R :public Runnable
{
public:
	~R()
	{

	}
	void Run()
	{
		EnterCriticalSection(&g_csThreadCode);
		g_seq ++;
		Sleep(0);
		printf("Hello World is: %d\n",g_seq);
		LeaveCriticalSection(&g_csThreadCode);
	}
protected:
private:
};

DWORD __stdcall WapperFun1(void* arg)
{
	CThreadPoolExecutor* poolmanager = (CThreadPoolExecutor*)arg;
	if (poolmanager == NULL)
		return 0;
	for (int i=0;i<TASK_COUNT;++i)
	{
		R* r= new R;
		g_runmutex.enter();
		while(!poolmanager->Execute(r)) 
		{
			Sleep(100); // �������ʧ�ܣ����߳�����ʱ�䣬�Ժ�������
		}
		g_totalcount++;
		g_runmutex.leave();
	}

	if (g_totalcount >= TEST_TREAD_COUNT*TASK_COUNT)
	{
		g_semFinishFlag.post();
	}
	return 0;
}

int main()
{
	// �����̳߳أ�����ʼ��10�������߳�
	CThreadPoolExecutor* pExecutor = new CThreadPoolExecutor();
	pExecutor->Init(10,64,100);

	InitializeCriticalSection(&g_csThreadCode);

	// ���20���߳�����
	HANDLE m_threadId[TEST_TREAD_COUNT] = {0};
	for (int cow =0;cow < TEST_TREAD_COUNT;cow ++)
	{
		m_threadId[cow] = CreateThread(NULL,0,WapperFun1,pExecutor,0,NULL);
	}

	// ÿ100�����ж��������Ƿ�ִ�������
	while(1)
	{
		if (g_semFinishFlag.pend(100) <0)
		{
			Sleep(10);
			continue; 
		}
		else
			break;
	}
	pExecutor->Terminate();
	delete pExecutor;
	DeleteCriticalSection(&g_csThreadCode);
	getchar();
	return 0;
}
