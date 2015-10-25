#ifndef L74DIGIT_H
#define L74DIGIT_H

#include <Arduino.h>

enum l74_t
{
  A=10,
  B,
  C,
  D,
  E,
  F,
  TOP=16,
  MIDDLE,
  BOTTOM,
  LEFT,
  OFF
};

extern int16_t l74_values[4];
extern bool l74_blinks[4];

void l74_initialize();
void l74_number_rollover_dependent();
void l74_number_rollover_independent();
void l74_update();

#endif

