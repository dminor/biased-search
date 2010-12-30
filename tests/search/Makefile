
INCS = -I../../include 
LIBS = 
CFLAGS = -g -O2
LDFLAGS = -L../../bin 
OBJS = search.o
TARGET = ../../bin/search

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

search.o: ../../include/biased_treap.h

clean:
	rm *.o $(TARGET) 
