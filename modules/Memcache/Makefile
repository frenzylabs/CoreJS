MEMCACHE_DIR = /usr/local/libmemcache/lib/libmemcached.a
V8_DIR       = /usr/local/v8/libv8_g.dylib
V8_INC       = /usr/local/v8/include

CORE_DIR     = /Users/weslcope/Desktop/Fae/modules/Core/src
CORE_INC     = -I$(CORE_DIR) \
               -I$(CORE_DIR)/Headers \
               -I$(CORE_DIR)/System \
               -I$(CORE_DIR)/Env \
               -I$(CORE_DIR)/Directory \
               -I$(CORE_DIR)/File \
               -I$(CORE_DIR)/Path \
               -I$(CORE_DIR)/Module
               

SOURCES =   memcache.cpp

         
HEADERS =   -I$(V8_INC) \
            -I$(CORE_INC) \
            -I.\
            $(CORE_INC)

LIBS = $(V8_DIR) $(MEMCACHE_DIR)
      
      
         
TARGET = memcache.fm

OBJS = $(SOURCES:.cpp=.o)
CC=g++
CXXFLAGS= -arch i386 -Wall -lpthread -shared -rdynamic

all: $(OBSJ) 
	$(CC) $(CXXFLAGS) $(HEADERS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	rm -rf memcache.fm
