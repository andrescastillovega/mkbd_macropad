if(CONFIG_ZMK_DISPLAY)
  zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
  zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM custom_status_screen.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM battery_img.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM usb_img.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_WIDGET_BATTERY_ANIM widgets/battery_anim.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_WIDGET_BATTERY_ANIM battery_level_images.c)
endif()

# Timer logger module
# zephyr_library_sources_ifdef(CONFIG_ZMK_TIMER_LOGGER timer_logger.c)