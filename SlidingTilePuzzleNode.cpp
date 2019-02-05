#include "SlidingTilePuzzleNode.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>

void SlidingTilePuzzleNode::checkOnSlideUp() const 
{
    if (m_zero_tile_y == 0) {
        ZeroTileOutsideException exception(std::string("'m_zero_tile_y == 0' upon slideUp"));
        throw exception;
    }
}

void SlidingTilePuzzleNode::checkOnSlideDown() const
{
    if (m_zero_tile_y == m_height - 1) {
        ZeroTileOutsideException exception(std::string("'m_zero_tile_y == m_height - 1' upon slideDown"));
        throw exception;
    }
}

void SlidingTilePuzzleNode::checkOnSlideLeft() const 
{
    if (m_zero_tile_x == 0) {
        ZeroTileOutsideException exception(std::string("'m_zero_tile_x == 0' upon slideLeft"));
        throw exception;
    }
}

void SlidingTilePuzzleNode::checkOnSlideRight() const 
{
    if (m_zero_tile_y == m_width - 1) {
        ZeroTileOutsideException exception(std::string("'m_zero_tile_x == m_width - 1' upon slideRight"));
        throw exception;
    }
}

// Initial constructor building the solved slide puzzle:
SlidingTilePuzzleNode::SlidingTilePuzzleNode(std::size_t width,
											 std::size_t height) 
	: 
	m_width(width),
	m_height(height),
    m_zero_tile_x(width - 1),
    m_zero_tile_y(height - 1)
{
    // Copy the state:
	m_state.resize(m_width * m_height);
	std::size_t n;
	std::generate(m_state.begin(), m_state.end(), [n = 1]() mutable {
		return n++; 
	});

    // Deal with the empty tile:
    m_state[width * height - 1] = 0;

    // Build the row index:
	for (std::size_t y = 0; y < m_height; y++) {
		SlidingTilePuzzleNodeRowSelector row_selector(this, y * width);
		m_row_map[y] = row_selector;
	}
}

// Copy constructor.
SlidingTilePuzzleNode::SlidingTilePuzzleNode(const SlidingTilePuzzleNode& other) 
{
    // Copy the easy stuff.
    this->m_height      = other.m_height;
    this->m_width       = other.m_width;
    this->m_zero_tile_x = other.m_zero_tile_x;
    this->m_zero_tile_y = other.m_zero_tile_y;
    this->m_state       = other.m_state;

    std::size_t y = 0;

    for (auto const& entry : other.m_row_map) {
        SlidingTilePuzzleNodeRowSelector row_selector(this, y);
        this->m_row_map[entry.first] = row_selector;
        y += m_width;
    }
}

// Copy assignment.
SlidingTilePuzzleNode& SlidingTilePuzzleNode::operator=(const SlidingTilePuzzleNode& other)
{
    this->m_height      = other.m_height;
    this->m_width       = other.m_width;
    this->m_zero_tile_x = other.m_zero_tile_x;
    this->m_zero_tile_y = other.m_zero_tile_y;
    this->m_state       = other.m_state;
    this->m_row_map     = other.m_row_map;

    // Correct the map values to row selectors of this tile:
    for (auto& entry : m_row_map) {
        SlidingTilePuzzleNodeRowSelector row_selector(this, entry.first);
        entry.second = row_selector;
    }

    return *this;
}

// Move assignment.
SlidingTilePuzzleNode& SlidingTilePuzzleNode::operator=(SlidingTilePuzzleNode&& other)
{
    m_height      = other.m_height;
    m_width       = other.m_width;
    m_zero_tile_x = other.m_zero_tile_x;
    m_zero_tile_y = other.m_zero_tile_y;
    m_state       = std::move(other.m_state);
    m_row_map     = std::move(other.m_row_map);

    std::size_t y = 0;

    for (auto& entry : m_row_map) {
        SlidingTilePuzzleNodeRowSelector row_selector(this, y);
        entry.second = row_selector;
        y += m_width;
    }

    return *this;
}

SlidingTilePuzzleNode::~SlidingTilePuzzleNode() = default;

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideUp()
{
    checkOnSlideUp();
    SlidingTilePuzzleNode node(*this);
    auto x2 = m_zero_tile_x;
    auto y2 = m_zero_tile_y - 1;
    std::swap(node[m_zero_tile_y][m_zero_tile_x], node[y2][x2]);
    setZeroTileCoordinates(x2, y2);
	return node;
}

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideDown()
{
    checkOnSlideDown();
	SlidingTilePuzzleNode node(*this);
	auto x2 = m_zero_tile_x;
    auto y2 = m_zero_tile_y + 1;
    std::swap(node[m_zero_tile_y][m_zero_tile_x], node[y2][x2]);
    setZeroTileCoordinates(x2, y2);
	return node;
}

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideLeft() 
{
    checkOnSlideLeft();
	SlidingTilePuzzleNode node(*this);
    auto x2 = m_zero_tile_x - 1;
    auto y2 = m_zero_tile_y;
    std::swap(node[m_zero_tile_y][m_zero_tile_x], node[y2][x2]);
    setZeroTileCoordinates(x2, y2);
    return node;
}

SlidingTilePuzzleNode SlidingTilePuzzleNode::slideRight()
{
    checkOnSlideRight();
	SlidingTilePuzzleNode node(*this);
    auto x2 = m_zero_tile_x + 1;
    auto y2 = m_zero_tile_y;
    std::swap(node[m_zero_tile_y][m_zero_tile_x], node[y2][x2]);
    setZeroTileCoordinates(x2, y2);
    return node;
}

SlidingTilePuzzleNodeRowSelector::SlidingTilePuzzleNodeRowSelector(
	SlidingTilePuzzleNode* node,
	std::size_t start_index) 
	:
	m_node(node),
	m_offset_index(start_index) 
{

}

int& SlidingTilePuzzleNodeRowSelector::operator[](std::size_t x) 
{
	return m_node->m_state.at(m_offset_index + x);
}

SlidingTilePuzzleNodeRowSelector& SlidingTilePuzzleNode::operator[](std::size_t y) 
{
	return m_row_map[y];
}

std::ostream& operator<<(std::ostream& out, SlidingTilePuzzleNode& node) 
{
    const auto max_tile_number = node.getHeight() * node.getWidth() - 1;
    const auto max_tile_number_length = 
        static_cast<std::size_t>(
            std::floor(std::log10(max_tile_number)) + 1); // Space between adjacent entries.
    
    for (auto y = 0; y < node.getHeight(); y++) {
        if (y > 0) {
            out << '\n';
        }

        for (auto x = 0; x < node.getWidth(); x++) {
            if (x > 0) {
                out << " ";
            }

            out << std::setfill(' ') << node[y][x];
        }
    }

    return out;
}

inline void SlidingTilePuzzleNode::setZeroTileCoordinates(std::size_t x, std::size_t y)
{
    m_zero_tile_x = x;
    m_zero_tile_y = y;
}
