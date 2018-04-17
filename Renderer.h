#pragma once
#include "Vector.h"

void UpdateFrame(void);
void PutPixel(IntPoint pt);
ULONG Interpolation(ULONG current, ULONG Target, float alpha);