SET(SOURCES screens/ui_scrMain.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_lena80x80_png.c)

add_library(ui ${SOURCES})
