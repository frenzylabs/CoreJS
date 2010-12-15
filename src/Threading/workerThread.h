#include "common.h"

#ifndef event_workerThread_H
#define event_workerThread_H

#include "session.h"
#include "ThreadBase.h"

class WorkerThread
{	
	public:
		WorkerThread(){};
		~WorkerThread(){};

		pthread_t wId;
		pthread_mutex_t wLock;
		pthread_cond_t wCond;
		ThreadBase *tb;	
};

#endif