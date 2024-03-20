#include <Arduino.h>

#include <esp32_smartdisplay.h>
#include <ui/ui.h>

#include <src/extra/libs/qrcode/lv_qrcode.h>

#include <SD.h>
#include <hal/spi_types.h>
#include <driver/spi_common.h>
#include <driver/spi_master.h>
#include <sdmmc_cmd.h>
#include <driver/sdspi_host.h>
#include <esp_vfs_fat.h>
//#include "driver/spi_master.h"
//#include "esp32-hal-spi.h"

void OnAddOneClicked(lv_event_t *e)
{
    static uint8_t cnt = 0;
    cnt++;
    lv_label_set_text_fmt(ui_lblCountValue, "%d", cnt);
}

void OnRotateClicked(lv_event_t *e)
{
    auto disp = lv_disp_get_default();
    auto rotation = (lv_disp_rot_t)((lv_disp_get_rotation(disp) + 1) % (LV_DISP_ROT_270 + 1));
    lv_disp_set_rotation(disp, rotation);
}

void setup()
{
#ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(5000);
#endif
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    log_i("Board: %s", BOARD_NAME);
    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)", ESP.getChipModel(), ESP.getChipRevision(), getCpuFrequencyMhz(), ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("Free PSRAM: %d bytes", ESP.getPsramSize());
    log_i("SDK version: %s", ESP.getSdkVersion());

    smartdisplay_init();

    __attribute__((unused)) auto disp = lv_disp_get_default();
    // lv_disp_set_rotation(disp, LV_DISP_ROT_90);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_270);

    ui_init();

    // To use third party libraries, enable the define in lv_conf.h: #define LV_USE_QRCODE 1
    auto ui_qrcode = lv_qrcode_create(ui_scrMain, 100, lv_color_black(), lv_color_white());
    const char *qr_data = "https://github.com/rzeldent/esp32-smartdisplay";
    lv_qrcode_update(ui_qrcode, qr_data, strlen(qr_data));
    lv_obj_center(ui_qrcode);

#ifdef BOARD_HAS_TF

//HSPI_HOST
    // Configura i pin SPI per la comunicazione con la SD
     spi_bus_config_t buscfg = {
         .mosi_io_num = TF_SPI_MOSI,
         .miso_io_num = TF_SPI_MISO,
         .sclk_io_num = TF_SPI_SCLK,
         .quadwp_io_num = -1,
         .quadhd_io_num = -1,
         .max_transfer_sz = 4000,
     };
    // // Inizializza il bus SPI
    ESP_ERROR_CHECK(spi_bus_initialize(TF_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .mode = 0,                         // Modalità SPI
        .clock_speed_hz = SDMMC_FREQ_DEFAULT, // Velocità di clock SPI
        .spics_io_num = TF_CS,         // Pin del chip select
        .queue_size = 7,                   // Lunghezza della coda delle transazioni SPI
    };

    spi_device_handle_t spi;
    ESP_ERROR_CHECK(spi_bus_add_device(TF_SPI_HOST, &devcfg, &spi)); 

    sdspi_dev_handle_t sd_handle;
    sdspi_device_config_t dev_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    dev_config.host_id   = TF_SPI_HOST;
    dev_config.gpio_cs   = (gpio_num_t)TF_CS;

    ESP_ERROR_CHECK(sdspi_host_init_device(&dev_config, &sd_handle));

    sdmmc_host_t sd_host = SDSPI_HOST_DEFAULT();
    sd_host.slot = TF_SPI_HOST;
    sd_host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    sdmmc_card_t *card;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    //ESP_ERROR_CHECK(esp_vfs_fat_sdspi_mount("/", &sd_host, &dev_config, &mount_config, &card));
   //esp_vfs_fat_sdspi_mount

#endif

}

ulong next_millis;

void loop()
{
    auto const now = millis();
    if (now > next_millis)
    {
        next_millis = now + 500;

        char text_buffer[32];
        sprintf(text_buffer, "%lu", now);
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