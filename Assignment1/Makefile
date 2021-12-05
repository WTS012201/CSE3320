GCC = gcc
GCCFLAGS += -g

default: myshell.o file_ops.o
	$(GCC) $(GCCFLAGS) myshell.o file_ops.o -o myshell

myshell.o: myshell.c
	$(GCC) $(GCCFLAGS) -c myshell.c -o myshell.o
file_ops.o: file_ops.c *.h
	$(GCC) $(GCCFLAGS) -c file_ops.c -o file_ops.o

clean:
	rm -f *.o myshell .editor