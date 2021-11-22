GCC = g++
GCCFLAGS = -g

default: shell.o fs_ops.o
	$(GCC) $(GCCFLAGS) shell.o fs_ops.o -o FS

fs.o: shell.cpp
	$(GCC) $(GCCFLAGS) -c shell.cpp -o shell.o
fs_ops.o: fs_ops.cpp *.h
	$(GCC) $(GCCFLAGS) -c fs_ops.cpp -o fs_ops.o

clean:
	rm -f *.o FS