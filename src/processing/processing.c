#include "processing.h"

int parse_data(const char *json_data, SensorData *data) {
    cJSON *json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Error parsing JSON data\n");
        return 1;
    }

    cJSON *main = cJSON_GetObjectItemCaseSensitive(json, "main");
    if (main == NULL) {
        printf("Error parsing 'main' object\n");
        cJSON_Delete(json);
        return 1;
    }

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(main, "temp");
    cJSON *humidity = cJSON_GetObjectItemCaseSensitive(main, "humidity");

    if (!cJSON_IsNumber(temp) || !cJSON_IsNumber(humidity)) {
        printf("Invalid data types\n");
        cJSON_Delete(json);
        return 1;
    }

    data->temperature = temp->valuedouble;
    data->humidity = humidity->valuedouble;

    // Get current timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(data->timestamp, sizeof(data->timestamp), "%Y-%m-%d %H:%M:%S", t);

    cJSON_Delete(json);
    return 0;
}

int process_data(SensorData *data) {
    // Write processed data to a CSV file
    FILE *fp = fopen("data/processed_data.csv", "a");
    if (fp == NULL) {
        printf("Error opening processed data file\n");
        return 1;
    }

    fprintf(fp, "%s,%.2f,%.2f\n", data->timestamp, data->temperature, data->humidity);
    fclose(fp);
    return 0;
}
