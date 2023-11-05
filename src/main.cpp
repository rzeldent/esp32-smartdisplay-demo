#include <Arduino.h>

#include <esp32_smartdisplay.h>
#include <ui/ui.h>

void millisecond_display_update()
{
    char time_buffer[32];
    sprintf(time_buffer, "%d", millis());
    lv_label_set_text(ui_lblMillisecondsValue, time_buffer);
}

void OnButtonClicked(lv_event_t *e)
{
    static uint8_t cnt = 0;
    cnt++;
    lv_label_set_text_fmt(ui_lblCountValue, "%d", cnt);
}

void setup()
{
    Serial.begin(115200);

    smartdisplay_init();
    ui_init();
}

void loop()
{
    lv_timer_handler();

#ifdef HAS_RGB_LED
    auto r = (byte)(millis() / 75);
    auto g = (byte)(millis() / 10);
    auto b = (byte)(millis() / 150);
    smartdisplay_set_led_color(lv_color32_t({.ch = {.blue = b, .green = g, .red = r}}));
#endif
    millisecond_display_update();
}