#include "common.h"

#ifndef event_Base_H
#define event_Base_H

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/event-config.h>
#include <event2/thread.h>

using namespace v8;

	class BaseEvent{
	
		static BaseEvent *bInstance;
		
		BaseEvent(){};
	
		public:
			~BaseEvent();
			static BaseEvent *instance()
			{
				if (!bInstance){
					bInstance = new BaseEvent();
					bInstance->base = event_base_new();
				}
				return bInstance;
			}
		
			event_base *base;
	
	};
	
#endif
