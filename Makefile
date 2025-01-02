UNAME_S := $(shell uname -s)

CFLAGS_LINUX = -pthread
CFLAGS_WIN = -lpthread

SRC_SERVER = server.c
SRC_CLIENT = client.c

TARGET_SERVER = server
TARGET_CLIENT = client

ifeq ($(UNAME_S), Linux)
    CFLAGS = $(CFLAGS_LINUX)
else
    CFLAGS = $(CFLAGS_WIN)
endif

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(SRC_SERVER)
	$(CC) $(SRC_SERVER) -o $(TARGET_SERVER) $(CFLAGS)

$(TARGET_CLIENT): $(SRC_CLIENT)
	$(CC) $(SRC_CLIENT) -o $(TARGET_CLIENT) $(CFLAGS)

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

.PHONY: all clean
