#include "Contact.h"
#include <iostream>

void Contact::resolvePenetration()
{
    if (a->isStatic() && b->isStatic())
    {
        return;
    }

    float da = depth / (a->invMass + b->invMass) * a->invMass;
    float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db;
}