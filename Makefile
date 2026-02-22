CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJS = QueueSimulation.o

project2 : ${OBJS}
	${CXX} ${CXXFLAGS} -o $@ ${OBJS}

QueueSimulation.o : Event.h Customer.h
	${CXX} ${CXXFLAGS} -c QueueSimulation.cpp

clean:
	rm -f *.o
	rm -f project2