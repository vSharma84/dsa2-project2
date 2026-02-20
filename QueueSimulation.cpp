#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Event.h"
#include "Customer.h"
#include "MinHeap.h"

using namespace std;

double GetNextRandomInterval(double avg) {
    double f = (double)rand() / RAND_MAX;
    if (f == 0) f = 0.00001;
    return - (1.0 / avg) * log(f);
}

int main() {

    srand(time(0));

    double lambda, mu;
    int M;
    int numCustomers;

    ifstream file("test1.txt");
    file >> lambda;
    file >> mu;
    file >> M;
    file >> numCustomers;
    file.close();

    MinHeap pq(200);
    queue<Customer*> fifo;

    int serverAvailableCnt = M;
    int customersServed = 0;

    double currentTime = 0.0;

    double totalWaitTime = 0.0;
    double totalServiceTime = 0.0;
    int customerWaitedCnt = 0;

    bool moreArrivals = true;

    double firstArrivalTime = GetNextRandomInterval(lambda);
    Customer* firstCust = new Customer(firstArrivalTime);
    pq.insert(Event(firstArrivalTime, ARRIVAL, firstCust));

    while (!pq.isEmpty() && customersServed < numCustomers) {

        Event e = pq.deleteMin();
        currentTime = e.eventTime;
        Customer* cust = e.cust;

        if (e.type == ARRIVAL) {

            if (serverAvailableCnt > 0) {

                serverAvailableCnt--;

                cust->startOfServiceTime = currentTime;

                double interval = GetNextRandomInterval(mu);
                cust->departureTime = currentTime + interval;

                totalServiceTime += interval;

                pq.insert(Event(cust->departureTime, DEPARTURE, cust));
            }
            else {
                fifo.push(cust);
            }
        }
        else {

            customersServed++;
            serverAvailableCnt++;

            double waitTime =
                cust->startOfServiceTime - cust->arrivalTime;

            if (waitTime > 0) {
                customerWaitedCnt++;
                totalWaitTime += waitTime;
            }

            if (!fifo.empty()) {

                Customer* nextCust = fifo.front();
                fifo.pop();

                serverAvailableCnt--;

                nextCust->startOfServiceTime = currentTime;

                double interval = GetNextRandomInterval(mu);
                nextCust->departureTime = currentTime + interval;

                totalServiceTime += interval;

                pq.insert(Event(nextCust->departureTime,
                                DEPARTURE,
                                nextCust));
            }
        }

        if (moreArrivals && pq.getSize() <= M + 1) {

            double nextArrivalTime =
                currentTime + GetNextRandomInterval(lambda);

            Customer* newCust =
                new Customer(nextArrivalTime);

            pq.insert(Event(nextArrivalTime,
                            ARRIVAL,
                            newCust));
        }
    }

    cout << "Simulation Complete." << endl;

    return 0;
}
