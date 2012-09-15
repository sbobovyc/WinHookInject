CC = g++
CFLAGS = -O0 -ggdb
all: libs bins

libs:
	${CC} ${CFLAGS} -c inject.cpp
	#${CC} -shared -o inject.dll inject.o -Wl,--out-implib,inject.a
	${CC} -shared -o inject.dll inject.o 
	${CC} ${CFLAGS} -c libutils.cpp
	#${CC} -shared -o libutils.dll libutils.o -Wl,--out-implib,libutils.a
	${CC} -shared -o libutils.dll libutils.o 

bins:	
	${CC} ${CFLAGS} libutils.cpp hooker.cpp -o hooker
	${CC} ${CFLAGS} -c hooker.cpp
	${CC} hooker.o -L. -llibutils -o hooker_linked 
	${CC} ${CFLAGS} load_test.cpp -o load_test
clean:
	rm -rf *.exe 
	rm -rf *.dll
	rm -rf *.a
	rm -rf *.o
    

