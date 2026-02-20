CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJS = QueueSimulation.o MinHeap.o

p : ${OBJS}
	${CXX} ${CXXFLAGS} -o $@ ${OBJS}

QueueSimulation.o : Event.h Customer.h MinHeap.h
	${CXX} ${CXXFLAGS} -c QueueSimulation.cpp

MinHeap.o : MinHeap.h Event.h
	${CXX} ${CXXFLAGS} -c MinHeap.cpp

clean:
	rm -f *.o
	rm -f p
