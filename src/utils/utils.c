#include "utils.h"

int load_env_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening .env file");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Ignore comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;

        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Split into key and value
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");
        if (key && value) {
            // Set environment variable
            setenv(key, value, 1);
        }
    }

    fclose(file);
    return 0;
}

int save_raw_data(const char *data) {
    FILE *fp;
    long file_size;
    int is_empty = 0;

    // Open the file in read/update mode
    fp = fopen("data/raw_data.json", "r+");
    if (fp == NULL) {
        // File doesn't exist; create a new one
        fp = fopen("data/raw_data.json", "w");
        if (fp == NULL) {
            printf("Error opening raw data file\n");
            return 1;
        }
        fprintf(fp, "[\n%s\n]", data); // Start a new JSON array
        fclose(fp);
        return 0;
    }

    // Check if file is empty
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    if (file_size <= 2) { // Account for '[' and ']' or empty file
        is_empty = 1;
    }

    if (is_empty) {
        // File is empty or contains only '[' and ']'
        fseek(fp, 0, SEEK_SET);
        fprintf(fp, "[\n%s\n]", data);
    } else {
        // Move back before the closing ']'
        fseek(fp, -2, SEEK_END);
        fprintf(fp, ",\n%s\n]", data);
    }

    fclose(fp);
    return 0;
}
