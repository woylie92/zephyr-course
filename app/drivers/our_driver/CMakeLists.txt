if(CONFIG_OUR_LED_SENSOR)
  zephyr_library()
  zephyr_library_sources(our_driver.c)
endif()