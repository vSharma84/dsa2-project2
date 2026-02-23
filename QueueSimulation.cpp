/***************************************************************
  Student Name: Vansh Sharma
  File Name: QueueSimulation.cpp
  Assignment number: 2

 Runs the queue simulation. Reads input, processes events, computes
 simulation statistics, and prints results alongside analytical values. 
***************************************************************/

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "Event.h"
#include "Customer.h"

using namespace std;

/********************************************************************
Function: GetNextRandomInterval
Description: Generates a random exponential time interval.
Parameters: rate (double) – arrival or service rate.
Returns: double – generated time interval.
********************************************************************/
double GetNextRandomInterval(double rate) {
    double f = (double)rand() / RAND_MAX;
    if (f == 0) f = 0.00001;
    return -log(f) / rate;
}

/********************************************************************
Function: factorial
Description: Computes n! for analytical calculations.
Parameters: n (int) – value to compute factorial of.
Returns: double – factorial of n.
********************************************************************/
double factorial(int n) {
    double result = 1.0;
    for (int i = 1; i <= n; i++)
        result *= i;
    return result;
}

/********************************************************************
Function: runSimulation
Description: Runs the queue simulation using parameters from a file
             and prints analytical and simulation results.
Parameters: filename (string) – input file name.
Returns: nothing
********************************************************************/
void runSimulation(string filename) {

    double lambda, mu;
    int M;
    int numCustomers;

    ifstream file(filename);
    file >> lambda >> mu >> M >> numCustomers;

    priority_queue<Event, vector<Event>, EventCompare> pq;
    queue<Customer*> fifo;

    int freeServers = M;
    int servedCount = 0;
    int arrivalsCreated = 0;
    double currentTime = 0.0;
    double lastArrivalTime = 0.0;
    double totalWaitTime = 0.0;
    double totalServiceTime = 0.0;
    int customerWaitedCnt = 0;
    double idleTime = 0.0;
    bool systemIsIdle = true;
    double idleStartTime = 0.0;

    lastArrivalTime = GetNextRandomInterval(lambda);
    Customer* firstCust = new Customer(lastArrivalTime);
    pq.push(Event(lastArrivalTime, ARRIVAL, firstCust));
    arrivalsCreated = 1;

    while (!pq.empty() && servedCount < numCustomers) {

        Event e = pq.top();
        pq.pop();

        currentTime = e.eventTime;
        Customer* cust = e.cust;

        if (e.type == ARRIVAL) {

            if (systemIsIdle) {
                idleTime += (currentTime - idleStartTime);
                systemIsIdle = false;
            }

            if (freeServers > 0) {

                freeServers--;
                cust->startOfServiceTime = currentTime;

                double serviceInterval = GetNextRandomInterval(mu);
                cust->departureTime = currentTime + serviceInterval;

                totalServiceTime += serviceInterval;

                pq.push(Event(cust->departureTime, DEPARTURE, cust));
            }
            else {
                fifo.push(cust);
            }

            if (arrivalsCreated < numCustomers) {
                lastArrivalTime += GetNextRandomInterval(lambda);
                Customer* newCust = new Customer(lastArrivalTime);
                pq.push(Event(lastArrivalTime, ARRIVAL, newCust));
                arrivalsCreated++;
            }
        }
        else {

            servedCount++;
            freeServers++;

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

                pq.push(Event(nextCust->departureTime,
                              DEPARTURE,
                              nextCust));

                freeServers--;
            }

            if (freeServers == M && fifo.empty()) {
                systemIsIdle = true;
                idleStartTime = currentTime;
            }
        }
    }

    double simDuration = currentTime;
    double W_sim  = (totalWaitTime + totalServiceTime) / numCustomers;
    double Wq_sim = totalWaitTime / numCustomers;
    double rho_sim = totalServiceTime / (M * simDuration);
    double Po_sim  = idleTime / simDuration;
    double probWait_sim = (double)customerWaitedCnt / numCustomers;

    double rho = lambda / (M * mu);

    double sum = 0.0;
    for (int i = 0; i <= M - 1; i++) {
        sum += pow(lambda / mu, i) / factorial(i);
    }

    double lastTerm = pow(lambda / mu, M) / (factorial(M) * (1 - rho));
    double Po = 1.0 / (sum + lastTerm);
    double Lq = (Po * pow(lambda / mu, M) * rho) / (factorial(M) * pow(1 - rho, 2));
    double L = Lq + (lambda / mu);
    double Wq = Lq / lambda;
    double W  = Wq + (1.0 / mu);
    

    cout << fixed << setprecision(4);

    cout << "Results for " << filename << ": " << endl;
    cout << endl;
    cout << "Analytical Model";
    cout << endl;
    cout << "Po:  " << Po << endl;
    cout << "L:   " << L << endl;
    cout << "W:   " << W << endl;
    cout << "Lq:  " << Lq << endl;
    cout << "Wq:  " << Wq << endl;
    cout << "rho: " << rho << endl;

    cout << endl;
    cout << "Simulation Results";
    cout << endl;
    cout << "Po:  " << Po_sim << endl;
    cout << "W:   " << W_sim << endl;
    cout << "Wq:  " << Wq_sim << endl;
    cout << "rho: " << rho_sim << endl;
    cout << "Probability(wait): " << probWait_sim << endl;
    cout << endl;
}

/********************************************************************
Function: main
Description: Starts the program and runs the simulation for both tests.
Parameters: none
Returns: int – program status.
********************************************************************/
int main() {
    srand(time(0));
    runSimulation("test1.txt");
    runSimulation("test2.txt");
}