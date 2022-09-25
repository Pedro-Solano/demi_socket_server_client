export HD ?= $(HOME)/demikernel
export INCDIR ?= $(HD)/include
export BUILD_DIR := $(HD)/target/release

export DEMIKERNEL_LIB := $(BUILD_DIR)/libdemikernel.so
export LIBS := $(DEMIKERNEL_LIB)

export CC := gcc
export CFLAGS := -Wall -Wextra -O3 -I $(INCDIR) -std=c99

all:

	$(CC) $(CFLAGS) client.c -o client $(LIBS)
	$(CC) $(CFLAGS) client.c -o client $(LIBS)

clean:
	rm client server
