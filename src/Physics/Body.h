#pragma once

#include "Vec2.h"
#include "Shape.h"

struct Body {
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 sumForces;

    float mass;
    float invMass;

    Shape* shape = nullptr;    

    Body(const Shape& shape, float x, float y, float mass);

    ~Body();

    void integrate(float dt);

    void addForce(const Vec2& force);
    void clearForces();
};

