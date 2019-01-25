#include "SlidingTilePuzzleNode.hpp"
#include <algorithm>
#include <string>

SlidingTilePuzzleNode::SlidingTilePuzzleNode(std::size_t width,
											 std::size_t height) 
	: 
	m_width(width),
	m_height(height)
{
	m_state.resize(m_width * m_height);
	std::size_t n;
	std::generate(m_state.begin(), m_state.end(), [n = 0]() mutable {
		return n++; 
	});

	m_zero_tile_index = width * height - 1;

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


inline int SlidingTilePuzzleNode::readTile(std::size_t x, std::size_t y) const {
	std::size_t index = m_width * y + x;
	return m_state.at(index);
}

SlidingTilePuzzleNodeRowSelector::SlidingTilePuzzleNodeRowSelector(
	SlidingTilePuzzleNode* node,
	std::size_t start_index) 
	:
	m_node(node),
	m_offset_index(start_index) {

}

int& SlidingTilePuzzleNodeRowSelector::operator[](std::size_t x) {
	return m_node->m_state[m_offset_index + x];
}

SlidingTilePuzzleNodeRowSelector SlidingTilePuzzleNode::operator[](std::size_t y) {
	return m_row_map[y];
}

inline std::size_t SlidingTilePuzzleNode::getX() const {
	return m_zero_tile_index % m_width;
}

inline std::size_t SlidingTilePuzzleNode::getY() const {
	return m_zero_tile_index / m_width;
}
