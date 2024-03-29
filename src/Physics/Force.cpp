#include "Force.h"
#include <cmath>
#include <algorithm>

Vec2 Force::generateDragForce(const Body& Body, float k) {
  Vec2 dragForce = Vec2(0, 0);

  if(Body.velocity.MagnitudeSquared() > 0) {
    //Calculate drag force direction which is inverse of velocity vector
    Vec2 dragForceDirection = Body.velocity.UnitVector() * -1.0;

    //Drag Magnitude, K * |v|^2
    float dragMagnitude = k * Body.velocity.MagnitudeSquared();

    //Generate the force
    dragForce = dragForceDirection * dragMagnitude;
  }

  return dragForce;
}

Vec2 Force::generateFrictionForce(const Body& Body, float k) {
   Vec2 frictionForce = Vec2(0, 0);

   if(Body.velocity.Magnitude() == 0) {
     return frictionForce;
   }

   Vec2 frictionForceDirection = Body.velocity.UnitVector() * -1.0;
    
   float frictionMagnitude = k;

   frictionForce = frictionForceDirection * frictionMagnitude;

   return frictionForce; 
}

// gives force of attraction on a by b
Vec2 Force::generateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance) {
    // calculate distance vector from a to b
    Vec2 d = b.position - a.position;

    Vec2 direction =  d.UnitVector();

    float magnitude = std::clamp(d.MagnitudeSquared(), minDistance, maxDistance);

    float attractionMagnitude = G * (a.mass * b.mass) / magnitude;

    Vec2 attractionForce = direction * attractionMagnitude;   
    
    return attractionForce;
}

Vec2 Force::generateSpringForce(const Body& Body, Vec2& anchor, float restLength, float k) {
  // distance between anchor and Body
  Vec2 distance = Body.position - anchor;
  
  // displacement magnitude
  float displacementMag = distance.Magnitude() - restLength;

  Vec2 springForce = distance.UnitVector() * displacementMag * -k;

  return springForce;
}


Vec2 Force::generateSpringForce(const Body& a, const Body& b, float restLength, float k) {
  // distance between anchor and Body
  Vec2 distance = a.position - b.position;
  
  // displacement magnitude
  float displacementMag = distance.Magnitude() - restLength;

  Vec2 springForce = distance.UnitVector() * displacementMag * -k;

  return springForce;
}