#include "api/api.h"
#include "processing/processing.h"
#include "alert/alert.h"
#include "utils/utils.h"

int main() {
    char *api_response = NULL;

    // Fetch data from API
    if (fetch_data(&api_response) != 0) {
        printf("Failed to fetch data\n");
        return 1;
    }

    // Save raw data
    if (save_raw_data(api_response) != 0) {
        printf("Failed to save raw data\n");
        free(api_response);
        return 1;
    }

    // Parse and process data
    SensorData data;
    if (parse_data(api_response, &data) != 0) {
        printf("Failed to parse data\n");
        free(api_response);
        return 1;
    }

    if (process_data(&data) != 0) {
        printf("Failed to process data\n");
        free(api_response);
        return 1;
    }

    // Check thresholds and send alerts if necessary
    check_thresholds(data.temperature, data.humidity);

    // Free allocated memory
    free(api_response);

    return 0;
}
