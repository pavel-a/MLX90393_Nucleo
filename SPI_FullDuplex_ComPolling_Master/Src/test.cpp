/*
 * MLX test
 * !!! Float print does not fit in C031 flash! use workarounds!
 */
#include "MLX90393.h"
#include <cstddef>
#include <cstring>
#include <cstdio>

static MLX90393 g_mlx;

typedef float float_t;




extern "C"
void test()
{
  bool b;
  int err=0;
  
  // TEST $$$$
  uint16_t v;
  b = g_mlx.readRegister(0, &v);
  if (!b) ++err;

  uint16_t meas[4];
  b = g_mlx.readMeasurementRaw(&meas[0], 0xF);
  if (!b) ++err;

  printf("%d %u\n", err, v );
  
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



