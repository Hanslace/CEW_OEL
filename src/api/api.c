#include "api.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        // Out of memory
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = '\0';

    return realsize;
}

int fetch_data(char **response) {
    CURL *curl_handle;
    CURLcode res;

    // Fetch API key and city name from environment variables
    const char *api_key = getenv("OPENWEATHER_API_KEY");
    const char *city_name = getenv("CITY_NAME");

    if (!api_key || !city_name) {
        fprintf(stderr, "Error: Missing API key or city name in environment variables\n");
        return 1;
    }

    // Dynamically construct the API URL
    char url[512];
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric", city_name, api_key);

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  // Will be grown as needed by realloc
    chunk.size = 0;            // No data at this point

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if (curl_handle) {
        // Set CURL options
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request
        res = curl_easy_perform(curl_handle);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl_handle);
            curl_global_cleanup();
            free(chunk.memory);
            return 1;
        }

        // Clean up
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();

        *response = chunk.memory;
        return 0;
    } else {
        fprintf(stderr, "Failed to initialize CURL\n");
        return 1;
    }
}
