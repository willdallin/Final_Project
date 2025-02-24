#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

// I2C handler utilizado (deve ser o mesmo configurado no CubeMX)
#define SSD1306_I2C_HANDLE hi2c1

// Endereço do display (geralmente 0x3C ou 0x78)
#define SSD1306_I2C_ADDR 0x3C

// Definir o tamanho da tela (dependendo do modelo do seu OLED)
#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64

// Ativar suporte a fontes e gráficos (se precisar)
#define SSD1306_USE_FONTS 1
#define SSD1306_USE_GRAPHICS 1

#endif // __SSD1306_CONF_H__
