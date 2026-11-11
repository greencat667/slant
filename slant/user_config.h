#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// Pin map for Waveshare ESP32-S3-ePaper-1.54 (V2, ESP32-S3-PICO-1-N8R8)
// Source: Waveshare's official ESP32-S3-ePaper-1.54 examples.

#define EPD_SPI_NUM        SPI2_HOST
#define EPD_WIDTH  200
#define EPD_HEIGHT 200

#define EPD_DC_PIN    GPIO_NUM_10
#define EPD_CS_PIN    GPIO_NUM_11
#define EPD_SCK_PIN   GPIO_NUM_12
#define EPD_MOSI_PIN  GPIO_NUM_13
#define EPD_RST_PIN   GPIO_NUM_9
#define EPD_BUSY_PIN  GPIO_NUM_8

#define EPD_PWR_PIN     GPIO_NUM_6
#define Audio_PWR_PIN   GPIO_NUM_42
#define VBAT_PWR_PIN    GPIO_NUM_17

#define BOOT_BUTTON_PIN GPIO_NUM_0
#define PWR_BUTTON_PIN  GPIO_NUM_18

#endif // !USER_CONFIG_H
