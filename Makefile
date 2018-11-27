OBJS = main.o
CC = g++ -std=c++11 -stdlib=libc++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)

main: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o a

clean:
	rm -f *.o main a
