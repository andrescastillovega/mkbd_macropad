if(CONFIG_ZMK_DISPLAY)
  zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
  zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM four_areas_screen.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_WIDGET_AREA widgets/s0_widget.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_WIDGET_AREA widgets/s1_widget.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_WIDGET_AREA widgets/s2_widget.c)
  zephyr_library_sources_ifdef(CONFIG_ZMK_WIDGET_AREA widgets/s3_widget.c)
endif()

# Timer logger module
# zephyr_library_sources_ifdef(CONFIG_ZMK_TIMER_LOGGER timer_logger.c)