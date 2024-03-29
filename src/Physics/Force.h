#pragma once

#include "./Vec2.h"
#include "./Particle.h"

struct  Force
{
    static Vec2 generateDragForce(const Particle& particle, float k);

    static Vec2 generateFrictionForce(const Particle& particle, float k);

    static Vec2 generateGravitationalForce(const Particle& a, const Particle& b, float G, float minDistance, float maxDistance);

    static Vec2 generateSpringForce(const Particle& particle, Vec2& anchor, float restLength, float k);

    static Vec2 generateSpringForce(const Particle& a, const Particle& b, float restLength, float k);
};

