#pragma once

#include "Vec2.h"
#include "Shape.h"

struct Body {
    bool isColliding;
    // linear motion variables
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 sumForces;

    float mass;
    float invMass;
    
    // angular motion variables
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    float sumTorque;
    float I; // moment of inertia
    float invI;

    // pointer to shape or geometry
    Shape* shape = nullptr;    

    Body(const Shape& shape, float x, float y, float mass);

    ~Body();

    void integrateLinear(float dt);
    void integrateAngular(float dt);
    void update(float dt);
    bool isStatic() const;


    void addForce(const Vec2& force);
    void addTorque(float torque);
    void clearForces();
    void clearTorque();
};

