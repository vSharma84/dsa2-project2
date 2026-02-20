#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer {
public:
    double arrivalTime;
    double startOfServiceTime;
    double departureTime;
    double pqTime;

    Customer* nextCust;

    Customer(double aTime) {
        arrivalTime = aTime;
        startOfServiceTime = 0;
        departureTime = 0;
        pqTime = 0;
        nextCust = nullptr;
    }
};

#endif
