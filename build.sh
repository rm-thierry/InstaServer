#!/bin/bash

mkdir -p build

archs=("x86_64" "arm" "aarch64" "mips" "riscv64")
success=0
failures=0

for arch in "${archs[@]}"; do
    echo "Compiling for architecture: $arch..."
    
    case $arch in
        x86_64)
            compiler="x86_64-linux-gnu-gcc"
            ;;
        arm)
            compiler="arm-linux-gnueabi-gcc"
            ;;
        aarch64)
            compiler="aarch64-linux-gnu-gcc"
            ;;
        mips)
            compiler="mips-linux-gnu-gcc"
            ;;
        riscv64)
            compiler="riscv64-linux-gnu-gcc"
            ;;
        *)
            echo "Unknown architecture: $arch"
            continue
            ;;
    esac

    $compiler -I./include \
        src/main.c \
        src/terminal.c \
        src/menu.c \
        src/server_setup.c \
        -o build/server_setup_$arch \
        -static \
        -Wall -Wextra

    if [ $? -eq 0 ]; then
        echo "Build for $arch complete! Binary: build/server_setup_$arch"
        chmod +x build/server_setup_$arch
        success=$((success + 1))
    else
        echo "Build for $arch failed!"
        failures=$((failures + 1))
    fi
done

echo "Build summary: $success succeeded, $failures failed."
if [ $failures -gt 0 ]; then
    exit 1
else
    exit 0
fi
