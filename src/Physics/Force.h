#pragma once

#include "./Vec2.h"
#include "./Body.h"

struct  Force
{
    static Vec2 generateDragForce(const Body& Body, float k);

    static Vec2 generateFrictionForce(const Body& Body, float k);

    static Vec2 generateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance);

    static Vec2 generateSpringForce(const Body& Body, Vec2& anchor, float restLength, float k);

    static Vec2 generateSpringForce(const Body& a, const Body& b, float restLength, float k);
};

