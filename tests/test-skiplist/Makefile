
INCS = -I../../include 
LIBS = 
CFLAGS = -g -O2 -Wall
LDFLAGS = -L../../bin 
OBJS = main.o 
TARGET = ../../bin/test-skiplist

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

main.o: ../../include/biased_skiplist.h

clean:
	rm *.o $(TARGET) 
