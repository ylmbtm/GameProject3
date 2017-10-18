CC=g++ -std=c++11 
CC_INCLUDE=-I/usr/include
	 
MY_INCLUDE=-I$(PWD) \
		-I../ServerEngine \
		-I../../Common 
					 
CC_LIB=-L/usr/lib \
	-L/lib \
	-lpthread -lprotobuf
			 
MY_LIB=-L../Debug \
	-L../ServerEngine \
	-lServerEngine

TARGET=GameServer

SRCS=$(wildcard ./*.cpp)
SRCS:=$(SRCS) $(wildcard ./GameObject/*.cpp)
SRCS:=$(SRCS) $(wildcard ../Message/*.cc)

OBJS=$(SRCS:.cpp=.o)

$(TARGET):$(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(CC_LIB) $(MY_LIB)
	mv $(OBJS) ../Linux

$(OBJS):%.o:%.cpp
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)
	
clean:
	rm $(OBJS) $(TARGET)
