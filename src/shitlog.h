// Industrial level logging system

#pragma once
#include <iostream>
#define print(x) std::cout << x << std::endl
#define printw(x) std::wcout << x << std::endl
#define printx(x, y) std::cout << x << y << std::endl
#define printd(x) if (is_debug) print(x)