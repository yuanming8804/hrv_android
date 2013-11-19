#include "Timer.h"
#include <stdio.h>
#include <sys/select.h>
#include <time.h>
//#include <pthread.h>
#include <iostream>

//#ifndef NULL
//#define NULL  0//((void *) 0)
//#endif

//using namespace std;

//void (*FunP)(int );

void timer_thread_exit_handler(int sig)
{
	pthread_exit(0);
}

//////////////////////////public methods//////////////////////////
CTimer::CTimer():
    m_second(0), m_microsecond(0)
{
}

CTimer::CTimer(long second, long microsecond) :
    m_second(second), m_microsecond(microsecond)
{
}

CTimer::CTimer(void *(*start_routine)(void *), long DueTime, long second, long microsecond, void *Parameter)
{
	this->start_routine = *start_routine;
	this->m_duetime = DueTime;
	this->m_second = second;
	this->m_microsecond = microsecond;
	this->m_parameter = Parameter;
}

CTimer::~CTimer()
{
}

void CTimer::SetTimer(long second, long microsecond)
{
	this->m_second = second;
	this->m_microsecond = microsecond;
}

void CTimer::StartTimer()
{
    pthread_create(&thread_timer, NULL, OnTimer_stub, this);
}

void CTimer::StopTimer()
{
    //pthread_cancel(thread_timer);
	//pthread_join(thread_timer, NULL); //wait the thread stopped
	// 终止线程
	int status;
	if ( (status = pthread_kill( thread_timer, SIGUSR1 ) ) != 0 )
	{
		//printf("Error cancelling thread %d, error = %d (%s)", pthread_id, status, strerror status));
	}
}

//////////////////////////private methods//////////////////////////
void CTimer::thread_proc()
{
	// 首次执行代码之前的间隔
	if (m_duetime > 0)
	{
		struct timeval tempval;
		tempval.tv_sec = 0;
		tempval.tv_usec = m_duetime;
		select(0, NULL, NULL, NULL, &tempval);
	}
    while (true)
    {
    	// 强制退出
		struct sigaction actions;
		memset(&actions, 0, sizeof(actions));
		sigemptyset(&actions.sa_mask);
		actions.sa_flags = 0;
		actions.sa_handler = timer_thread_exit_handler;
		int rc = sigaction(SIGUSR1, &actions, NULL);

        //OnTimer();
        this->start_routine(this->m_parameter);
        //pthread_testcancel();
        struct timeval tempval;
        tempval.tv_sec = m_second;
        tempval.tv_usec = m_microsecond;
        select(0, NULL, NULL, NULL, &tempval);
    }
}

void *CTimer::OnTimer()
{
    //cout << "Timer once..." << endl;
	return (void *)0;
}
