
DIRS = search test-hashtable test-treap test-skiplist test-splaytree

all:
	for dir in $(DIRS); do cd $$dir; make; cd ..; done

clean:
	for dir in $(DIRS); do cd $$dir; make clean; cd ..; done

