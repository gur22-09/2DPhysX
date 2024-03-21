#include "Force.h"

Vec2 Force::generateDragForce(const Particle& particle, float k) {
  Vec2 dragForce = Vec2(0, 0);

  if(particle.velocity.MagnitudeSquared() > 0) {
    //Calculate drag force direction which is inverse of velocity vector
    Vec2 dragForceDirection = particle.velocity.UnitVector() * -1.0;

    //Drag Magnitude, K * |v|^2
    float dragMagnitude = k * particle.velocity.MagnitudeSquared();

    //Generate the force
    dragForce = dragForceDirection * dragMagnitude;
  }

  return dragForce;
}

Vec2 Force::generateFrictionForce(const Particle& particle, float k) {
   Vec2 frictionForce = Vec2(0, 0);

   Vec2 frictionForceDirection = particle.velocity.UnitVector() * -1.0;
   
   float frictionMagnitude = k;

   frictionForce = frictionForceDirection * frictionMagnitude;

   return frictionForce; 
}

// gives force of attraction on b by a
Vec2 Force::generateGravitationalForce(const Particle& a, const Particle& b, float G) {
    // calculate distance vector from a to b
    Vec2 d = b.position - a.position;

    Vec2 direction =  d.UnitVector();

    float attractionMagnitude = G * (a.mass * b.mass) / d.MagnitudeSquared();

    Vec2 attractionForce = direction * attractionMagnitude;   
    
    return attractionForce;
}