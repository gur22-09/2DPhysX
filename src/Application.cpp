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

    this->anchor = Vec2(Graphics::Width() / 2.0, 30);

    for(int i = 0; i < this->NUM_OF_PARTICLES; ++i) {
        Particle* bob = new Particle(this->anchor.x, this->anchor.y + (i * this->springRestLength), 2.0f);
        bob->radius = 6;
        this->particles.push_back(bob);
    }
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
                    this->running = false;
                if (event.key.keysym.sym == SDLK_UP) {
                    this->pushForce.y = -50 * PIXELS_PER_METER;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    this->pushForce.y = 50 * PIXELS_PER_METER; 
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    this->pushForce.x = -50 * PIXELS_PER_METER;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    this->pushForce.x = 50 * PIXELS_PER_METER;
                }  
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP) {
                    this->pushForce.y = 0;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    this->pushForce.y = 0; 
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    this->pushForce.x = 0;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    this->pushForce.x = 0;
                }   
               break;

            case SDL_MOUSEMOTION:
                this->mouseCursor.x = event.motion.x;
                this->mouseCursor.y = event.motion.y;
                break;    

            case SDL_MOUSEBUTTONDOWN:
                if (!this->leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    this->leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    this->mouseCursor.x = x;
                    this->mouseCursor.y = y;
                }
               break;   

            case SDL_MOUSEBUTTONUP:
                 if (this->leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    int lastParticle = this->NUM_OF_PARTICLES - 1;
                    this->leftMouseButtonDown = false;
                    Vec2 impulseDirection = (this->particles[lastParticle]->position - this->mouseCursor).UnitVector();
                    float impulseMagnitude = (this->particles[lastParticle]->position - this->mouseCursor).Magnitude() * 5.0;
                    this->particles[lastParticle]->velocity = impulseDirection * impulseMagnitude;
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

 
    for(auto particle: this->particles) {
         particle->addForce(pushForce);

        // Apply a drag force
        Vec2 drag = Force::generateDragForce(*particle, 0.002);
        particle->addForce(drag);

        // Apply weight force
        Vec2 weight = Vec2(0.0f, particle->mass * 9.8f * PIXELS_PER_METER);
        particle->addForce(weight);
    }

    // Apply spring force to particle connected to anchor
    Vec2 springForce = Force::generateSpringForce(*this->particles[0], this->anchor, this->springRestLength, SPRING_CONSTANT);
    this->particles[0]->addForce(springForce); 

    // connect other particles with with spring
    for(int i = 1; i < this->NUM_OF_PARTICLES; ++i) {
        int currIdx = i;
        int prevIdx = i - 1;

        Vec2 springForce = Force::generateSpringForce(*this->particles[currIdx], *this->particles[prevIdx], this->springRestLength, SPRING_CONSTANT);
        this->particles[currIdx]->addForce(springForce);
        this->particles[prevIdx]->addForce(-springForce);
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
    Graphics::ClearScreen(0xFF0b2375);

    //Draw liquid
    // Graphics::DrawFillRect(this->liquid.x + this->liquid.w / 2, this->liquid.y + this->liquid.h / 2, this->liquid.w, this->liquid.h,  0xFF6E3713); 

    
    if (this->leftMouseButtonDown) {
        int lastParticle = this->NUM_OF_PARTICLES - 1;
        Graphics::DrawLine(
            this->particles[lastParticle]->position.x, 
            this->particles[lastParticle]->position.y, 
            this->mouseCursor.x, this->mouseCursor.y, 
            0xFF0000FF
        );
    }

    // draw spring
    Graphics::DrawLine(this->anchor.x, this->anchor.y, this->particles[0]->position.x, this->particles[0]->position.y, 0xFF313131);

    // draw anchor
    Graphics::DrawFillCircle(this->anchor.x, this->anchor.y, 5, 0xFF001155);

    // draw remaining springs
    for(int i = 0; i < this->NUM_OF_PARTICLES - 1; ++i) {
        int currIdx = i;
        int prevIdx = i + 1;

        Graphics::DrawLine(
            this->particles[currIdx]->position.x, 
            this->particles[currIdx]->position.y, 
            this->particles[prevIdx]->position.x, 
            this->particles[prevIdx]->position.y, 
            0xFF313131
        );
    }


    for(auto particle: this->particles) {
        Graphics::DrawFillCircle((int)particle->position.x, (int)particle->position.y, particle->radius, 0xFFBBEE00);
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