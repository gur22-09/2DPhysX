#include "./Shape.h"
#include <iostream>

Circle::Circle(const float radius)
{
  this->radius = radius;
  std::cout << "Circle Constructed" << "\n";
}

ShapeType Circle::getType() const
{
  return CIRCLE;
}

Shape *Circle::clone() const
{
  return new Circle(this->radius);
}

float Circle::getMomentOfInertia() const
{
  // still needs to be multiplied by rigid body mass
  return 0.5 * (this->radius * this->radius);
}

Circle::~Circle()
{
  std::cout << "Circle Destroyed" << "\n";
}

Polygon::Polygon(const std::vector<Vec2> vertices)
{
}

ShapeType Polygon::getType() const
{
  return POLYGON;
}

Shape *Polygon::clone() const
{
  return new Polygon(this->localVertices);
}

float Polygon::getMomentOfInertia() const
{
  // TODO
  return 0.0;
}

void Polygon::updateVertices(float angle, const Vec2 &positon)
{
  // transform vertices to world space
  for (int i = 0; i < this->localVertices.size(); i++)
  {
    // rotate
    this->worldVertices[i] = this->localVertices[i].Rotate(angle);

    // translate
    this->worldVertices[i] += positon;
  }
}

Polygon::~Polygon()
{
}

Box::Box(float width, float height)
{
  this->width = width;
  this->height = height;
  std::cout << "Box Constructed" << "\n";

  // load vertices of box polygon
  this->localVertices.push_back(Vec2(-width / 2.0f, -height / 2.0f));
  this->localVertices.push_back(Vec2(+width / 2.0f, -height / 2.0f));
  this->localVertices.push_back(Vec2(+width / 2.0f, +height / 2.0f));
  this->localVertices.push_back(Vec2(-width / 2.0f, +height / 2.0f));


  this->worldVertices.push_back(Vec2(-width / 2.0f, -height / 2.0f));
  this->worldVertices.push_back(Vec2(+width / 2.0f, -height / 2.0f));
  this->worldVertices.push_back(Vec2(+width / 2.0f, +height / 2.0f));
  this->worldVertices.push_back(Vec2(-width / 2.0f, +height / 2.0f));
}

ShapeType Box::getType() const
{
  return BOX;
}

Shape *Box::clone() const
{
  return new Box(this->width, this->height);
}

float Box::getMomentOfInertia() const
{
  return 0.083333 * (this->width * this->width + this->height * this->height);
}

Box::~Box()
{
}
