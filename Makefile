# Lara4 Root Engine v6.0 - Dopamine 3.0 Makefile
# Build: make
# Clean: make clean

CC      = clang
CFLAGS  = -arch arm64e -isysroot $(shell xcrun --sdk iphoneos --show-sdk-path) -O2 -fobjc-arc
CFLAGS += -Wall -Wextra -Wno-unused-function

# Dopamine 3.0 submodule paths
DOPAMINE_ROOT = External/Dopamine
TITAN_ROOT    = External/Titan

# Include paths
INCLUDES = -I$(DOPAMINE_ROOT)/BaseBin/libjailbreak            -I$(DOPAMINE_ROOT)/Application/Dopamine/Exploits            -I$(TITAN_ROOT)/include            -IExploits/Kernel -IExploits/PPL -IExploits/PAC            -IPrimitives -IJailbreakCore

# Source files
EXPLOIT_SRC = $(wildcard Exploits/*/*.c)
PRIM_SRC    = $(wildcard Primitives/*.c)
CORE_SRC    = $(wildcard JailbreakCore/*.c)

SRC = $(EXPLOIT_SRC) $(PRIM_SRC) $(CORE_SRC)

OBJS = $(SRC:.c=.o)

TARGET = lara4.dylib

.PHONY: all clean submodules

all: submodules $(TARGET)

submodules:
	@echo "[Lara4] Checking submodules..."
	@if [ ! -d "$(DOPAMINE_ROOT)/.git" ]; then 		echo "[!] Dopamine submodule missing. Run: git submodule update --init"; 	fi
	@if [ ! -d "$(TITAN_ROOT)/.git" ]; then 		echo "[!] Titan submodule missing. Run: git submodule update --init"; 	fi

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(INCLUDES) 		-framework IOKit -framework Foundation 		-lSystem
	@echo "[+] Built: $@"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "[+] Cleaned"

# Install to device (requires root)
install: $(TARGET)
	scp $(TARGET) root@iphone:/usr/lib/
	@echo "[+] Installed to device"
