#include "SlidingTilePuzzleNode.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ios>
#include <ostream>
#include <string>

SlidingTilePuzzleNode::SlidingTilePuzzleNode(std::size_t width,
											 std::size_t height) 
	: 
	m_width(width),
	m_height(height)
{
	m_state.resize(m_width * m_height);
	std::size_t n;
	std::generate(m_state.begin(), m_state.end(), [n = 1]() mutable {
		return n++; 
	});

	m_zero_tile_index = width * height - 1;
    m_state[m_zero_tile_index] = 0;

	for (std::size_t y = 0; y < m_height; y++) {
		SlidingTilePuzzleNodeRowSelector row_selector(this, y * width);
		m_row_map[y] = row_selector;
	}
}

SlidingTilePuzzleNode::~SlidingTilePuzzleNode() = default;

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideUp() {
	SlidingTilePuzzleNode node(getWidth(), getHeight());
	auto x = getX();
	auto y = getY();

	if (y == 0) {
		ZeroTileOutsideException exception(std::string("y == 0 upon slideUp"));
		throw exception;
	}

	auto next_x = x;
	auto next_y = y - 1;
	auto my = *this;
	std::swap(my[y][x], my[next_y][next_x]);
	return node;
}

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideDown() {
	SlidingTilePuzzleNode node(getWidth(), getHeight());
	auto x = getX();
	auto y = getY();

	if (y == m_width - 1) {
		ZeroTileOutsideException exception(std::string("y == m_height - 1"));
		throw exception;
	}

	auto next_x = x;
	auto next_y = y + 1;
	auto my = *this;
	std::swap(my[y][x], my[next_y][next_x]);
	return node;
}

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideLeft() {
	SlidingTilePuzzleNode node(getWidth(), getHeight());
	auto x = getX();
	auto y = getY();

	if (x == 0) {
		ZeroTileOutsideException exception(std::string("x == 0"));
		throw exception;
	}

	auto next_x = x - 1;
	auto next_y = y;
	auto my = *this;
	std::swap(my[y][x], my[next_y][next_x]);
	return node;
}

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideRight() {
	SlidingTilePuzzleNode node(getWidth(), getHeight());
	auto x = getX();
	auto y = getY();

	if (x == 0) {
		ZeroTileOutsideException exception(std::string("x == m_width - 1"));
		throw exception;
	}

	auto next_x = x + 1;
	auto next_y = y;
	auto my = *this;
	std::swap(my[y][x], my[next_y][next_x]);
	return node;
}

SlidingTilePuzzleNodeRowSelector::SlidingTilePuzzleNodeRowSelector(
	SlidingTilePuzzleNode* node,
	std::size_t start_index) 
	:
	m_node(node),
	m_offset_index(start_index) {

}

int& SlidingTilePuzzleNodeRowSelector::operator[](std::size_t x) {
	return m_node->m_state.at(m_offset_index + x);
}

SlidingTilePuzzleNodeRowSelector SlidingTilePuzzleNode::operator[](std::size_t y) {
	return m_row_map[y];
}

// Rename to getEmptyTileX!
inline std::size_t SlidingTilePuzzleNode::getX() const {
	return m_zero_tile_index % m_width;
}

inline std::size_t SlidingTilePuzzleNode::getY() const {
	return m_zero_tile_index / m_width;
}

std::ostream& operator<<(std::ostream& out, SlidingTilePuzzleNode& node) {
    const auto max_tile_number = node.getHeight() * node.getWidth() - 1;
    const auto max_tile_number_length = 
        static_cast<std::size_t>(
            std::floor(
                std::log10(max_tile_number)) + 1); // Space between adjacent entries.
    
    out.fill(' ');
    out << std::setw(max_tile_number_length);

    for (auto y = 0; y < node.getHeight(); y++) {
        if (y > 0) {
            out << '\n';
        }

        for (auto x = 0; x < node.getWidth(); x++) {
            if (x > 0) {
                out << ' ';
            }

            out << node[y][x];
        }
    }

    return out;
}
