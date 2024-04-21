#include <iostream>
#include <cmath>
#include "Body.h"

Body::Body(const Shape &shape, float x, float y, float mass)
{
    this->shape = shape.clone();
    this->position = Vec2(x, y);
    this->mass = mass;
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);

    this->rotation = 0;
    this->angularVelocity = 0;
    this->angularAcceleration = 0;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0;

    if (this->mass != 0)
    {
        this->invMass = 1.0f / this->mass;
    }
    else
    {
        this->invMass = 0;
    }

    this->I = shape.getMomentOfInertia() * mass;

    if (this->I != 0)
    {
        this->invI = 1.0f / this->I;
    }
    else
    {
        this->invI = 0;
    }

    std::cout << "Body constructed" << "\n";
}

Body::~Body()
{
    delete shape;
    std::cout << "Body destroyed" << "\n";
}

void Body::integrateLinear(float dt)
{
    if(Body::isStatic()) {
        return;
    }
    // figure the acceleration based on applied force
    this->acceleration = this->sumForces * this->invMass;

    // integrate to find velocity
    this->velocity += this->acceleration * dt;

    this->position += this->velocity * dt;

    this->clearForces();
}

void Body::integrateAngular(float dt)
{
    if(Body::isStatic()) {
        return;
    }
    // figure the angular acceleration based on torque
    this->angularAcceleration = this->sumTorque * this->invI;

    // integrate angular acceleration to find angular velocity
    this->angularVelocity += this->angularAcceleration * dt;

    // integrate angular velocity to find rotation
    this->rotation += this->angularVelocity * dt;

    this->clearTorque();
}

void Body::addForce(const Vec2 &force)
{
    this->sumForces += force;
}

void Body::clearForces()
{
    this->sumForces = Vec2(0.0f, 0.0f);
}

void Body::clearTorque()
{
    this->sumTorque = 0;
}

void Body::addTorque(float torque)
{
    this->sumTorque += torque;
}

void Body::update(float dt)
{
    this->integrateLinear(dt);
    this->integrateAngular(dt);
    ShapeType type = this->shape->getType();
    bool isPolygon = type == POLYGON || type == BOX;
   
    if (isPolygon)
    {
        Polygon *polygon = dynamic_cast<Polygon *>(this->shape);
        polygon->updateVertices(this->rotation, this->position);
    }
}

bool Body::isStatic() const {
    const float epsilon = 0.005f;
    return fabs(this->invI - 0.0f)  < epsilon; 
}