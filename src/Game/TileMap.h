#pragma once

#include <stdint.h>
#include <vectormath/vectormath_types.h>

#include "Misc/LDtkParser.h"
#include "Graphics/Graphics.h"
#include "Graphics/SpriteBatch.h"

/// TileMapLayer
typedef struct TileMapLayer
{
    int32_t         width;          // Width of tilemap (aka number of columns * tile width)
    int32_t         height;         // Height of tilemap (aka number of rows * tile height)

    int32_t         cols;           // Number of columns (horizon number of tiles)
    int32_t         rows;           // Number of rows (vertical humber of tiles)

    int32_t         tileWidth;      // Tile width
    int32_t         tileHeight;     // Tile height

    SpriteBatch     spriteBatch;    // Batching all tiles into one, only cost 1-draw call when draw tilemap
} TileMapLayer;

/// TileMapDynLayer
typedef struct TileMapDynLayer
{
    int32_t         width;          // Width of tilemap (aka number of columns * tile width)
    int32_t         height;         // Height of tilemap (aka number of rows * tile height)

    int32_t         cols;
    int32_t         rows;

    int32_t         tileSize;       // Tile width

    SpriteSheet     tileSet;
    int32_t*        tiles;          // Each item contain index to sprite in tileset (SpriteSheet)
} TileMapDynLayer;

/// TileMapGrid
/// Use for detect collision
/// @note: Store this data structure stack not is not a good-practice (more details later)
typedef struct TileMapGrid
{
    int32_t         size;
    int32_t         cols;           // Number of columns (horizon number of tiles)
    int32_t         rows;           // Number of rows (vertical humber of tiles)
    int32_t         data[];         // Data, item value is zero mean no collision
} TileMapGrid;

/// Create TileMapGrid from LDtkLayer
TileMapGrid* TileMapGrid_FromLDtkLayer(const LDtkLayer* layer);

/// Destroy TileMapGrid
void TileMapGrid_Destroy(TileMapGrid* grid);

/// Check is the position is collided
inline int32_t TileMapGrid_SafeGet(const TileMapGrid* grid, ivec2 position, int32_t defValue)
{
    if (position.x < 0 || position.x >= grid->cols)
    {
        return defValue;
    }

    if (position.y < 0 || position.y >= grid->rows)
    {
        return defValue;
    }

    return grid->data[position.y * grid->cols + position.x];
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
