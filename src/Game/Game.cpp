#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <vectormath.h>

#include "Game/Game.h"
#include "Game/TileMap.h"
#include "Game/Components.h"

#include "Native/Input.h"
#include "Native/Window.h"
#include "Native/FileSystem.h"

#include "Graphics/Graphics.h"
#include "Graphics/SpriteBatch.h"

#include "Misc/Logging.h"
#include "Misc/LDtkParser.h"

#include "Framework/JobSystem.h"

#include <map>
#include <string>

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

static int32_t      spritesheetCount;
static SpriteSheet* spritesheets;

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

Entity          frog;
TileMapGrid*    grid;

inline vec2 GetWorldPosition(int32_t gridCellSize, ivec2 gridPosition, vec2 ratioPosition)
{
    return vec2_new((float)gridCellSize * ((float)gridPosition.x + ratioPosition.x), (float)gridCellSize * ((float)gridPosition.y + ratioPosition.y));
}

inline vec2 GetWorldPosition(const TileMapGrid* grid, const Entity& entity)
{
    return GetWorldPosition(grid->size, entity.gridPosition, entity.ratioPosition);
}

inline bool HasGridCollision(const TileMapGrid* grid, ivec2 position)
{
    return TileMapGrid_SafeGet(grid, position, 0) != 0;
}

inline bool HasEntityCollision(const Entity& a, const Entity& b)
{
    const float zone = a.radius + b.radius;
    const vec2  posA = GetWorldPosition(1, a.gridPosition, a.ratioPosition);
    const vec2  posB = GetWorldPosition(1, a.gridPosition, a.ratioPosition);
    return vec2_distsqr(posA, posB) <= zone * zone;
}

inline void UpdateEntity(Entity* entity, const TileMapGrid* grid, float deltaTime)
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

inline bool EntityOnGround(const Entity& entity, const TileMapGrid* grid)
{
    return (HasGridCollision(grid, ivec2{ entity.gridPosition.x, entity.gridPosition.y - 1 }) && entity.ratioPosition.y <= 0.5f);
}

inline bool EntityOnWall(const Entity& entity, const TileMapGrid* grid)
{
    return !EntityOnGround(entity, grid)
        && ((HasGridCollision(grid, ivec2{ entity.gridPosition.x - 1, entity.gridPosition.y }))
        ||  (HasGridCollision(grid, ivec2{ entity.gridPosition.x + 1, entity.gridPosition.y })));
}

bool CreateSpriteSheet(SpriteSheet* spriteSheet, const LDtkTileset tileset)
{
    int32_t cols = tileset.width / tileset.tileSize;
    int32_t rows = tileset.height / tileset.tileSize;
    
    char absPath[1024];
    if (!FileSystem_GetExistsPath(absPath, sizeof(absPath), tileset.path))
    {
        return false;
    }

    Graphics_LoadSpriteSheet(spriteSheet, absPath, cols, rows);
    return true;
}

void CreateSpriteBatch(const int32_t levelHeight, const LDtkLayer* layer)
{
    const LDtkTileset* tileset = &layer->tileset;
    if (tileset->index < 0 || tileset->index > spritesheetCount || layer->tileCount == 0)
    {
        return;
    }

    const SpriteSheet spritesheet = spritesheets[tileset->index];

    SpriteBatch spriteBatch = {};
    SpriteBatch_Create(&spriteBatch, &spritesheets[tileset->index], (int32_t)layer->tileCount * 6);
    
    SpriteBatch_Begin(&spriteBatch);
    for (int i = 0; i < layer->tileCount; i++)
    {
        const LDtkTile& tile = layer->tiles[i];
        const int32_t cols = tile.textureX / tileset->tileSize;
        const int32_t rows = tile.textureY / tileset->tileSize;
        const Sprite* sprite = &spritesheet.sprites[rows * spritesheet.cols + cols];
        const vec2 position = vec2_new(tile.x + tileset->tileSize * 0.5f, levelHeight - tile.y - tileset->tileSize * 0.5f);
        const vec2 scale = vec2_new(tile.flipX ? -1.0f : 1.0f, tile.flipY ? -1.0f : 1.0f);
        SpriteBatch_DrawSprite(&spriteBatch, sprite, position, 0.0f, scale, vec3_new1(1.0f));
    }
    SpriteBatch_End(&spriteBatch);

    spriteBatchs[layer->name] = spriteBatch;
}

bool Game_Setup()
{
    FileSystem_AddSearchPath("assets");
    FileSystem_AddSearchPath("../../assets");
    FileSystem_AddSearchPath("../../../assets");
    FileSystem_AddSearchPath("../../../../assets");

    char worldPath[1024];
    if (!FileSystem_GetExistsPath(worldPath, sizeof(worldPath), "pixel_adventure.ldtk"))
    {
        return false;
    }

    int32_t tempBufferSize = 20 * 1024 * 1024;
    void* tempBuffer = malloc((size_t)tempBufferSize);

    LDtkContext ldtkContext = LDtkContextDefault(tempBuffer, tempBufferSize);

    LDtkWorld world;
    LDtkError error = LDtkParse(worldPath, ldtkContext, LDtkParseFlags_LayerReverseOrder, &world);
    if (error.code != LDtkErrorCode_None)
    {
        fprintf(stderr, "Parse ldtk sample content failed!: %s\n", error.message);
        free(tempBuffer);
        return false;
    }

    spritesheetCount = world.tilesetCount;
    spritesheets = (SpriteSheet*)malloc(spritesheetCount * sizeof(SpriteSheet));
    for (int32_t i = 0; i < world.tilesetCount; i++)
    {
        LDtkTileset tileset = world.tilesets[i];

        SpriteSheet sheet;
        if (!CreateSpriteSheet(&sheet, tileset))
        {
            free(tempBuffer);
            return false;
        }

        spritesheets[tileset.index] = sheet;
    }

    int32_t levelIndex = 0;
    for (int32_t i = 0; i < world.levelCount; i++)
    {
        if (strcmp(world.levels[i].name, "Level0") == 0)
        {
            levelIndex = i;
            break;
        }
    }

    LDtkLevel level = world.levels[levelIndex];
    for (int32_t i = 0; i < level.layerCount; i++)
    {
        CreateSpriteBatch(level.height, &level.layers[i]);
    }

    int spriteCols_FrogIdle = 11;
    int spriteRows_FrogIdle = 1;
    Graphics_LoadSpriteSheet(&spriteBatch_FrogIdle, "main_characters/frog_idle.png", spriteCols_FrogIdle, spriteRows_FrogIdle);

    int spriteCols_FrogRun = 12;
    int spriteRows_FrogRun = 1;
    Graphics_LoadSpriteSheet(&spriteBatch_FrogRun, "main_characters/frog_run.png", spriteCols_FrogRun, spriteRows_FrogRun);

    int spriteCols_FrogJumpUp = 1;
    int spriteRows_FrogJumpUp = 1;
    Graphics_LoadSpriteSheet(&spriteBatch_FrogJumpUp, "main_characters/frog_jump_up.png", spriteCols_FrogJumpUp, spriteRows_FrogJumpUp);

    int spriteCols_FrogFallDown = 1;
    int spriteRows_FrogFallDown = 1;
    Graphics_LoadSpriteSheet(&spriteBatch_FrogFallDown, "main_characters/frog_fall_down.png", spriteCols_FrogFallDown, spriteRows_FrogFallDown);

    int spriteCols_FrogWallCollide = 5;
    int spriteRows_FrogWallCollide = 1;
    Graphics_LoadSpriteSheet(&spriteBatch_FrogWallCollide, "main_characters/frog_wall_holding.png", spriteCols_FrogWallCollide, spriteRows_FrogWallCollide);

    frogScale = vec2_new(1.0f, 1.0f);
    frogPosition = vec2_new((float)Window_GetWidth() * 0.5f, (float)Window_GetHeight() * 0.5f); // @todo: use graphics get render size

    int32_t collisionLayerIndex = 0;
    for (int32_t i = 0; i < level.layerCount; i++)
    {
        if (strcmp(level.layers[i].name, "Collisions") == 0)
        {
            collisionLayerIndex = i;
            break;
        }
    }
    const LDtkLayer* collisionLayer = &level.layers[collisionLayerIndex];
    grid = TileMapGrid_FromLDtkLayer(collisionLayer);

    frog.radius = 0.0f;
    frog.gridSize = ivec2{ 2, 2 };
    frog.gridPosition = ivec2{ grid->cols >> 1, grid->rows >> 1 };
    frog.ratioVelocity = vec2_new1(0.0f);
    frog.ratioPosition = vec2_new1(0.0f);

    int32_t entitiesLayerIndex = 0;
    for (int32_t i = 0; i < level.layerCount; i++)
    {
        if (strcmp(level.layers[i].name, "Entities") == 0)
        {
            entitiesLayerIndex = i;
            break;
        }
    }
    const LDtkLayer* entitiesLayer = &level.layers[entitiesLayerIndex];

    for (int32_t i = 0; i < entitiesLayer->entityCount; i++)
    {
        const LDtkEntity entity = entitiesLayer->entities[i];
        frog.radius = 0.0f;
        frog.gridSize = ivec2{ 2, 2 };
        frog.gridPosition = ivec2{ entity.gridX, entity.gridY + 2 };
        frog.ratioVelocity = vec2_new1(0.0f);
        frog.ratioPosition = vec2_new1(0.0f);
    }

    free(tempBuffer);
    return true;
}

void Game_Shutdown(void)
{
    TileMapGrid_Destroy(grid);
    grid = nullptr;
}

void Game_Load(void)
{
}

void Game_Unload(void)
{
}

void Game_Update(float totalTime, float deltaTime)
{
    // Skip impossible deltaTime
    if (deltaTime >= 1.0f)
    {
        return;
    }

    const float maxStepTime = 1.0f / 60.0f;

    int32_t stepCount = int32_max((int32_t)(deltaTime / maxStepTime), 1);

    float remainDeltaTime = deltaTime;
    while (stepCount--)
    {
        const float stepTime = float_min(maxStepTime, remainDeltaTime);
        remainDeltaTime -= maxStepTime;

        // @todo: create GameInput module
        if (Input_GetKey(KeyCode_LeftArrow))
        {
            frog.ratioVelocity.x = -4.0f;

            frogScale.x = -fabsf(frogScale.x);
            frogPosition.x -= 100.0f * stepTime; // This is wrong
        }

        if (Input_GetKey(KeyCode_RightArrow))
        {
            frog.ratioVelocity.x = 4.0f;

            frogScale.x = fabsf(frogScale.x);
            frogPosition.x += 100.0f * stepTime; // This is wrong
        }

        if (Input_GetKeyDown(KeyCode_Space))
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

        spriteTimer += stepTime;
        if (spriteTimer >= spriteInterval)
        {
            spriteTimer -= spriteInterval;
            spriteIndex = (spriteIndex + 1) % frogSpriteSheet->spriteCount;
        }

        UpdateEntity(&frog, grid, stepTime);
    }

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
                        frogPosition.x = lower.x - 0.35f * frogWidth - FLOAT_EPSILON;
                    }
                    else
                    {
                        // If the player is approaching from negative X
                        frogPosition.x = upper.x + 0.35f * frogWidth + FLOAT_EPSILON;
                    }

                    if (dist.y < 0)
                    {
                        frogPosition.y = lower.y - 0.5f * frogHeight - FLOAT_EPSILON;
                    }
                    else
                    {
                        frogPosition.y = upper.y + 0.5f * frogHeight + FLOAT_EPSILON;
                    }

                    fallSpeed -= fallSpeed * 0.2f;
                    if (fallSpeed < FLOAT_EPSILON)
                    {
                        fallSpeed = 0.0f;
                    }
                }
                else if (absDist.x > absDist.y)
                {
                    // If the player is approaching from positive X
                    if (dist.x < 0)
                    {
                        frogPosition.x = lower.x - 0.35f * frogWidth - FLOAT_EPSILON;
                    }
                    else
                    {
                        // If the player is approaching from negative X
                        frogPosition.x = upper.x + 0.35f * frogWidth + FLOAT_EPSILON;
                    }
                }
                else
                {
                    if (dist.y < 0)
                    {
                        frogPosition.y = lower.y - 0.5f * frogHeight - FLOAT_EPSILON;
                    }
                    else
                    {
                        frogPosition.y = upper.y + 0.5f * frogHeight + FLOAT_EPSILON;
                    }

                    fallSpeed -= fallSpeed * 0.2f;
                    if (fallSpeed < FLOAT_EPSILON)
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
    Graphics_DrawSpriteBatch(spriteBatch);
}

static bool s_drawCollisionGrid = true;

void Game_Render(void)
{
    for (auto pair : spriteBatchs)
    {
        Graphics_DrawSpriteBatch(&pair.second);
    }

    const float frogWidth = frogSpriteSheet->sprites[spriteIndex].width;
    const float frogHeight = frogSpriteSheet->sprites[spriteIndex].height;

    //const vec2 frogPosition = vec2_new(frogBody->GetPosition().x, frogBody->GetPosition().y);
    const vec2 frogPosition = vec2_add(GetWorldPosition(grid, frog), vec2_new(0.0f, frogHeight * 0.25f));
    //const vec2 frogPosition = vec2_mul1(vec2_add(frogLower, frogUpper), 0.5f);
    Graphics_DrawSprite(&frogSpriteSheet->sprites[spriteIndex], vec2_new(frogPosition.x, frogPosition.y), 0.0f, frogScale, vec3_new1(1.0f));

    if (s_drawCollisionGrid)
    {
        const vec2 frogLower = vec2_new(frogPosition.x - 0.35f * frogWidth, frogPosition.y - 0.5f * frogHeight);
        const vec2 frogUpper = vec2_new(frogPosition.x + 0.35f * frogWidth, frogPosition.y + 0.5f * frogHeight);
        Graphics_DrawQuadLine(frogLower, frogUpper, vec3_new1(1.0f));

        const float cellSize = (float)grid->size;
        for (int32_t y = 0, h = grid->rows; y < h; y++)
        {
            for (int32_t x = 0, w = grid->cols; x < w; x++)
            {
                if (!HasGridCollision(grid, ivec2{x, y}))
                {
                    continue;
                }

                Graphics_DrawQuadLine(
                    vec2_new((float)x * cellSize, (float)y * cellSize),
                    vec2_new((float)x * cellSize + cellSize, (float)y * cellSize + cellSize), vec3_new1(1.0f));
            }
        }
    }
}

#include <imgui/imgui.h>
void Game_RenderDevTools(void)
{
    ImGui::Begin("Game Debug", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

    bool settingsChanged = false;
    if (ImGui::Checkbox("Draw Collision Grid", &s_drawCollisionGrid))
    {
        settingsChanged = true;
    }

    if (settingsChanged)
    {
        Log_Info("Game", "Settings change, saving...");
    }

    ImGui::End();
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
