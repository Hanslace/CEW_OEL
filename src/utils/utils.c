#include "utils.h"

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
