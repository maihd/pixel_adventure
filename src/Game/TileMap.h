#pragma once

#include <stdint.h>
#include <vectormath/vectormath_types.h>

#include "Misc/LDtkParser.h"
#include "Graphics/SpriteBatch.h"

struct TileMapLayer
{
    int32_t         width;          // Width of tilemap (aka number of columns * tile width)
    int32_t         height;         // Height of tilemap (aka number of rows * tile height)

    int32_t         cols;           // Number of columns (horizon number of tiles)
    int32_t         rows;           // Number of rows (vertical humber of tiles)

    int32_t         tileWidth;      // Tile width
    int32_t         tileHeight;     // Tile height

    SpriteBatch     spriteBatch;    // Batching all tiles into one, only cost 1-draw call when draw tilemap
};

/// TileMapGrid
/// Use for detect collision
/// @note: Store this data structure stack not is not a good-practice (more details later)
struct TileMapGrid
{
    int32_t         size;
    int32_t         cols;           // Number of columns (horizon number of tiles)
    int32_t         rows;           // Number of rows (vertical humber of tiles)
    int32_t         data[];         // Data, item value is zero mean no collision

    /// Create TileMapGrid from LDtkLayer
    static TileMapGrid* FromLDtkLayer(const LDtkLayer* layer);

    /// Destroy TileMapGrid
    void Destroy(); 

    /// Check is the position is collided
    inline int32_t SafeGet(ivec2 position, int32_t defValue) const
    {
        if (position.x < 0 || position.x >= cols)
        {
            return defValue;
        }

        if (position.y < 0 || position.y >= rows)
        {
            return defValue;
        }

        return data[position.y * cols + position.x];
    }
};

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
