CC = gcc
OBJs = 
CFLAGS = -ansi -pedantic-errors -Wall -Wextra
DEBUG_FLAGS = -g
RELEASE_FLAGS = -DNDEBUG -O3
TARGET = watchdog
IDIR = ./include
DEBUG_PATH = ./bin/debug
RELEASE_PATH = ./bin/release
SRC_PATH = ./src
TEST_PATH = ./test
VLG_FLAGS = --leak-check=yes --track-origins=yes -s

.PHONY: debug release all clean run vlg gdb

debug: 
	gcc -ansi -pedantic-errors -Wall -Wextra -pthread -I ./include/ src/watchdog.c src/scheduler.c src/pqueue.c src/sortlist.c src/dlist.c src/task.c src/uid.c test/watchdog_test.c -o bin/debug/watchdog_test.out
	gcc -ansi -pedantic-errors -Wall -Wextra -pthread -I ./include/ src/watchdog_op.c src/scheduler.c src/pqueue.c src/sortlist.c src/dlist.c src/task.c src/uid.c src/watchdog.c -o bin/debug/watchdog_op.out

$(DEBUG_PATH)/$(TARGET).out: $(TARGET).o $(TARGET)_test.o
	$(CC) $(TARGET).o $(TARGET)_test.o -o $(DEBUG_PATH)/$(TARGET).out 

$(TARGET).o: $(SRC_PATH)/$(TARGET).c $(IDIR)/$(TARGET).h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c -I$(IDIR) $(SRC_PATH)/$(TARGET).c 

$(TARGET)_test.o: $(TEST_PATH)/$(TARGET)_test.c $(IDIR)/$(TARGET).h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c -I$(IDIR) $(TEST_PATH)/$(TARGET)_test.c

$(RELEASE_PATH)/$(TARGET).out: $(OBJs) $(IDIR)/$(TARGET).h
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -I$(IDIR) -o $(RELEASE_PATH)/$(TARGET).out $(OBJs) 
	
#debug: $(DEBUG_PATH)/$(TARGET).out

release: $(RELEASE_PATH)/$(TARGET).out
	
all: debug release

run: 
	$(DEBUG_PATH)/$(TARGET).out
	
vlg: 	
	valgrind $(VLG_FLAGS) $(DEBUG_PATH)/$(TARGET).out

cgdb:
	cgdb $(DEBUG_PATH)/$(TARGET).out	
	
#clean:
#	rm ./*.o 
#	rm $(DEBUG_PATH)/$(TARGET)* 
#	rm $(RELEASE_PATH)/$(TARGET)* 

