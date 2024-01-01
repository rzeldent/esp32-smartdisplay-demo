#include <Arduino.h>

#include <esp32_smartdisplay.h>
#include <ui/ui.h>
#include <Audio.h>

#define WIFI_SSID "<your ssid>"
#define WIFI_PASSWORD "<your ap password>"
#define RADIO_URL "http://www.wdr.de/wdrlive/media/einslive.m3u"

Audio *audio;

void OnButtonClicked(lv_event_t *e)
{
    static uint8_t cnt = 0;
    cnt++;
    lv_label_set_text_fmt(ui_lblCountValue, "%d", cnt);
}

void setup()
{
    delay(250);
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)", ESP.getChipModel(), ESP.getChipRevision(), getCpuFrequencyMhz(), ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("Free PSRAM: %d bytes", ESP.getPsramSize());
    log_i("SDK version: %s", ESP.getSdkVersion());

#ifdef BOARD_HAS_SPEAK
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() == WL_CONNECTED)
    {
        audio = new Audio(true, I2S_DAC_CHANNEL_LEFT_EN);
        audio->forceMono(true);
        audio->setVolume(10);

        while (!audio->connecttohost(RADIO_URL))
            delay(500);
    }
#endif

    smartdisplay_init();

    auto disp = lv_disp_get_default();
    lv_disp_set_rotation(disp, LV_DISP_ROT_90);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_270);

    ui_init();
}

ulong next_millis;

void loop()
{
#ifdef BOARD_HAS_SPEAK
    if (audio)
        audio->loop();
#endif

    auto const now = millis();
    if (now > next_millis)
    {
        next_millis = now + 500;

        char text_buffer[32];
        sprintf(text_buffer, "%d", now);
        lv_label_set_text(ui_lblMillisecondsValue, text_buffer);

#ifdef BOARD_HAS_RGB_LED
        auto const rgb = (now / 2000) % 8;
        smartdisplay_led_set_rgb(rgb & 0x01, rgb & 0x02, rgb & 0x04);
#endif

#ifdef BOARD_HAS_CDS
        auto cdr = analogReadMilliVolts(CDS);
        sprintf(text_buffer, "%d", cdr);
        lv_label_set_text(ui_lblCdrValue, text_buffer);
#endif
    }

    lv_timer_handler();
}