#pragma once
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// Brochage
#define PIN_SCK       12
#define PIN_MISO      13   // non utilisé pour l'affichage
#define PIN_MOSI      11
#define PIN_LCD_CS    15   // NSS : CS de l'écran
#define PIN_LCD_DC     9   // RS : commande / donnée
#define PIN_TOUCH_SDA 10   // SDA : I2C du tactile NS2009
#define PIN_TOUCH_SCL 14   // SCL : I2C du tactile NS2009

#define SPI_WRITE_HZ 20000000

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9488  _panel_instance;
  lgfx::Bus_SPI        _bus_instance;
  lgfx::Touch_NS2009   _touch_instance;

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host    = SPI2_HOST;
      cfg.spi_mode    = 0;
      cfg.freq_write  = SPI_WRITE_HZ;
      cfg.freq_read   = 8000000;
      cfg.spi_3wire   = false;
      cfg.use_lock    = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk    = PIN_SCK;
      cfg.pin_mosi    = PIN_MOSI;
      cfg.pin_miso    = PIN_MISO;
      cfg.pin_dc      = PIN_LCD_DC;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs           = PIN_LCD_CS;
      cfg.pin_rst          = -1;      // reset logiciel
      cfg.pin_busy         = -1;
      cfg.panel_width      = 320;
      cfg.panel_height     = 480;
      cfg.memory_width     = 320;
      cfg.memory_height    = 480;
      cfg.offset_x         = 0;
      cfg.offset_y         = 0;
      cfg.offset_rotation  = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable         = false;   // MISO non relié à l'écran
      cfg.invert           = false;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       = false;
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _touch_instance.config();
      cfg.i2c_port  = 0;
      cfg.i2c_addr  = 0x48;
      cfg.pin_sda   = PIN_TOUCH_SDA;
      cfg.pin_scl   = PIN_TOUCH_SCL;
      cfg.pin_int   = -1;            // interrogation continue, IRQ non utilisée
      cfg.freq      = 400000;
      cfg.x_min     = 205;           // calibration à partir des coins
      cfg.x_max     = 3891;
      cfg.y_min     = 232;
      cfg.y_max     = 3809;
      cfg.offset_rotation = 0;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};
