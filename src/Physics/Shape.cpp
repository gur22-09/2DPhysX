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

float Circle::getMomentOfInertia() const {
    // still needs to be multiplied by rigid body mass
    return 0.5 * (this->radius * this->radius);
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

float Polygon::getMomentOfInertia() const {
    // TODO
    return 0.0;
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

float Box::getMomentOfInertia() const {
    return 0.083333 * (this->width * this->width + this->height * this->height);
}

Box::~Box() {

}


