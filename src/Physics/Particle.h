#pragma once

#include "Vec2.h"

struct Particle {
    int radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 sumForces;

    float mass;
    float invMass;

    Particle(float x, float y, float mass);

    ~Particle();

    void integrate(float dt);

    void addForce(const Vec2& force);
    void clearForces();
};

