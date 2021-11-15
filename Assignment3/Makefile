GCC = g++
GCCFLAGS = -g

default: fs.o fs_ops.o
	$(GCC) $(GCCFLAGS) fs.o fs_ops.o -o FS

fs.o: fs.cpp
	$(GCC) $(GCCFLAGS) -c fs.cpp -o fs.o
fs_ops.o: fs_ops.cpp *.h
	$(GCC) $(GCCFLAGS) -c fs_ops.cpp -o fs_ops.o

clean:
	rm -f *.o FS