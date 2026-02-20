#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "Event.h"
#include "Customer.h"
#include "MinHeap.h"

using namespace std;

double GetNextRandomInterval(double avg) {
    double f = (double)rand() / RAND_MAX;
    if (f == 0) f = 0.00001;
    return - (1.0 / avg) * log(f);
}

double factorial(int n) {
    double result = 1.0;
    for (int i = 1; i <= n; i++)
        result *= i;
    return result;
}

void runSimulation(string filename) {

    double lambda, mu;
    int M;
    int numCustomers;

    ifstream file(filename);
    if (!file) {
        cout << "Error opening " << filename << endl;
        return;
    }

    file >> lambda;
    file >> mu;
    file >> M;
    file >> numCustomers;
    file.close();

    MinHeap pq(200);
    queue<Customer*> fifo;

    int serverAvailableCnt = M;
    int customersServed = 0;
    int arrivalsGenerated = 0;
    bool moreArrivals = true;

    double currentTime = 0.0;

    double totalWaitTime = 0.0;
    double totalServiceTime = 0.0;
    int customerWaitedCnt = 0;

    double idleTime = 0.0;
    bool systemIsIdle = true;
    double idleStartTime = 0.0;

    double firstArrivalTime = GetNextRandomInterval(lambda);
    Customer* firstCust = new Customer(firstArrivalTime);
    pq.insert(Event(firstArrivalTime, ARRIVAL, firstCust));
    arrivalsGenerated = 1;

    while (!pq.isEmpty() && customersServed < numCustomers) {

        Event e = pq.deleteMin();
        currentTime = e.eventTime;
        Customer* cust = e.cust;

        if (e.type == ARRIVAL) {

            if (systemIsIdle) {
                idleTime += (currentTime - idleStartTime);
                systemIsIdle = false;
            }

            if (serverAvailableCnt > 0) {
                serverAvailableCnt--;
                cust->startOfServiceTime = currentTime;

                double serviceInterval = GetNextRandomInterval(mu);
                cust->departureTime = currentTime + serviceInterval;

                totalServiceTime += serviceInterval;

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

                nextCust->startOfServiceTime = currentTime;

                double serviceInterval = GetNextRandomInterval(mu);
                nextCust->departureTime = currentTime + serviceInterval;

                totalServiceTime += serviceInterval;

                pq.insert(Event(nextCust->departureTime,
                                DEPARTURE,
                                nextCust));

                serverAvailableCnt--;
            }

            if (serverAvailableCnt == M && fifo.empty()) {
                systemIsIdle = true;
                idleStartTime = currentTime;
            }
        }

        if (arrivalsGenerated >= numCustomers) {
            moreArrivals = false;
        }

        if (moreArrivals && pq.getSize() <= M + 1) {
            double nextArrivalTime =
                currentTime + GetNextRandomInterval(lambda);

            Customer* newCust =
                new Customer(nextArrivalTime);

            pq.insert(Event(nextArrivalTime,
                            ARRIVAL,
                            newCust));

            arrivalsGenerated++;
        }
    }

    double totalSimTime = currentTime;

    double W_sim  = (totalWaitTime + totalServiceTime) / numCustomers;
    double Wq_sim = totalWaitTime / numCustomers;

    double rho_sim = totalServiceTime / (M * totalSimTime);
    double Po_sim  = idleTime / totalSimTime;

    double probWait_sim =
        (double)customerWaitedCnt / numCustomers;

    double rho = lambda / (M * mu);

    double sum = 0.0;
    for (int i = 0; i <= M - 1; i++) {
        sum += pow(lambda / mu, i) / factorial(i);
    }

    double lastTerm =
        pow(lambda / mu, M) /
        (factorial(M) * (1 - rho));

    double Po = 1.0 / (sum + lastTerm);

    double Lq =
        (Po * pow(lambda / mu, M) * rho) /
        (factorial(M) * pow(1 - rho, 2));

    double L = Lq + (lambda / mu);

    double Wq = Lq / lambda;
    double W  = Wq + (1.0 / mu);

    cout << fixed << setprecision(4);

    cout << "\n======================================\n";
    cout << "Results for " << filename << endl;
    cout << "======================================\n";

    cout << "\nAnalytical Model\n";
    cout << "Po:  " << Po << endl;
    cout << "L:   " << L << endl;
    cout << "W:   " << W << endl;
    cout << "Lq:  " << Lq << endl;
    cout << "Wq:  " << Wq << endl;
    cout << "rho: " << rho << endl;

    cout << "\nSimulation Results\n";
    cout << "Po:  " << Po_sim << endl;
    cout << "W:   " << W_sim << endl;
    cout << "Wq:  " << Wq_sim << endl;
    cout << "rho: " << rho_sim << endl;
    cout << "Prob(wait): " << probWait_sim << endl;
}

int main() {
    srand(time(0));

    runSimulation("test1.txt");
    runSimulation("test2.txt");

    return 0;
}
