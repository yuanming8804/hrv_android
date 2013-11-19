#ifndef CTIMER_H_
#define CTIMER_H_

#include <pthread.h>
#include <sys/time.h>

class CTimer
{
private:
    pthread_t thread_timer;
    long m_second, m_microsecond, m_duetime;
    void *m_parameter;
    static void *OnTimer_stub(void *p)
    {
        (static_cast<CTimer*>(p))->thread_proc();
    }
    void thread_proc();
    virtual void *OnTimer();

public:
    CTimer();
    CTimer(long second, long microsecond);
    CTimer(void *(*start_routine)(void *), long DueTime, long second, long microsecond, void *Parameter);
    virtual ~CTimer();
    void SetTimer(long second, long microsecond);
    void StartTimer();
    void StopTimer();
    void *(*start_routine)(void *);
//    void thread_exit_handler(int sig)
//    {
//    	pthread_exit(0);
//    }
};

#endif /* CTIMER_H_ */
