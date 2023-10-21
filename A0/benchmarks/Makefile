CC = gcc
CFLAGS = -g -w

all:: parallel_cal vector_multiply external_cal bm1 test

parallel_cal:
	$(CC) $(CFLAGS) -pthread -o parallel_cal parallel_cal.c -L../ -lmypthread

vector_multiply:
	$(CC) $(CFLAGS) -pthread -o vector_multiply vector_multiply.c -L../ -lmypthread

external_cal:
	$(CC) $(CFLAGS) -pthread -o external_cal external_cal.c -L../ -lmypthread

bm1:
	$(CC) $(CFLAGS) -pthread -o bm1 benchmark1.c -L../ -lmypthread

test:
	$(CC) $(CFLAGS) -pthread -o test test.c -L../ -lmypthread

clean:
	rm -rf testcase test parallel_cal vector_multiply external_cal bm1 *.o
