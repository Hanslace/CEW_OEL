CC = gcc
CFLAGS = -Wall -I/usr/include -lcurl -lm -I/usr/include/cjson
LIBS = -lcurl -lcjson

SRC_DIR = src
API_DIR = $(SRC_DIR)/api
PROCESSING_DIR = $(SRC_DIR)/processing
ALERT_DIR = $(SRC_DIR)/alert
UTILS_DIR = $(SRC_DIR)/utils

SOURCES = $(SRC_DIR)/main.c \
          $(API_DIR)/api.c \
          $(PROCESSING_DIR)/processing.c \
          $(ALERT_DIR)/alert.c \
          $(UTILS_DIR)/utils.c
		  
OBJECTS = $(SOURCES:.c=.o)

TARGET = monitoring_system

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
