#include <Arduino.h>

#include <esp32_smartdisplay.h>

// LVGL Objects
static lv_obj_t *label_timer;
static lv_obj_t *label_some_text;

void display_update()
{
    char time_buffer[32];
    sprintf(time_buffer, "%d", millis());
    lv_label_set_text(label_timer, time_buffer);
}

void btn_event_cb(lv_event_t *e)
{
    auto code = lv_event_get_code(e);
    auto btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;

        auto label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void setup_mainscreen()
{
    // Clear screen
    lv_obj_clean(lv_scr_act());

    // Create a buttom
    auto btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
    // Set label to Button
    auto label = lv_label_create(btn);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    label_timer = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_timer, &lv_font_montserrat_22, LV_STATE_DEFAULT);
    lv_obj_align(label_timer, LV_ALIGN_BOTTOM_MID, 0, -50);

    label_some_text = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_some_text, &lv_font_montserrat_22, LV_STATE_DEFAULT);
    lv_obj_align(label_some_text, LV_ALIGN_BOTTOM_MID, 0, -80);
}

void setup()
{
    Serial.begin(115200);

    smartdisplay_init();
    setup_mainscreen();
}

void loop()
{
    auto r = (byte)(millis() / 75);
    auto g = (byte)(millis() / 10);
    auto b = (byte)(millis() / 150);

    smartdisplay_set_led_color(lv_color32_t({.ch = {.blue = b, .green = g, .red = r}}));

    display_update();
    lv_timer_handler();
}