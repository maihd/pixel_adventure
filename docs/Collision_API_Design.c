#include <stdbool.h>

typedef struct CircleShape
{
    float x, y;
    float radius;
} CircleShape;

typedef struct PolygonShape
{
    float* xs;
    float* ys;
} PolygonShape;

typedef enum ShapeType
{
    ShapeType_Circle,
    ShapeType_Polygon,
    ShapeType_COUNT,
} ShapeType;

typedef struct Shape
{
    ShapeType type;
    union
    {
        CircleShape circle;
        PolygonShape polygon;
    };
} Shape;

typedef bool (*Shape_IsCollideFn)(const Shape* a, const Shape* b);

bool Shape_IsCollide_Circle_Circle(const Shape* a, const Shape* b);
bool Shape_IsCollide_Circle_Polygon(const Shape* a, const Shape* b);
bool Shape_IsCollide_Polygon_Circle(const Shape* a, const Shape* b);
bool Shape_IsCollide_Polygon_Polygon(const Shape* a, const Shape* b);

static Shape_IsCollideFn IsCollideFnLookup[ShapeType_COUNT][ShapeType_COUNT] = {
    { Shape_IsCollide_Circle_Circle, Shape_IsCollide_Circle_Polygon },
    { Shape_IsCollide_Polygon_Circle, Shape_IsCollide_Polygon_Polygon },
};

bool Shape_IsCollide(const Shape* a, const Shape* b)
{
#ifdef USE_LOOKUP_TABLE
    return IsCollideFnLookup[a->type][b->type](a, b);
#else
    switch (a->type)
    {
        case ShapeType_Circle:
            switch (b->type)
            {
                case ShapeType_Circle:
                    return IsCollide_Circle_Circle(a, b);

                case ShapeType_Polygon:
                    return IsCollide_Circle_Polygon(a, b);
            }

        case ShapeType_Polygon:
            switch (b->type)
            {
                case ShapeType_Circle:
                    return IsCollide_Circle_Polygon(b, a);

                case ShapeType_Polygon:
                    return IsCollide_Polygon_Polygon(a, b);
            }
    }
#endif
}