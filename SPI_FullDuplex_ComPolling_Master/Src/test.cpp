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
  float_t x,y,z;
  bool b;
  int err=0;
  b = g_mlx.begin_SPI(42, nullptr);
  if (!b) ++err;
  b = g_mlx.reset();
  if (!b) ++err;
  b = g_mlx.startSingleMeasurement();
  if (!b) ++err;
  b = g_mlx.readData(&x, &y, &z);
  if (!b) ++err;
  x += 1.0f;
  y += 1.0f;
  b = g_mlx.readData(&x, &y, &z);
  if (!b) ++err;
  printf("%d : %d %d %d\n", err, (int)(x + 3.14f),(int)y,(int)z);
}



