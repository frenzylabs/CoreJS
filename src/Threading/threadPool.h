#include "common.h"


#ifndef event_threadPool_H
#define event_threadPool_H

#include <errno.h>
#include <signal.h>

#include <iostream>
#include <map>
#include <queue>
#include "workerThread.h"
#include "BaseEvent.h"
#include "ThreadBase.h"

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
		int numThreads;
		int maxThreads;
		int finished;
		int eventCnt;
				
		
		std::queue<ThreadBase* > wqueue;
		std::map<pthread_t, WorkerThread*> wk;
		
		event * wakeup_event;
		int wakeup_pipe_out;
		int wakeup_pipe_in;
		
		pthread_mutex_t threadLock;
		
		static void onNotify(evutil_socket_t fd, short what, void *arg);
		void dispatch(ThreadBase *t);
	
};
	

	
#endif
