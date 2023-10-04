GD = gcc -c -Iinclude -g -pthread -fpic
GC = gcc -c -Iinclude -DNDEBUG -O3 -pthread -fpic 
RELEASE_CFLAGS = -Iinclude -DNDEBUG -O3 -pthread -Wall
DEBUG_CFLAGS = -Iinclude -g -O0 -pthread -Wall
SRC = src/*.c src/libwd.c

libwd.so : $(SRC)
	$(GC) $(SRC)
	gcc -shared -o libwd.so *.o

libwd_debug.so: $(SRC)
	$(GD) $(SRC)
	gcc -shared -o libwd.so *.o

user.out: test/user.c
	$(GC) test/user.c
	gcc $(RELEASE_CFLAGS) -o user.out test/user.c -L. -Wl,-rpath=. -lwd

user_debug.out: test/user.c
	$(GD) test/user.c
	gcc $(DEBUG_CFLAGS) -o user.out test/user.c -L. -Wl,-rpath=. -lwd

wd.out: src/wd_proc.c
	gcc $(RELEASE_CFLAGS) -o wd.out src/wd_proc.c -L. -Wl,-rpath=. -lwd

wd_debug.out: src/wd_proc.c
	gcc $(DEBUG_CFLAGS) -o wd.out src/wd_proc.c -L. -Wl,-rpath=. -lwd

release : 
	make libwd.so
	make user.out
	make wd.out

debug : 
	make libwd_debug.so
	make user_debug.out
	make wd_debug.out

.PHONY : clean
clean :
	rm -f *.so
	rm -f *.out
	rm -f *.o
