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
struct Date_gps{
    uint8_t day;
    uint8_t month;
    uint16_t year;

};
struct Time_gps{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};
struct GPSData{
    double lat;
    double lng;
    double speed;
    Date_gps date;
    Time_gps time;
};

#endif // STRUCTURES_H
