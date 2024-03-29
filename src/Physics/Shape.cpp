#include "./Shape.h"
#include <iostream>

Circle::Circle(const float radius) {
    this->radius = radius;
    std::cout << "Circle Constructed" << "\n";
}

ShapeType Circle::getType() const {
  return CIRCLE;
}

Shape* Circle::clone() const  {
  return new Circle(this->radius);
}

Circle::~Circle() {
    std::cout << "Circle Destroyed" << "\n";
}

Polygon::Polygon(const std::vector<Vec2> vertices) {

}

ShapeType Polygon::getType() const  {
    return POLYGON;
}

Shape* Polygon::clone() const {
    return new Polygon(this->vertices);
}

Polygon::~Polygon() {

}

Box::Box(float width, float heigt) {

}

ShapeType Box::getType() const {
    return BOX;
}

Shape* Box::clone() const {
    return new Box(this->width, this->height);
}

Box::~Box() {

}


