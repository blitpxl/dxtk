#include "mathutils.h"

bool equalf(float a, float b, float epsilon)
{
	return (fabs(a - b) < epsilon);
}

float mapRange(float value, float istart, float iend, float ostart, float oend)
{
	return ostart + (value - istart) * (oend - ostart) / (iend - istart);
}