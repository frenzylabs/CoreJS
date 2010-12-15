#include "threadPool.h"
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace v8;

ThreadPool *ThreadPool::s_instance;

ThreadPool::ThreadPool(){
	maxThreads = 2;
	numThreads = 0;
	finished = 0;
	eventCnt = 0;
	
	if (pthread_mutex_init(&threadLock, NULL)) {
		fprintf(stderr, "pthread_mutex_init failed\n");
		exit(1);
	}
}

ThreadPool::~ThreadPool(){
	
	std::map<pthread_t, WorkerThread*>::iterator it;
	for ( it=this->wk.begin() ; it != this->wk.end(); it++ )
	{
		free(it->second);
	}

	pthread_mutex_destroy( &threadLock );

}


void ThreadPool::onNotify(evutil_socket_t fd, short what, void *arg){

	ThreadPool *tpool = ThreadPool::instance();
	char byte;	
	int n = read(tpool->wakeup_pipe_out, &byte, 1);

	if(tpool->eventCnt>0 && n>0)
	{
		std::map<pthread_t, WorkerThread *>::iterator it;
		for ( it=tpool->wk.begin() ; it != tpool->wk.end(); it++ )
		{
			if(it->second->tb->ready == 1)
			{	
				pthread_mutex_lock(&tpool->threadLock);
					Handle<Value> val = {String::New(it->second->tb->data.c_str())};
					Handle<Value> res;
					res = (tpool->wk[it->first]->tb->callback)->Call(v8::Context::GetCurrent()->Global(), 1, &val);
					tpool->eventCnt--;
				pthread_mutex_unlock(&tpool->threadLock);

				if((int)tpool->wqueue.size()>0){
					pthread_mutex_lock(&tpool->threadLock);
						ThreadBase *temp = tpool->wqueue.front();
						tpool->wqueue.pop();
						tpool->wk[it->first]->tb = temp;
					pthread_mutex_unlock(&tpool->threadLock);
					pthread_cond_signal( &tpool->wk[it->first]->wCond ) ;
				}
				else
				{
					pthread_mutex_lock(&tpool->threadLock);
						if(tpool->eventCnt==0) {
							pthread_mutex_unlock(&tpool->threadLock);
							break;
						}
					pthread_mutex_unlock(&tpool->threadLock);
				}
			}
		}
	}
		
	pthread_mutex_lock(&tpool->threadLock);					
		if(tpool->eventCnt ==0)
		{
			tpool->finished = 1;
			if(event_del(tpool->wakeup_event)<0)
			{
				printf("could not delete event\n");
			}
		}
	pthread_mutex_unlock(&tpool->threadLock);
	
}

void * initialized(void* t){
	
	ThreadPool *pool = ThreadPool::instance();	
	WorkerThread * thread = static_cast<WorkerThread*>(t);
	
	
		
	pthread_t threadID = pthread_self();
	pthread_mutex_lock( &pool->threadLock );
	thread->wId = threadID;
	pool->wk[threadID] = thread;
	pthread_mutex_unlock( &pool->threadLock );

	while (1) {
		int rc = pthread_mutex_lock(&pool->threadLock);
		if(!pool->wk.count(thread->wId) || rc <0 || pool->eventCnt == 0){
			pthread_mutex_unlock( &pool->threadLock );
			break;
		}
		string temp = (*thread->tb)();
		thread->tb->data = temp;
		pool->wk[thread->wId]->tb->ready=1;
		
		if(pool->finished==1) break;
		
		pthread_mutex_unlock( &pool->threadLock );	
		
		int n = write(pool->wakeup_pipe_in, "", 1);
		if(n<0){
			break;
		}
		pthread_mutex_lock( &pool->threadLock );
		pthread_cond_wait( &thread->wCond, &pool->threadLock );
		pthread_mutex_unlock( &pool->threadLock );
	}
	return NULL;
}


void ThreadPool::dispatch(ThreadBase *t){
	this->eventCnt++;
	
	if(this->numThreads >= this->maxThreads ){
		pthread_mutex_lock(&this->threadLock);
		this->wqueue.push(t);	
		pthread_mutex_unlock(&this->threadLock);
	}else{
		
		if(this->numThreads<=this->maxThreads){
		
			WorkerThread * thread = new WorkerThread();			
			thread->tb = t;
			
			if( 0 == pthread_create( &( thread->wId ), NULL, initialized, thread ) ) {
				this->numThreads++;
			} else {
				std::cout << "Unable to create thread" << std::endl;
				free( thread );
			}
		} 
	}
}

