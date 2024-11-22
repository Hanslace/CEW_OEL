#ifndef API_H
#define API_H

#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

#define API_URL_BASE "http://api.openweathermap.org/data/2.5/weather"

struct MemoryStruct {
    char *memory;
    size_t size;
};

int fetch_data(char **response);

#endif // API_H
