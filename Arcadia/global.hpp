#pragma once
#include <cstdint>

// so we will have only 5 states
enum class GameStatus : uint8_t
{
	main_Menu, level_1, level_2, level_3, level_4,credits
};
