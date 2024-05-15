CXX = g++
CXXFLAGS = -Wall

p: mytest.cpp satnet.o 
	$(CXX) $(CXXFLAGS) mytest.cpp satnet.o -o proj2

satnet.o: satnet.h satnet.cpp
	$(CXX) $(CXXFLAGS) -c satnet.cpp

clean:
	rm *.o*
	rm *~ 

v:
	valgrind --leak-check=full --track-origins=yes ./proj2

r:
	./proj2

b:
	gdb ./proj2
