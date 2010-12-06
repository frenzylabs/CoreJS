V8_DIR       = /usr/local/v8/libv8_g.dylib
V8_INC       = /usr/local/v8/include

SOURCES =   testmod.cpp

         
HEADERS =   -I$(V8_INC) \
            -I. \

LIBS = $(V8_DIR)
         
TARGET = testmod.fm

OBJS = $(SOURCES:.cpp=.o)
CC=g++
CXXFLAGS= -arch i386 -Wall -lpthread -shared -rdynamic

all: $(OBSJ) 
	$(CC) $(CXXFLAGS) $(HEADERS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	rm -rf testmod.fm
