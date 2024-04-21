#include "CollisionDetection.h"

bool CollisionDetection::isColliding(Body *a, Body *b, Contact &contact)
{
    bool aIsCircle = a->shape->getType() == CIRCLE;
    bool bIsCircle = b->shape->getType() == CIRCLE;

    if (aIsCircle && bIsCircle)
    {
        return isCollidingCircleCirlce(a, b, contact);
    }

    return false;
};

bool CollisionDetection::isCollidingCircleCirlce(Body *a, Body *b, Contact &contact)
{
    Circle *aCircleShape = dynamic_cast<Circle *>(a->shape);
    Circle *bCircleShape = dynamic_cast<Circle *>(b->shape);

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding)
    {
        return false;
    }

    contact.a = a;
    contact.b = b;

    contact.normal = ab;
    contact.normal.Normalize();

    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;

    contact.depth = (contact.end - contact.start).Magnitude();

    return true;
}
