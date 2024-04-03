#pragma once
#include <vector>
#include "./Vec2.h"

enum ShapeType
{
    CIRCLE,
    POLYGON,
    BOX
};

struct Shape
{
    virtual ~Shape() = default;
    virtual ShapeType getType() const = 0;
    virtual Shape *clone() const = 0;
    virtual float getMomentOfInertia() const = 0;
};

struct Circle : public Shape
{
    float radius;
    Circle(const float radius);

    virtual ~Circle();

    ShapeType getType() const override;

    Shape *clone() const override;

    float getMomentOfInertia() const override;
};

struct Polygon : public Shape
{
    std::vector<Vec2> localVertices;
    std::vector<Vec2> worldVertices;

    Polygon() = default;
    Polygon(const std::vector<Vec2> vertices);

    ~Polygon();

    ShapeType getType() const override;

    Shape *clone() const override;

    float getMomentOfInertia() const override;

    void updateVertices(float angle, const Vec2 &position);
};

struct Box : public Polygon
{
    float width;
    float height;

    Box(float width, float height);
    ~Box();

    ShapeType getType() const override;

    Shape *clone() const override;

    float getMomentOfInertia() const override;
};