#ifndef API_H
#define API_H

#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

#define API_KEY "7f164c9b76fe546457f5db19a69ca589"
#define CITY_NAME "Karachi"
#define API_URL "http://api.openweathermap.org/data/2.5/weather?q=" CITY_NAME "&appid=" API_KEY "&units=metric"

struct MemoryStruct {
    char *memory;
    size_t size;
};

int fetch_data(char **response);

#endif // API_H
