#include "SlidingTilePuzzleNode.hpp"
#include <iostream>

int main()
{
	SlidingTilePuzzleNode node(2, 3);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 2; x++) {
			std::cout << node[y][x];
		}
	}

	std::cin.get();
	return 0;
}