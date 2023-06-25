#include <memory.h>

#include "Game/TileMap.h"
#include "Native/Memory.h"
#include "Graphics/Graphics.h"
#include "Graphics/SpriteBatch.h"

TileMapGrid* TileMapGrid_FromLDtkLayer(const LDtkLayer* layer)
{
    int32_t memoryBlockSize = (int32_t)(sizeof(TileMapGrid) + layer->cols * layer->rows * sizeof(int32_t));
    TileMapGrid* grid = (TileMapGrid*)Memory_AllocTag("TileMapGrid", memoryBlockSize, alignof(int32_t));
    // Out of memory should crash (assertion)
    
    grid->size = layer->tileSize;
    grid->cols = layer->cols;
    grid->rows = layer->rows;

    for (int32_t y = 0, h = layer->rows; y < h; y++)
    {
        int32_t y0 = (h - y - 1) * layer->cols;
        int32_t y1 = y * grid->cols;
        for (int32_t x = 0, w = layer->cols; x < w; x++)
        {
            const LDtkIntGridValue cell = layer->values[x + y0];
            grid->data[x + y1] = (int32_t)(cell.value != 0);
        }
    }

    return grid;
}

void TileMapGrid_Destroy(TileMapGrid* grid)
{
    Memory_FreeTag("TileMapGrid", grid);
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
