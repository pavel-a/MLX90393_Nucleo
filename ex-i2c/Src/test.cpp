/*
 * MLX test
 * !!! Float print does not fit in C031 flash! use workarounds!
 */
#include "MLX90393.h"
#include <cstddef>
#include <cstring>
#include <cstdio>

#define dbgprintf(fmt,...) dprintf(1, fmt, ## __VA_ARGS__) 

extern "C" {

void HAL_Delay(unsigned ms);
void wait_button(void);
bool check_button(void);
}

inline void delay(unsigned ms)
{
    if(0 == ms) return;
    HAL_Delay(ms);
}

static MLX90393 g_mlx;

typedef float float_t;

extern "C"
void test()
{
  bool b;
  int err=0;

  b = g_mlx.begin_I2C(0, nullptr);
  
  uint16_t v;
  for (uint8_t nr=0; nr < 16; nr++) {
      b = g_mlx.readRegister(nr, &v);
      if (!b) ++err;
      else
          dbgprintf("Reg.%u=%4.4X\n", nr, v );
  }

  b = g_mlx.startSingleMeasurement(MLX90393_AXIS_T);
  if (!b) ++err;
  delay(2); // TODO wait for READY pin
  uint16_t meas[4] = {0}; // order: TXYZ
  b = g_mlx.readMeasurementRaw(&meas[0], MLX90393_AXIS_T);
  if (!b) ++err;
  // Temperature [pg.11]: 45.0/DEG.C; value@25C=46244
  int itemp = (((int)meas[0] - 46244 + (45/2)) / 45) + 25;
  dbgprintf("%d temp=%u\n", err, itemp );

  while(1) {

  float_t x,y,z;
  b = g_mlx.readData(&x, &y, &z);
  if (!b) ++err;
  printf("%d : %d %d %d\n", err, (int)(x),(int)y,(int)z);
  
  wait_button();
  }

#if 0  
#endif
}

extern "C"
void test0()
{
  bool b;
  int err=0;
  
  // TEST $$$$
  b = g_mlx.exitMode();
  if (!b) ++err;
  b = g_mlx.reset();
  if (!b) ++err;
  delay(2); // after reset

  // Write reg
  b = g_mlx.writeRegister(11, 0x2345);
  if (!b) ++err;
  b = g_mlx.writeRegister(12, 0xABCD);
  if (!b) ++err;
  
  
  uint16_t v;
  for (uint8_t nr=0; nr < 16; nr++) {
      b = g_mlx.readRegister(nr, &v);
      if (!b) ++err;
      else
          dbgprintf("Reg.%u=%4.4X\n", nr, v );
  }

  while(1) {

  b = g_mlx.startSingleMeasurement(0xF);
  if (!b) ++err;
  delay(2); // TODO wait for READY pin
  uint16_t meas[4] = {0}; // order: TXYZ
  b = g_mlx.readMeasurementRaw(&meas[0], 0xF);
  if (!b) ++err;
  dbgprintf("%d x=%u y=%u z=%u\n", err, meas[1], meas[2], meas[3]);
  // Temperature [pg.11]: 45.0/DEG.C; value@25C=46244
  int itemp = (((int)meas[0] - 46244 + (45/2)) / 45) + 25;
  dbgprintf("%d temp=%u\n", err, itemp );
  
  wait_button();
  }

#if 0  
  b = g_mlx.begin_SPI(42, nullptr);
  if (!b) ++err;
  b = g_mlx.reset();
  if (!b) ++err;
  b = g_mlx.startSingleMeasurement();
  if (!b) ++err;

  float_t x,y,z;
  b = g_mlx.readData(&x, &y, &z);
  if (!b) ++err;
  x += 1.0f;
  y += 1.0f;
  b = g_mlx.readData(&x, &y, &z);
  if (!b) ++err;
  printf("%d : %d %d %d\n", err, (int)(x + 3.14f),(int)y,(int)z);
#endif
}



