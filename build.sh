#!/bin/bash

mkdir -p build

echo "Compiling source files..."
x86_64-linux-musl-gcc -I./include \
    src/main.c \
    src/terminal.c \
    src/menu.c \
    src/server_setup.c \
    -o build/instaserver_x86_64 \
    -static \
    -Wall -Wextra

if [ $? -eq 0 ]; then
    echo "Build complete! The binary 'instaserver_x86_64' is ready in the build directory"
    chmod +x build/instaserver_x86_64
else
    echo "Build failed!"
    exit 1
fi