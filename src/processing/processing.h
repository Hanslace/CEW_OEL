#ifndef PROCESSING_H
#define PROCESSING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON.h"
#include <sys/stat.h>

typedef struct {
    float temperature;
    float humidity;
    char timestamp[20]; // YYYY-MM-DD HH:MM:SS
    char weather[50];
    float pressure;
    float visibility;
    float wind_speed;

} SensorData;

int parse_data(const char *json_data, SensorData *data);
int process_data(SensorData *data);

#endif // PROCESSING_H
