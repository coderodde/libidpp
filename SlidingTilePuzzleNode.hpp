#pragma once
#include <ostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

class ZeroTileOutsideException : public std::logic_error {
public:
	ZeroTileOutsideException(std::string& error_message) 
	: 
		std::logic_error(error_message)
	{

	}
};

class SlidingTilePuzzleNode;

class SlidingTilePuzzleNodeRowSelector
{
public:
	SlidingTilePuzzleNodeRowSelector() = default;
	SlidingTilePuzzleNodeRowSelector(
		SlidingTilePuzzleNode* node,
		std::size_t start_index);
	int& operator[](std::size_t x);

private:
	SlidingTilePuzzleNode* m_node;
	std::size_t m_offset_index;
};

class SlidingTilePuzzleNode
{
public:
	SlidingTilePuzzleNode(std::size_t width, std::size_t height);
    SlidingTilePuzzleNode(const SlidingTilePuzzleNode& other);
    SlidingTilePuzzleNode& operator=(const SlidingTilePuzzleNode& other);
    SlidingTilePuzzleNode& operator=(SlidingTilePuzzleNode&& other);
    //SlidingTilePuzzleNode(SlidingTilePuzzleNode&& other);
    ~SlidingTilePuzzleNode();

	SlidingTilePuzzleNode slideUp();
	SlidingTilePuzzleNode slideDown();
	SlidingTilePuzzleNode slideLeft();
	SlidingTilePuzzleNode slideRight();

	std::size_t getWidth()  const { return m_width; }
	std::size_t getHeight() const { return m_height;  };

	SlidingTilePuzzleNodeRowSelector& operator[](size_t y);
	
    friend std::ostream& operator<<(
        std::ostream& out,
        SlidingTilePuzzleNode & node);

private:

	std::vector<int> m_state;
	std::unordered_map<std::size_t, SlidingTilePuzzleNodeRowSelector> m_row_map;
	std::size_t m_width;
	std::size_t m_height;
    std::size_t m_zero_tile_x;
    std::size_t m_zero_tile_y;

	// Let ...RowSelector access m_state:
	friend class SlidingTilePuzzleNodeRowSelector;
    // Check that a slide is possible:
    void checkOnSlideUp   () const;
    void checkOnSlideDown () const;
    void checkOnSlideLeft () const;
    void checkOnSlideRight() const;

    inline void setZeroTileCoordinates(std::size_t x, std::size_t y);
};
