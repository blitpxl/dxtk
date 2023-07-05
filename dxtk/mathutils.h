#pragma once

bool equalf(float a, float b, float epsilon = 0.01f)
{
	return (fabs(a - b) < epsilon);
}