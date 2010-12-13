#include "common.h"


#ifndef event_threadPool_H
#define event_threadPool_H

#include <map>
#include <queue>
#include "session.h"
#include "workerThread.h"
#include "BaseEvent.h"

using namespace v8;


class ThreadPool{
	
	static ThreadPool *s_instance;
	
	ThreadPool();
	public:
		~ThreadPool();
	
		static ThreadPool *instance()
		{
			if (!s_instance){
				s_instance = new ThreadPool();
			}
			return s_instance;
		}
		int curIndex;
		int numThreads;
		int maxThreads;
		int finished;
		int eventCnt;
		
		event * wakeup_event;		
		
		std::queue<Session*> wqueue;
		std::map<pthread_t, WorkerThread*> wk;
		
		int wakeup_pipe_out;
		int wakeup_pipe_in;
		
		pthread_mutex_t lock;
		pthread_mutex_t threadLock;
		pthread_cond_t isIdle, isEmpty;
		pthread_cond_t isFull;

		static void onWakeup(evutil_socket_t fd, short what, void *arg);
		void dispatch(Session*sp);
		int saveThread(WorkerThread *thread);
		
	
};
	

	
#endif
