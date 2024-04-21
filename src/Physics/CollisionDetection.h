#pragma once

#include "Body.h"
#include "Contact.h"

struct CollisionDetection {
    static bool isColliding(Body* a, Body* b, Contact& contact);

    static bool isCollidingCircleCirlce(Body* a, Body* b, Contact& contact);
    static bool isCollidingPolygonPolygon(Body* a, Body* b, Contact& contact);
};