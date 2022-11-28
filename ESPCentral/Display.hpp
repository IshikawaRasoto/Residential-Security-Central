#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "defineESPCentral.hpp"

void serialDisplayConfig();
void atualizaStatus(const char statusWiFi, const bool alarme);
void mostrarModoAP();
void mostrarNomeProjeto();
void mostrarAlarme(const bool alarme);
void mostrarWifi(const char statusWifi);
void mostrarDisparo();

static const uint8_t wifi_ok[1024] = 
{
  0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x1f, 0xff, 0x80, 0x3e, 0x0f, 0xc0, 0x78, 
  0x03, 0xe0, 0x70, 0x01, 0xe0, 0x61, 0xf8, 0xc0, 0x07, 0xfc, 0x00, 0x0f, 0xff, 0x00, 0x1f, 0x1f, 
  0x00, 0x0c, 0x06, 0x00, 0x00, 0x40, 0x00, 0x00, 0xe0, 0x00, 0x01, 0xf0, 0x00, 0x01, 0xf0, 0x00, 
  0x01, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t wifi_nok[1024] = 
{
  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x70, 0xf0, 0x00, 0x7b, 0xfe, 0x00, 0x3f, 0xff, 0xc0, 0x7f, 
  0xff, 0xe0, 0xff, 0x7f, 0xf0, 0xff, 0x83, 0xf0, 0x73, 0xc0, 0xe0, 0x27, 0xfe, 0x40, 0x0f, 0xff, 
  0x00, 0x0f, 0xfb, 0x00, 0x07, 0xbe, 0x00, 0x02, 0x1e, 0x00, 0x00, 0x6f, 0x00, 0x00, 0xf7, 0x80, 
  0x01, 0xfb, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t no_wifi[1024] = 
{
  0x03, 0xfc, 0x00, 0x0f, 0xff, 0x00, 0x1b, 0x6d, 0x80, 0x36, 0x66, 0xc0, 0x7c, 0x63, 0xe0, 0x4e, 
  0x67, 0x20, 0xcf, 0xff, 0x30, 0x8c, 0x63, 0x10, 0x8c, 0x63, 0x10, 0xff, 0xff, 0xf0, 0xff, 0xff, 
  0xf0, 0x8c, 0x63, 0x10, 0x8c, 0x63, 0x10, 0xcf, 0xff, 0x30, 0x4e, 0x67, 0x20, 0x7c, 0x63, 0xe0, 
  0x36, 0x66, 0xc0, 0x1b, 0x6d, 0x80, 0x0f, 0x6f, 0x00, 0x03, 0xfe, 0x00
};

static const uint8_t com_alarme[1024] =
{
  0x10, 0x00, 0x80, 0x3a, 0x05, 0xc0, 0x7e, 0x07, 0xe0, 0x39, 0xf9, 0xc0, 0x3b, 0xfe, 0xc0, 0x36, 
  0x06, 0xc0, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 
  0x00, 0x08, 0x01, 0x80, 0x18, 0x01, 0x80, 0x30, 0x00, 0xc0, 0x38, 0x01, 0xc0, 0x1f, 0xff, 0x80, 
  0x0f, 0xff, 0x00, 0x01, 0x98, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x60, 0x00
};

static const uint8_t sem_alarme[1024] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x60, 0x00, 0xf0, 0x70, 0x00, 0x7b, 0xfc, 0x00, 0x3f, 
  0xfc, 0x00, 0x1f, 0xfe, 0x00, 0x0f, 0xfe, 0x00, 0x03, 0xfe, 0x00, 0x01, 0xfe, 0x00, 0x0c, 0xff, 
  0x00, 0x0f, 0x3f, 0x00, 0x0f, 0x9f, 0x80, 0x1f, 0xcf, 0x80, 0x1f, 0xf3, 0xc0, 0x00, 0x01, 0xe0, 
  0x00, 0xf0, 0xf0, 0x00, 0xf0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t dispara_alarme[1024] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 
  0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x01, 0xc3, 0x80, 0x00, 0x00, 0x03, 
  0xc3, 0xc0, 0x00, 0x00, 0x03, 0x81, 0xc0, 0x00, 0x00, 0x07, 0x81, 0xe0, 0x00, 0x00, 0x0f, 0x00, 
  0xf0, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0x1e, 0x00, 0x78, 0x00, 0x00, 0x1e, 0x00, 0x38, 
  0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x38, 0x18, 0x1c, 0x00, 0x00, 0x78, 0x18, 0x1e, 0x00, 
  0x00, 0x70, 0x18, 0x0e, 0x00, 0x00, 0xf0, 0x18, 0x07, 0x00, 0x01, 0xe0, 0x18, 0x07, 0x80, 0x01, 
  0xc0, 0x18, 0x03, 0x80, 0x03, 0xc0, 0x18, 0x03, 0xc0, 0x03, 0x80, 0x18, 0x01, 0xc0, 0x07, 0x80, 
  0x18, 0x01, 0xe0, 0x0f, 0x00, 0x18, 0x00, 0xf0, 0x0f, 0x00, 0x18, 0x00, 0xf0, 0x1e, 0x00, 0x18, 
  0x00, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x38, 0x3c, 0x00, 0x18, 0x00, 0x3c, 0x38, 0x00, 0x3c, 0x00, 
  0x1c, 0x78, 0x00, 0x3c, 0x00, 0x1e, 0xf0, 0x00, 0x1c, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x07, 
  0xe0, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
