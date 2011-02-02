
INCS = -I../../include 
LIBS = 
CFLAGS = -g -O2 -Wall
LDFLAGS = -L../../bin 
OBJS = main.o 
TARGET = ../../bin/test-splaytree

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

main.o: ../../include/splaytree.h

clean:
	rm *.o $(TARGET) 
