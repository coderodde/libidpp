#include "SlidingTilePuzzleNode.hpp"
#include <iostream>
#include <sstream>

static SlidingTilePuzzleNode createSourceNode(
    std::size_t width, 
    std::size_t height)
{
    SlidingTilePuzzleNode node(width, height);
    int number = 1;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            node[y][x] = number++;
        }
    }

    node[node.getHeight() - 1][node.getWidth() - 1] = 0;
    return node;
}

int main()
{
    SlidingTilePuzzleNode node(4, 4);
    std::string bar(11, '-');
    try {
        while (true) {
            std::cout 
                << node
                << "\n"
                << bar
                << '\n';

            char choice = static_cast<char>(std::cin.get());
            // Ignore all the leftover chars:
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {   
            case 'u':
                node = node.slideUp();
                break;

            case 'd':
                node = node.slideDown();
                break;

            case 'l':
                node = node.slideLeft();
                break;

            case 'r':
                node = node.slideRight();
                break;

            case 'q':
                return 0;
            }
        }
    }
    catch (ZeroTileOutsideException& ex) {
        std::cout << "Can't move outside of the box.\n";
    }

	return 0;
}