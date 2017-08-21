CC=g++
CC_INCLUDE=-I/usr/include
	 
MY_INCLUDE=-I$(PWD) \
		-I../ServerEngine \
		-I../../Common \
		-I../../Common/Mysql \
		-I../../Common/protobuf/src
					 
CC_LIB=-L/usr/lib \
	   -L/usr/lib64\mysql \
	-L/lib \
	-L/usr/lib/mysql \
	-lpthread
			 
MY_LIB=-L../Debug \
	-lmysqlclient \
	-lServerEngine \
	-lsqlite3

TARGET=../Linux/DBServer

SRCS=$(wildcard *.cpp)
SRCS:=$(SRCS) $(wildcard ../../Common/Sqlite/*.cpp)

OBJS=$(SRCS:.cpp=.o)

$(TARGET):$(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(CC_LIB) $(MY_LIB)
	mv $(OBJS) ../Linux

$(OBJS):%.o:%.cpp
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)
	
clean:
	rm $(OBJS) $(TARGET)