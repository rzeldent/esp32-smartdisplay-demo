#include <Arduino.h>

#include <esp32_smartdisplay.h>
#include <ui/ui.h>

#define SPEAKER_PWM_CHANNEL 0

void OnButtonClicked(lv_event_t *e)
{
    static uint8_t cnt = 0;
    cnt++;
    lv_label_set_text_fmt(ui_lblCountValue, "%d", cnt);
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)", ESP.getChipModel(), ESP.getChipRevision(), getCpuFrequencyMhz(), ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("SDK version: %s", ESP.getSdkVersion());

#ifdef HAS_SPEAKER
    ledcAttachPin(SPEAKER_PIN, SPEAKER_PWM_CHANNEL);
#endif
    smartdisplay_init();
    ui_init();
}

void loop()
{
    char text_buffer[32];
    sprintf(text_buffer, "%d", millis());
    lv_label_set_text(ui_lblMillisecondsValue, text_buffer);

    auto const now = millis();
#ifdef HAS_RGB_LED
    auto const rgb = (now / 2000) % 8;
    digitalWrite(LED_PIN_R, !(rgb & 0x01));
    digitalWrite(LED_PIN_G, !(rgb & 0x02));
    digitalWrite(LED_PIN_B, !(rgb & 0x04));
#endif

#ifdef HAS_LIGHTSENSOR
    auto cdr = analogRead(LIGHTSENSOR_IN);
    sprintf(text_buffer, "%d", cdr);
    lv_label_set_text(ui_lblCdrValue, text_buffer);
#endif

#ifdef HAS_SPEAKER
    if ((now % 1000) == 0)
    {
        ledcWriteTone(SPEAKER_PWM_CHANNEL, 1000);
        delay(10);
        ledcWriteTone(SPEAKER_PWM_CHANNEL, 0);
    }
#endif

    lv_timer_handler();
}