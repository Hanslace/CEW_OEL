#include "processing.h"

int parse_data(const char *json_data, SensorData *data) {
    cJSON *json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Error parsing JSON data\n");
        return 1;
    }

    // Parse main object
    cJSON *main = cJSON_GetObjectItemCaseSensitive(json, "main");
    if (main == NULL) {
        printf("Error parsing 'main' object\n");
        cJSON_Delete(json);
        return 1;
    }

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(main, "temp");
    cJSON *humidity = cJSON_GetObjectItemCaseSensitive(main, "humidity");
    cJSON *pressure = cJSON_GetObjectItemCaseSensitive(main, "pressure");

    if (!cJSON_IsNumber(temp) || !cJSON_IsNumber(humidity) || !cJSON_IsNumber(pressure)) {
        printf("Invalid data types in 'main' object\n");
        cJSON_Delete(json);
        return 1;
    }

    data->temperature = temp->valuedouble;
    data->humidity = humidity->valuedouble;
    data->pressure = pressure->valuedouble;

    // Parse visibility
    cJSON *visibility = cJSON_GetObjectItemCaseSensitive(json, "visibility");
    if (!cJSON_IsNumber(visibility)) {
        printf("Invalid data type for 'visibility'\n");
        cJSON_Delete(json);
        return 1;
    }

    data->visibility = visibility->valuedouble;

    // Parse wind object
    cJSON *wind = cJSON_GetObjectItemCaseSensitive(json, "wind");
    if (wind == NULL) {
        printf("Error parsing 'wind' object\n");
        cJSON_Delete(json);
        return 1;
    }

    cJSON *wind_speed = cJSON_GetObjectItemCaseSensitive(wind, "speed");
    if (!cJSON_IsNumber(wind_speed)) {
        printf("Invalid data type for 'wind_speed'\n");
        cJSON_Delete(json);
        return 1;
    }

    data->wind_speed = wind_speed->valuedouble;

    // Parse weather object
    cJSON *weather_array = cJSON_GetObjectItemCaseSensitive(json, "weather");
    if (!cJSON_IsArray(weather_array)) {
        printf("Error parsing 'weather' array\n");
        cJSON_Delete(json);
        return 1;
    }

    cJSON *weather_item = cJSON_GetArrayItem(weather_array, 0);
    if (weather_item == NULL) {
        printf("Error retrieving weather object\n");
        cJSON_Delete(json);
        return 1;
    }

    cJSON *weather_desc = cJSON_GetObjectItemCaseSensitive(weather_item, "description");
    if (!cJSON_IsString(weather_desc)) {
        printf("Invalid data type for 'weather description'\n");
        cJSON_Delete(json);
        return 1;
    }

    strncpy(data->weather, weather_desc->valuestring, sizeof(data->weather) - 1);
    data->weather[sizeof(data->weather) - 1] = '\0';

    // Get current timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(data->timestamp, sizeof(data->timestamp), "%Y-%m-%d %H:%M:%S", t);

    cJSON_Delete(json);
    return 0;
}


int process_data(SensorData *data) {
    // Ensure the output directory exists (if needed)
    const char *directory = "data";
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0700);
    }

    // Write processed data to a CSV file
    FILE *fp = fopen("data/processed_data.csv", "a");
    if (fp == NULL) {
        printf("Error opening processed data file\n");
        return 1;
    }

    // Write header if the file is empty
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (file_size == 0) {
        fprintf(fp, "Timestamp,Temperature,Humidity,Pressure,Visibility,Wind Speed,Weather Description\n");
    }

    // Write the data
    fprintf(fp, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,\"%s\"\n",
            data->timestamp,
            data->temperature,
            data->humidity,
            data->pressure,
            data->visibility,
            data->wind_speed,
            data->weather);

    fclose(fp);
    return 0;
}
