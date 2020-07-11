CC=g++ -std=c++11
CC_INCLUDE=-I/usr/include
	 
MY_INCLUDE=-I$(PWD) \
		-I../ServerEngine \
		-I../PublicData \
		-I../../Common 
					 
CC_LIB=-L/usr/lib \
	-L/lib \
	-lpthread -lprotobuf
			 
MY_LIB=-L../Debug \
	-L../Linux \
	-L../ServerEngine \
	-lServerEngine

TARGET=WatchServer

SRCS=$(wildcard *.cpp)
#SRCS:=$(SRCS) $(wildcard ../Message/*.cc)
SRCS2=$(wildcard ../Message/*.cc)

OBJS=$(SRCS:.cpp=.o)
OBJS2=$(SRCS2:.cc=.o)

OBJS:=$(OBJS) $(OBJS2)

default:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -g -o $@ $^ $(MY_LIB) $(CC_LIB)
	mv $(OBJS) ../Linux

#$(OBJS):%.o:%.cpp
#	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)

%.o:%.cpp
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)

%.o:%.cc
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)
	
clean:
	rm $(OBJS) $(TARGET)