#include <iostream>
#include "Body.h"


Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.clone();
    this->position = Vec2(x, y);
    this->mass = mass;

    if(this->mass != 0) {
        this->invMass = 1.0f/this->mass;
    } else {
        this->invMass = 0;
    }

    std::cout << "Body constructed" << "\n";
}


Body::~Body() {
    delete shape;
    std::cout << "Body destroyed" << "\n";
}

void Body::integrate(float dt) {
    // figure the acceleration based on applied force
    this->acceleration = this->sumForces * this->invMass;

    // integrate to find velocity
    this->velocity += this->acceleration * dt;

    this->position += this->velocity * dt;
  
    this->clearForces();
}

void Body::addForce(const Vec2& force) {
    this->sumForces += force;
}

void Body::clearForces() {
    this->sumForces = Vec2(0.0f, 0.0f);
}