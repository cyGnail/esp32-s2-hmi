set(COMPONENT_SRCS  "main.c"
                    "lv_2048.c"
                    "lv_2048_setting.c"
                    "mouse_cursor_icon.c"
                    )

set(COMPONENT_ADD_INCLUDEDIRS "include")

register_component()

spiffs_create_partition_image(storage ../spiffs_image FLASH_IN_PROJECT)