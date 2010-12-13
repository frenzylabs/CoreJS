#include "threadPool.h"
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace v8;

ThreadPool *ThreadPool::s_instance;



void ThreadPool::onWakeup(evutil_socket_t fd, short what, void *arg){
	ThreadPool *tpool = ThreadPool::instance();
	pthread_mutex_lock(&tpool->threadLock);
			
	char byte;
	int n;
	if ((n = read(tpool->wakeup_pipe_out, &byte, 1)) <= 0) {
	  if (n == 0)
	    printf("got a close\n");
	  else
	    printf("***read***\n");
	} else {

		std::map<pthread_t, WorkerThread *>::iterator it;
		for ( it=tpool->wk.begin() ; it != tpool->wk.end(); it++ )
		{
			if(it->second->cursession->ready == 1)
			{	
				Handle<Value> val = {String::New(it->second->cursession->data.c_str())};
				Handle<Value> res;
				res = (tpool->wk[it->first]->cursession->callback)->Call(v8::Context::GetCurrent()->Global(), 1, &val);
				tpool->eventCnt--;
				
				if((int)tpool->wqueue.size()>0){
					Session *temp = tpool->wqueue.front();
					tpool->wqueue.pop();
					tpool->wk[it->first]->cursession = temp;
					pthread_mutex_lock( &tpool->wk[it->first]->wLock );
					pthread_cond_signal( &tpool->wk[it->first]->wCond ) ;
					pthread_mutex_unlock ( &tpool->wk[it->first]->wLock );
				}
				else
				{
					free(tpool->wk[it->first]);
					tpool->wk.erase(it->first);
				}
			}
		}
		if(tpool->eventCnt ==0)
		{
			tpool->finished = 1;
			if(event_del(tpool->wakeup_event)<0)
			{
				printf("could not delete event\n");
			}
		}
	}
	pthread_mutex_unlock(&tpool->threadLock);
}



ThreadPool::ThreadPool(){
	maxThreads = 2;
	curIndex = 0;
	numThreads = 0;
	finished = 0;
	eventCnt = 0;
	
	if (pthread_mutex_init(&threadLock, NULL)) {
		fprintf(stderr, "pthread_mutex_init failed\n");
		exit(1);
	}
	pthread_cond_init( &isIdle, NULL );
	pthread_cond_init( &isFull, NULL );
	pthread_cond_init( &isEmpty, NULL );
}

ThreadPool::~ThreadPool(){
	
	
	std::map<pthread_t, WorkerThread*>::iterator it;
	for ( it=this->wk.begin() ; it != this->wk.end(); it++ )
	{
		free(it->second->cursession);
		free(it->second);
	}

	pthread_mutex_destroy( &threadLock );
	pthread_cond_destroy( &isIdle );
	pthread_cond_destroy( &isFull );
	pthread_cond_destroy( &isEmpty );

}

void * waitforit(void* t){
	
	ThreadPool *pool = ThreadPool::instance();	
	WorkerThread * thread = static_cast<WorkerThread*>(t);
	
	pthread_t threadID = pthread_self();
	thread->wId = threadID;
	pthread_mutex_lock( &pool->threadLock );
	pool->wk[threadID] = thread;
	pthread_mutex_unlock( &pool->threadLock );

	while (1) {

		string temp = thread->cursession->funcptr(thread->cursession);
		pthread_mutex_lock(&pool->threadLock);
		thread->cursession->data = temp;
		pthread_mutex_unlock(&pool->threadLock);

		if(pool->finished==1) break;

		pthread_mutex_lock( &thread->wLock );
		if( 0 == pool->saveThread( thread ) ) {
			pthread_cond_wait( &thread->wCond, &thread->wLock );
			pthread_mutex_unlock( &thread->wLock );
		} 
		else 
		{
			pthread_mutex_unlock( &thread->wLock );
			pthread_cond_destroy( &thread->wCond );
			pthread_mutex_destroy( &thread->wLock );
			break;
		}
	}
	return NULL;
}

void ThreadPool::dispatch(Session *sp){
	
	int ret = 0;
	
	this->eventCnt++;
	pthread_attr_t attr;
	
	if(this->curIndex<=0 && this->numThreads >= this->maxThreads ){
		pthread_mutex_lock(&this->threadLock);
		this->wqueue.push(sp);	
		pthread_mutex_unlock(&this->threadLock);
	}else{
		
		if(this->numThreads<=this->maxThreads){
		
			WorkerThread * thread = new WorkerThread();
			
			memset( &thread->wId, 0, sizeof( thread->wId ) );

			pthread_mutex_init( &thread->wLock, NULL );
			pthread_cond_init( &thread->wCond, NULL );
			thread->cursession = sp;
			pthread_attr_init( &attr );
			//pthread_attr_setdetachstate( &attr, SP_THREAD_CREATE_DETACHED );
			if( 0 == pthread_create( &( thread->wId ), &attr, waitforit, thread ) ) {
				
				this->numThreads++;
			} else {
				std::cout << "Unable to create thread" << std::endl;
				ret = -1;
				pthread_mutex_destroy( &thread->wLock );
				pthread_cond_destroy( &thread->wCond );
				free( thread );
			}
			pthread_attr_destroy( &attr );
		} 
	}
	pthread_mutex_unlock( &this->threadLock );
	
}

int ThreadPool::saveThread(WorkerThread *thread){
	
	int ret = 0;
	
pthread_mutex_lock( &this->threadLock );
	char buf = 0;
	if(this->finished==1) return -1;
	if(!this->wk.count(thread->wId)){
		std::map<pthread_t, WorkerThread*>::iterator it;
		for ( it=this->wk.begin() ; it != this->wk.end(); it++ )
		{
			cout << "it key= " << it->first << " and sec= " << it->second->cursession->data << "\n";
		}
		return -1;
	}
	this->wk[thread->wId]->cursession->ready=1;
	write(this->wakeup_pipe_in, &buf, 1);
	pthread_mutex_unlock( &this->threadLock );
	return ret;
}

