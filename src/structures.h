// DHTMessage.h
#ifndef STRUCTURES_H
#define STRUCTURES_H

struct DHTReadings {
    float temperature;
    float humidity;
};

struct Message {
    int seq_n;
    int id;
    bool enable;
    DHTReadings dht11;
};

#endif // STRUCTURES_H
