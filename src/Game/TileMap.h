#pragma once

#include <stdint.h>
#include <vectormath/vectormath_types.h>

struct TileMap
{
    int32_t         width;          // Width of tilemap (aka number of columns * tile width)
    int32_t         height;         // Height of tilemap (aka number of rows * tile height)

    int32_t         cols;           // Number of columns (horizon number of tiles)
    int32_t         rows;           // Number of rows (vertical humber of tiles)

    int32_t         tileWidth;      // Tile width
    int32_t         tileHeight;     // Tile height

    int32_t         spriteBatch;    // Batching all tiles into one, only cost 1-draw call when draw tilemap
};

struct TileMapCollision
{
    int32_t*        data;           // Data, item value is zero mean no collision

    int32_t         cols;           // Number of columns (horizon number of tiles)
    int32_t         rows;           // Number of rows (vertical humber of tiles)

    int32_t         tileWidth;      // Tile width
    int32_t         tileHeight;     // Tile height

    /// Check is the position is collided
    inline bool IsCollided(ivec2 position)
    {
        if (position.x < 0 || position.x >= cols)
        {
            return false;
        }

        if (position.y < 0 || position.y >= rows)
        {
            return false;
        }

        return data[position.y * cols + position.x];
    }
};

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
