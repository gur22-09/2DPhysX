#include <iostream>
#include "Particle.h"


Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;

    if(this->mass != 0) {
        this->invMass = 1.0f/this->mass;
    } else {
        this->invMass = 0;
    }

    std::cout << "Particle constructed" << "\n";
}


Particle::~Particle() {
    std::cout << "Particle destroyed" << "\n";
}

void Particle::integrate(float dt) {
    // figure the acceleration based on applied force
    this->acceleration = this->sumForces * this->invMass;

    // integrate to find velocity
    this->velocity += this->acceleration * dt;

    this->position += this->velocity * dt +  (this->acceleration * 0.5) * dt * dt;
  
    this->clearForces();
}

void Particle::addForce(const Vec2& force) {
    this->sumForces += force;
}

void Particle::clearForces() {
    this->sumForces = Vec2(0.0f, 0.0f);
}