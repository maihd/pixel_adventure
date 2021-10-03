#include <time.h>
#include <float.h>
#include <memory.h>
#include <stdlib.h>

#include "Game/Game.h"
#include "Game/Components.h"
#include "Math/VectorMathSimd.h"

#include "Native/Window.h"
#include "Native/FileSystem.h"
#include "Graphics/Graphics.h"
#include "Graphics/SpriteBatch.h"

#include "Framework/JobSystem.h"
#include "Framework/InputSystem.h"

#include <LDtkLoader/World.hpp>

struct Grid
{
    int32_t     cols;
    int32_t     rows;
    int32_t     size;
    int32_t*    data;
};

struct Entity
{
    ivec2       gridSize;
    ivec2       gridPosition;

    vec2        ratioPosition;
    vec2        ratioVelocity;

    float       radius;
};

static SpriteSheet spriteSheetBackground;
static SpriteSheet spriteSheetTerrain;

static std::map<int, SpriteSheet> spritesheets;
static std::map<std::string, SpriteBatch> spriteBatchs;

int   spriteIndex = 0;
float spriteTimer = 0.0f;
float spriteInterval = 0.1f;

vec2 frogScale;
vec2 frogPosition;

SpriteSheet spriteBatch_FrogIdle;
SpriteSheet spriteBatch_FrogRun;
SpriteSheet spriteBatch_FrogJumpUp;
SpriteSheet spriteBatch_FrogFallDown;
SpriteSheet spriteBatch_FrogWallCollide;

bool frogRunning = false;
bool prevFrogRunning = false;
SpriteSheet* frogSpriteSheet = &spriteBatch_FrogIdle;

float gravity = 90.0f;
float fallSpeed = 0.0f;

Entity  frog;
Grid    grid;

inline vec2 GetWorldPosition(int32_t gridCellSize, ivec2 gridPosition, vec2 ratioPosition)
{
    return vec2_new((float)gridCellSize * ((float)gridPosition.x + ratioPosition.x), (float)gridCellSize * ((float)gridPosition.y + ratioPosition.y));
}

inline vec2 GetWorldPosition(const Grid& grid, const Entity& entity)
{
    return GetWorldPosition(grid.size, entity.gridPosition, entity.ratioPosition);
}

inline bool HasGridCollision(const Grid& grid, ivec2 position)
{
    if (position.x < 0 || position.x >= grid.cols)
    {
        return false;
    }

    if (position.y < 0 || position.y >= grid.rows)
    {
        return false;
    }

    return grid.data[position.y * grid.cols + position.x];
}

inline bool HasEntityCollision(const Entity& a, const Entity& b)
{
    const float zone = a.radius + b.radius;
    const vec2  posA = GetWorldPosition(1, a.gridPosition, a.ratioPosition);
    const vec2  posB = GetWorldPosition(1, a.gridPosition, a.ratioPosition);
    return vec2_distsqr(posA, posB) <= zone * zone;
}

inline void UpdateEntity(Entity* entity, const Grid& grid, float deltaTime)
{
    entity->ratioPosition += entity->ratioVelocity * deltaTime + vec2_new(0.0f, -30.0f) * 0.5f * deltaTime * deltaTime;
    entity->ratioVelocity += vec2_new(0.0f, -30.0f) * deltaTime;
    entity->ratioVelocity *= vec2_new(0.90f, 0.96f); // Damping

    if (entity->ratioPosition.x > 1.0f)
    {
        entity->gridPosition.x += 1;
        entity->ratioPosition.x -= 1.0f;
    }

    if (entity->ratioPosition.x < 0.0f)
    {
        entity->gridPosition.x -= 1;
        entity->ratioPosition.x += 1.0f;
    }

    if (entity->ratioPosition.y > 1.0f)
    {
        entity->gridPosition.y += 1;
        entity->ratioPosition.y -= 1.0f;
    }

    if (entity->ratioPosition.y < 0.0f)
    {
        entity->gridPosition.y -= 1;
        entity->ratioPosition.y += 1.0f;
    }

    // Left
    if (HasGridCollision(grid, ivec2{entity->gridPosition.x - 1, entity->gridPosition.y}) && entity->ratioPosition.x <= 0.3f)
    {
        entity->ratioPosition.x = 0.3f;
        entity->ratioVelocity.x = 0.0f;
    }

    // Right
    if (HasGridCollision(grid, ivec2{entity->gridPosition.x + 1, entity->gridPosition.y}) && entity->ratioPosition.x >= 0.7f)
    {
        entity->ratioPosition.x = 0.7f;
        entity->ratioVelocity.x = 0.0f;
    }

    // Up
    if (HasGridCollision(grid, ivec2{entity->gridPosition.x, entity->gridPosition.y + 1}) && entity->ratioPosition.y >= 0.3f)
    {
        entity->ratioPosition.y = 0.3f;
        entity->ratioVelocity.y = 0.0f;
    }

    // Down
    if (HasGridCollision(grid, ivec2{entity->gridPosition.x, entity->gridPosition.y - 1}) && entity->ratioPosition.y <= 0.5f)
    {
        entity->ratioPosition.y = 0.5f;
        entity->ratioVelocity.y = 0.0f;
    }
}

inline bool EntityOnGround(const Entity& entity, const Grid& grid)
{
    return (HasGridCollision(grid, ivec2{ entity.gridPosition.x, entity.gridPosition.y - 1 }) && entity.ratioPosition.y <= 0.5f);
}

inline bool EntityOnWall(const Entity& entity, const Grid& grid)
{
    return !EntityOnGround(entity, grid)
        && ((HasGridCollision(grid, ivec2{ entity.gridPosition.x - 1, entity.gridPosition.y }))
        ||  (HasGridCollision(grid, ivec2{ entity.gridPosition.x + 1, entity.gridPosition.y })));
}

void CreateSpriteSheet(SpriteSheet* spriteSheet, const ldtk::Tileset& tileset)
{
    int32_t cols = tileset.texture_size.x / tileset.tile_size;
    int32_t rows = tileset.texture_size.y / tileset.tile_size;
    
    char absPath[1024];
    FileSystem_GetExistsPath(absPath, sizeof(absPath), tileset.path.c_str());

    Graphics::LoadSpriteSheet(spriteSheet, absPath, cols, rows);
}

void CreateSpriteBatch(const ldtk::Layer* layer)
{
    const ldtk::Tileset* tileset = layer->getTileset();
    if (!tileset || spritesheets.find(tileset->uid) == spritesheets.cend())
    {
        return;
    }

    const SpriteSheet spritesheet = spritesheets[tileset->uid];

    SpriteBatch spriteBatch = {};
    SpriteBatch_Create(&spriteBatch, &spritesheets[tileset->uid], layer->allTiles().size() * 6);
    
    SpriteBatch_Begin(&spriteBatch);
    for (ldtk::Tile tile : layer->allTiles())
    {
        const int32_t cols = tile.texture_position.x / tileset->tile_size;
        const int32_t rows = tile.texture_position.y / tileset->tile_size;
        const Sprite* sprite = &spritesheet.sprites[rows * spritesheet.cols + cols];
        const vec2 position = vec2_new(tile.position.x + tileset->tile_size * 0.5f, layer->level->size.y - tile.position.y - tileset->tile_size * 0.5f);
        const vec2 scale = vec2_new(tile.flipX ? -1.0f : 1.0f, tile.flipY ? -1.0f : 1.0f);
        SpriteBatch_DrawSprite(&spriteBatch, sprite, position, 0.0f, scale, vec3_new1(1.0f));
    }
    SpriteBatch_End(&spriteBatch);

    spriteBatchs[layer->getName()] = spriteBatch;
}

void Game_Setup()
{
    FileSystem_AddSearchPath("assets");
    FileSystem_AddSearchPath("../../assets");

    char worldPath[1024];
    if (!FileSystem_GetExistsPath(worldPath, sizeof(worldPath), "pixel_adventure.ldtk"))
    {
        return;
    }

    ldtk::World world;
    world.loadFromFile(worldPath);

    const auto& tilesets = world.allTilesets();
    for (const auto& tileset : tilesets)
    {
        SpriteSheet sheet;
        CreateSpriteSheet(&sheet, tileset);

        spritesheets[tileset.uid] = sheet;
    }

    for (const auto& layer : world.getLevel("Level0")->allLayers())
    {
        CreateSpriteBatch(&layer);
    }

    int spriteCols_FrogIdle = 11;
    int spriteRows_FrogIdle = 1;
    Graphics::LoadSpriteSheet(&spriteBatch_FrogIdle, "main_characters/frog_idle.png", spriteCols_FrogIdle, spriteRows_FrogIdle);

    int spriteCols_FrogRun = 12;
    int spriteRows_FrogRun = 1;
    Graphics::LoadSpriteSheet(&spriteBatch_FrogRun, "main_characters/frog_run.png", spriteCols_FrogRun, spriteRows_FrogRun);

    int spriteCols_FrogJumpUp = 1;
    int spriteRows_FrogJumpUp = 1;
    Graphics::LoadSpriteSheet(&spriteBatch_FrogJumpUp, "main_characters/frog_jump_up.png", spriteCols_FrogJumpUp, spriteRows_FrogJumpUp);

    int spriteCols_FrogFallDown = 1;
    int spriteRows_FrogFallDown = 1;
    Graphics::LoadSpriteSheet(&spriteBatch_FrogFallDown, "main_characters/frog_fall_down.png", spriteCols_FrogFallDown, spriteRows_FrogFallDown);

    int spriteCols_FrogWallCollide = 5;
    int spriteRows_FrogWallCollide = 1;
    Graphics::LoadSpriteSheet(&spriteBatch_FrogWallCollide, "main_characters/frog_wall_holding.png", spriteCols_FrogWallCollide, spriteRows_FrogWallCollide);

    frogScale = vec2_new(1.0f, 1.0f);
    frogPosition = vec2_new((float)Window_GetWidth() * 0.5f, (float)Window_GetHeight() * 0.5f);

    const ldtk::Layer* layer = world.getLevel("Level0")->getLayer("Collisions");
    
    grid.size = layer->getCellSize();
    grid.cols = layer->getGridSize().x;
    grid.rows = layer->getGridSize().y;
    grid.data = (int32_t*)malloc(grid.cols * grid.rows * sizeof(int32_t));

    const int cellSize = layer->getCellSize();
    for (int x = 0, w = layer->getGridSize().x; x < w; x++)
    {
        for (int y = 0, h = layer->getGridSize().y; y < h; y++)
        {
            const auto& cell = layer->getIntGridVal(x, h - y - 1);

            if (cell.value == ldtk::IntGridValue::None.value)
            {
                grid.data[x + y * grid.cols] = 0;
                continue;
            }

            grid.data[x + y * grid.cols] = 1;
        }
    }

    frog.radius = 0.0f;
    frog.gridSize = ivec2{ 2, 2 };
    frog.gridPosition = ivec2{ grid.cols >> 1, grid.rows >> 1 };
    frog.ratioVelocity = vec2_new1(0.0f);
    frog.ratioPosition = vec2_new1(0.0f);

    const ldtk::Layer* entitiesLayer = world.getLevel("Level0")->getLayer("Entities");
    for (const auto& entity : entitiesLayer->allEntities())
    {
        frog.radius = 0.0f;
        frog.gridSize = ivec2{ 2, 2 };
        frog.gridPosition = ivec2{ entity.getGridPosition().x, entitiesLayer->getGridSize().y - entity.getGridPosition().y - 1 };
        frog.ratioVelocity = vec2_new1(0.0f);
        frog.ratioPosition = vec2_new1(0.0f);
    }
}

void Game_Shutdown()
{
}

void Game_Load()
{
}

void Game_Unload()
{
}

void Game_Update(float totalTime, float deltaTime)
{
    if (InputSystem_GetKey(KeyCode_LeftArrow))
    {
        frog.ratioVelocity.x = -4.0f;

        frogScale.x = -fabsf(frogScale.x);
        frogPosition.x -= 100.0f * deltaTime;
    }

    if (InputSystem_GetKey(KeyCode_RightArrow))
    {
        frog.ratioVelocity.x = 4.0f;

        frogScale.x = fabsf(frogScale.x);
        frogPosition.x += 100.0f * deltaTime;
    }

    if (InputSystem_GetKeyDown(KeyCode_Space))
    {
        if (EntityOnGround(frog, grid))
        {
            fallSpeed += 90.0f;
            frog.ratioVelocity.y = 20.0f;
        }
        else if (EntityOnWall(frog, grid))
        {
            frog.ratioVelocity.x = -frog.ratioVelocity.x;
            frog.ratioVelocity.y = 15.0f;
        }
    }

    //frogPosition.y += fallSpeed * deltaTime - 0.5f * gravity * deltaTime * deltaTime;
    //fallSpeed -= gravity * deltaTime;

    SpriteSheet* nextSpritesheet;
    if (EntityOnWall(frog, grid))
    {
        nextSpritesheet = &spriteBatch_FrogWallCollide;
    }
    else if (frog.ratioVelocity.y > 0.0f)
    {
        nextSpritesheet = &spriteBatch_FrogJumpUp;
    }
    else if (frog.ratioVelocity.y < 0.0f)
    {
        nextSpritesheet = &spriteBatch_FrogFallDown;
    }
    else if (fabsf(frog.ratioVelocity.x) > 0.4f)
    {
        nextSpritesheet = &spriteBatch_FrogRun;
    }
    else
    {
        nextSpritesheet = &spriteBatch_FrogIdle;
    }

    if (nextSpritesheet != frogSpriteSheet)
    {
        frogSpriteSheet = nextSpritesheet;
        spriteIndex = 0;
    }

    spriteTimer += deltaTime;
    if (spriteTimer >= spriteInterval)
    {
        spriteTimer -= spriteInterval;
        spriteIndex = (spriteIndex + 1) % frogSpriteSheet->spriteCount;
    }

    UpdateEntity(&frog, grid, deltaTime);

    #if 0
    const float frogWidth = frogSpriteSheet->sprites[spriteIndex].width;
    const float frogHeight = frogSpriteSheet->sprites[spriteIndex].height;

    const vec2 frogLower = vec2_new(frogPosition.x - 0.35f * frogWidth, frogPosition.y - 0.5f * frogHeight);
    const vec2 frogUpper = vec2_new(frogPosition.x + 0.35f * frogWidth, frogPosition.y + 0.5f * frogHeight);
    const vec2 frogCenter = vec2_mul1(vec2_add(frogLower, frogUpper), 0.5f);

    const ldtk::Layer* layer = world.getLevel("Level0")->getLayer("Collisions");
    const ldtk::Tileset* tileset = layer->getTileset();
    const int cellSize = layer->getCellSize();
    for (int x = 0, w = layer->getGridSize().x; x < w; x++)
    {
        for (int y = 0, h = layer->getGridSize().x; y < h; y++)
        {
            const auto& cell = layer->getIntGridVal(x, y);
            if (cell.value == ldtk::IntGridValue::None.value)
            {
                continue;
            }

            const vec2 lower = vec2_new(x * cellSize, layer->level->size.y - y * cellSize - cellSize);
            const vec2 upper = vec2_new(x * cellSize + cellSize, layer->level->size.y - y * cellSize);
            const vec2 center = vec2_mul1(vec2_add(lower, upper), 0.5f);

            const bool isCollided = !(lower.x > frogUpper.x || lower.y > frogUpper.y || upper.x < frogLower.x || upper.y < frogLower.y);
            if (isCollided)
            {
                const vec2 dist = vec2_div1(vec2_sub(frogCenter, center), cellSize * 0.5f);
                const vec2 absDist = vec2_abs(dist);

                if (fabsf(absDist.x - absDist.y) < 0.1f)
                {
                    // If the player is approaching from positive X
                    if (dist.x < 0)
                    {
                        frogPosition.x = lower.x - 0.35f * frogWidth - FLT_EPSILON;
                    }
                    else
                    {
                        // If the player is approaching from negative X
                        frogPosition.x = upper.x + 0.35f * frogWidth + FLT_EPSILON;
                    }

                    if (dist.y < 0)
                    {
                        frogPosition.y = lower.y - 0.5f * frogHeight - FLT_EPSILON;
                    }
                    else
                    {
                        frogPosition.y = upper.y + 0.5f * frogHeight + FLT_EPSILON;
                    }

                    fallSpeed -= fallSpeed * 0.2f;
                    if (fallSpeed < FLT_EPSILON)
                    {
                        fallSpeed = 0.0f;
                    }
                }
                else if (absDist.x > absDist.y)
                {
                    // If the player is approaching from positive X
                    if (dist.x < 0)
                    {
                        frogPosition.x = lower.x - 0.35f * frogWidth - FLT_EPSILON;
                    }
                    else
                    {
                        // If the player is approaching from negative X
                        frogPosition.x = upper.x + 0.35f * frogWidth + FLT_EPSILON;
                    }
                }
                else
                {
                    if (dist.y < 0)
                    {
                        frogPosition.y = lower.y - 0.5f * frogHeight - FLT_EPSILON;
                    }
                    else
                    {
                        frogPosition.y = upper.y + 0.5f * frogHeight + FLT_EPSILON;
                    }

                    fallSpeed -= fallSpeed * 0.2f;
                    if (fallSpeed < FLT_EPSILON)
                    {
                        fallSpeed = 0.0f;
                    }
                }
            }
        }
    }
    #endif
}

void DrawLayer(const char* layerName)
{
    //const ldtk::Layer*      layer       = world.getLevel("Level0")->getLayer(layerName);
    //const ldtk::Tileset*    tileset     = layer->getTileset();
    //const SpriteSheet       spritesheet = spritesheets[tileset->uid];
    //
    //for (ldtk::Tile tile : layer->allTiles())
    //{
    //    const int32_t cols      = tile.texture_position.x / tileset->tile_size;
    //    const int32_t rows      = tile.texture_position.y / tileset->tile_size;
    //    const Sprite* sprite    = &spritesheet.sprites[rows * spritesheet.cols + cols];
    //    Renderer_DrawSprite(sprite, vec2_new(tile.position.x, layer->level->size.y - tile.position.y), 0.0f, vec2_new1(1.0f), vec3_new1(1.0f));
    //}
    const SpriteBatch* spriteBatch = &spriteBatchs[layerName];
    Graphics::DrawSpriteBatch(spriteBatch);
}

void Game_Render()
{
    for (auto pair : spriteBatchs)
    {
        Graphics::DrawSpriteBatch(&pair.second);
    }

    const float frogWidth = frogSpriteSheet->sprites[spriteIndex].width;
    const float frogHeight = frogSpriteSheet->sprites[spriteIndex].height;

    //const vec2 frogPosition = vec2_new(frogBody->GetPosition().x, frogBody->GetPosition().y);
    const vec2 frogPosition = vec2_add(GetWorldPosition(grid, frog), vec2_new(0.0f, frogHeight * 0.25f));
    //const vec2 frogPosition = vec2_mul1(vec2_add(frogLower, frogUpper), 0.5f);
    Graphics::DrawSprite(&frogSpriteSheet->sprites[spriteIndex], frogPosition, 0.0f, frogScale, vec3_new1(1.0f));

    const bool drawCollisionGrid = false;
    if (drawCollisionGrid)
    {
        const vec2 frogLower = vec2_new(frogPosition.x - 0.35f * frogWidth, frogPosition.y - 0.5f * frogHeight);
        const vec2 frogUpper = vec2_new(frogPosition.x + 0.35f * frogWidth, frogPosition.y + 0.5f * frogHeight);
        Graphics::DrawQuadLine(frogLower, frogUpper, vec3_new1(1.0f));

        const int cellSize = grid.size;
        for (int x = 0, w = grid.cols; x < w; x++)
        {
            for (int y = 0, h = grid.rows; y < h; y++)
            {
                if (!HasGridCollision(grid, ivec2{x, y}))
                {
                    continue;
                }

                Graphics::DrawQuadLine(
                    vec2_new(x * cellSize, y * cellSize),
                    vec2_new(x * cellSize + cellSize, y * cellSize + cellSize), vec3_new1(1.0f));
            }
        }
    }
}
