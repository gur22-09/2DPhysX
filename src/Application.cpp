#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    Particle* particle = new Particle((int)(Graphics::Width() / 2), (int)(Graphics::Height() / 2), 1.0);
    particle->radius = 15;
    particles.push_back(particle);

   this->liquid.x = 0;
   this->liquid.y = Graphics::Height() / 2;
   this->liquid.w = Graphics::Width();
   this->liquid.h = Graphics::Height() / 2;
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP) {
                    pushForce.y = -50 * PIXELS_PER_METER;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    pushForce.y = 50 * PIXELS_PER_METER; 
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    pushForce.x = -50 * PIXELS_PER_METER;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    pushForce.x = 50 * PIXELS_PER_METER;
                }  
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP) {
                    pushForce.y = 0;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    pushForce.y = 0; 
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    pushForce.x = 0;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    pushForce.x = 0;
                }   
               break;

            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
            break;    

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    int x,  y;

                    SDL_GetMouseState(&x, &y);

                    // Add new particle
                    Particle* particle = new Particle(x, y, 1.0f);
                    particle->radius = 5;
                    this->particles.push_back(particle);
                } 

                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
            break;   

            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                    particles[0]->velocity = impulseDirection * impulseMagnitude;
                }
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
   // Wait some time unti we reach the target frame time in milliseconds
   static int timePreviousFrame {0};
   int delay = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
   
   if(delay > 0) {
    SDL_Delay(delay);
   }

   //Calculate delta time in  seconds
   float deltaTime = std::min((SDL_GetTicks() - timePreviousFrame) / 1000.0f, 0.016f); // it is capped at 0.016 to prevent invalid delta time values

   // Set time of current frame to be used in next one 
   timePreviousFrame = SDL_GetTicks();


    // Update objects in scene
    // apply wind force, weight to particle and add forces
    for(auto particle: this->particles) {
        // Vec2 windForce = Vec2(static_cast<float>(0.2 * PIXELS_PER_METER), 0.0f);
        // particle->addForce(windForce);

        // Vec2 weight = Vec2(0.0f, static_cast<float>(particle->mass * 9.8f * PIXELS_PER_METER));
        // particle->addForce(weight);
         // Apply a friction force

        Vec2 frictionForce = Force::generateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
        particle->addForce(frictionForce);

        particle->addForce(this->pushForce);

        // Apply a drag force if inside a liquid
        if(particle->position.y >= this->liquid.y ) {
            Vec2 daragForce = Force::generateDragForce(*particle, 0.02);
            particle->addForce(daragForce);
        }
    }

    
    for(auto particle: this->particles) {
        // Integrate acceleration and velocity to find updated position and velocity
        particle->integrate(deltaTime);

        // confine particle within the boundary of the window
        // this is just a hack for now
        if(particle->position.x <= particle->radius) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9;
        }else if (particle->position.x >= Graphics::Width() - particle->radius) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9;
        }

        if(particle->position.y <= particle->radius) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        }else if (particle->position.y >= Graphics::Height() - particle->radius) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0x01FF0000);

    //Draw liquid
    // Graphics::DrawFillRect(this->liquid.x + this->liquid.w / 2, this->liquid.y + this->liquid.h / 2, this->liquid.w, this->liquid.h,  0xFF6E3713); 

    
    if (leftMouseButtonDown) {
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
    }

    for(auto particle: this->particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle: this->particles) {
        delete particle;
    }
    
    Graphics::CloseWindow();
}