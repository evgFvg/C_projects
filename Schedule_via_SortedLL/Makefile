
TARGET = sch
OBJs = dll.o sorted_list.o pq.o uid.o task.o scheduler.o scheduler_test.o
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -Iinclude
RELFLAGS = $(CFLAGS) -O3
DEBFLAGS = $(CFLAGS) -g
CC = gcc


.PHONY: all  

all: $(TARGET)
sch_out: dll.o sorted_list.o pq.o uid.o task.o scheduler.o scheduler_test.o
	$(CC) $(CFLAGS) -o sch_out.out $^
	
dll.o:  src/dll.c include/dll.h
	$(CC) $(CFLAGS) -c src/dll.c
	
sorted_list.o: src/sorted_list.c include/sorted_list.h include/dll.h
	$(CC) $(CFLAGS) -c src/sorted_list.c src/dll.c
	
pq.o:   src/pq.c include/pq.h include/dll.h include/sorted_list.h
	$(CC) $(CFLAGS) -c src/pq.c src/dll.c src/sorted_list.c
	
uid.o:  src/uid.c include/uid.h 
	$(CC) $(CFLAGS) -c src/uid.c 
	
task.o: src/task.c include/task.h include/uid.h 
	$(CC) $(CFLAGS) -c src/task.c src/uid.c 
	
scheduler.o:   src/scheduler.c include/scheduler.h include/task.h include/uid.h include/dll.h include/sorted_list.h include/pq.h
	$(CC) $(CFLAGS) -c src/pq.c src/dll.c src/sorted_list.c src/uid.c src/task.c src/scheduler.c 
	
scheduler_test.o:   test/scheduler_test.c include/scheduler.h 
	$(CC) $(CFLAGS) -c test/scheduler_test.c src/scheduler.c src/uid.c src/task.c 
	
	
sch:    src/dll.c src/sorted_list.c src/pq.c src/uid.c src/task.c src/scheduler.c test/scheduler_test.c  
	$(CC) $(RELFLAGS) -o sch.out $^	


#use the dll.o target

sorted_list_release: sorted_list.o sorted_list_test.o dll.o
	$(CC) $(RELEASE) -o bin/release/sorted_list 
	
.PHONY release:
release: $(TARGET)
	$(CC) $(RELFLAGS) $(sorted_cfile) $(sorted_main) $(dll_cfile) -o debug.out

.PHONY debug:
debug:  $(TARGET)
	$(CC) $(DEBFLAGS) $(sorted_cfile) $(sorted_main) $(dll_cfile) -o release.out

.PHONY clean:	
clean:
	rm *.o *.out
