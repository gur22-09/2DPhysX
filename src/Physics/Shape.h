#pragma once
#include <vector>
#include "./Vec2.h"

enum ShapeType {
    CIRCLE,
    POLYGON,
    BOX
};

struct Shape {
  virtual ~Shape() = default;
  virtual ShapeType getType() const = 0;
  virtual Shape* clone() const = 0;
};

struct Circle: public Shape {
    float radius;
    Circle(const float radius);

    virtual ~Circle();

    ShapeType getType() const override;

    Shape* clone() const override;
};

struct Polygon: public Shape {
    std::vector<Vec2> vertices;
    
    Polygon() = default;
    Polygon(const std::vector<Vec2> vertices);

    ~Polygon();

    ShapeType getType() const override;

    Shape* clone() const override;
};

struct Box: public Polygon {
    float width;
    float height;

    Box(float width, float height);
    ~Box();

    ShapeType getType() const override;

    Shape* clone() const override;
};