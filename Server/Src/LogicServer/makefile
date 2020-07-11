CC=g++ -std=c++11
CC_INCLUDE=-I/usr/include
	 
MY_INCLUDE=-I$(PWD) \
		-I../ServerEngine \
		-I../PublicData \
		-I../ServerData \
		-I../StaticData \
		-I../WatcherClient \
		-I../../Common \
		-I../../Common/Mysql \
		-I../../Common/Sqlite  \
		-I../../Common/Lua	\
		-I../../Common/toluapp
		
CC_LIB=-L/usr/lib \
	-L/lib \
	-L/usr/lib64/mysql \
	-L/usr/lib/mysql \
	-lpthread -lprotobuf -lmysqlclient -lsqlite3 -llua -ldl
			 
MY_LIB=-L../Debug \
	-L../ServerEngine \
	-lServerEngine
	

TARGET=LogicServer

SRCS=$(wildcard *.cpp)
SRCS:=$(SRCS) $(wildcard ../StaticData/*.cpp)
SRCS:=$(SRCS) $(wildcard ../WatcherClient/*.cpp)
SRCS2=$(wildcard ../Message/*.cc)

OBJS=$(SRCS:.cpp=.o)
OBJS2=$(SRCS2:.cc=.o)

OBJS:=$(OBJS) $(OBJS2)

default:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -g -o $@ $^ $(MY_LIB) $(CC_LIB)
	mv $(OBJS) ../Linux

%.o:%.cpp
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)

%.o:%.cc
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)
	
clean:
	rm $(OBJS) $(TARGET)