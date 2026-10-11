cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app)

target_sources(app PRIVATE src/main.cpp)

if(CONFIG_SHELL)
  target_sources(app PRIVATE src/sensor_shell.c)
endif()

if(CONFIG_OUR_LED_SENSOR)
  target_sources(app PRIVATE drivers/our_driver/our_driver.c)
endif()

target_include_directories(app PRIVATE include)