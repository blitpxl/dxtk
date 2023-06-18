#pragma once

bool ApproxEq(float a, float b, float epsilon = 0.01f)
{
	return (fabs(a - b) < epsilon);
}