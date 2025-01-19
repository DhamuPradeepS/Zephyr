cmake_minimum_required(VERSION 3.20.0)

# set(BOARD qemu_x86)
# set(BOARD qemu_cortex_m3)
set(BOARD qemu_riscv64)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(
    App
    VERSION 0.1
    DESCRIPTION "Zephyr application for threads."
    LANGUAGES C
)

target_sources(
    app
    PRIVATE
    src/main.c
)
