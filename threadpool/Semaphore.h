//
//	Copyright (c)1998-2014,Xunmei Technology
//	All Rights Reserved
//
//Description:
// Semaphore.h  2013-02-05 jiaogwei

/************************************************************************/
/* 
	http://processors.wiki.ti.com/images/4/4f/Semaphores.pdf
*/
/************************************************************************/

#pragma once

// ��ƽ̨�ź�����
class Semaphore
{
	Semaphore(Semaphore const &);
	Semaphore& operator=(Semaphore const &);
public:
	// ���캯�����ᴴ��ϵͳ�ź���
	// ���ﴴ��LONG_MAX����󲢷�����
	explicit Semaphore(int initialCount =0);
	// ����������������ϵͳ������
	~Semaphore(void);

	// Pending on a semaphore decrements count
	// Ҫ������һ��semaphore
	// �����ź�������,����Ѿ����ٵ�0�����������õ��߳�
	// ����ֵ�� >=0 �ɹ��� <0 ����
	int pend();

	// Pending on a semaphore decrements count
	// �����ź�������������Ǵ�0�ۼӣ��ỽ����ȴ����еĵ�һ���߳�
	// ��������ʱʱ�䣬��λ����
	// ����ֵ�� >=0 �ɹ��� <0 �����ʱ
	int pend(unsigned long timeout);

	// Posting a semaphore increments count
	// �����ź�������������Ǵ�0�ۼӣ��ỽ����ȴ����еĵ�һ���߳�
	// ����ֵ����ǰ�ź�������
	int post();
private:
	struct SemaphoreInternal;
	SemaphoreInternal* m_internal;
};
