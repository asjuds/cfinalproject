CC = gcc
CFLAGS = -Wall -Wextra -g `pkg-config --cflags gtk4`
LDFLAGS = `pkg-config --libs gtk4`

SRC_DIR = src
SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/storage.c \
	$(SRC_DIR)/logic.c \
	$(SRC_DIR)/ui_main_window.c \
	$(SRC_DIR)/ui_dialogs.c

OBJS = $(SRCS:.c=.o)

TARGET = stock_manager

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean


