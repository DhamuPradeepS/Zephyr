cmake_minimum_required(VERSION 3.20.0)


set(BOARD milkv_duo256m)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(
    EmptyApp
    VERSION 0.1
    DESCRIPTION "zephyr application for hardare interrupt."
    LANGUAGES C
)

target_sources(
    app
    PRIVATE
    src/main.c
)
