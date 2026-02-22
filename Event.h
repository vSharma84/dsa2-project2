#ifndef EVENT_H
#define EVENT_H

class Customer;

enum EventType { ARRIVAL, DEPARTURE };

class Event {
public:
    double eventTime;
    EventType type;
    Customer* cust;

    Event() {}
    Event(double t, EventType tp, Customer* c) {
        eventTime = t;
        type = tp;
        cust = c;
    }
};

struct EventCompare {
    bool operator()(const Event& a, const Event& b) const {
        return a.eventTime > b.eventTime;
    }
};

#endif