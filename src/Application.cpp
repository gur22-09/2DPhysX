#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning()
{
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
    running = Graphics::OpenWindow();

    Body *body = new Body(Box(200, 100), Graphics::Width() / 2.0f, Graphics::Height() / 2.0f, 1.0f);
    this->bodies.push_back(body);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                this->running = false;
            if (event.key.keysym.sym == SDLK_UP)
            {
                this->pushForce.y = -50 * PIXELS_PER_METER;
            }
            else if (event.key.keysym.sym == SDLK_DOWN)
            {
                this->pushForce.y = 50 * PIXELS_PER_METER;
            }
            else if (event.key.keysym.sym == SDLK_LEFT)
            {
                this->pushForce.x = -50 * PIXELS_PER_METER;
            }
            else if (event.key.keysym.sym == SDLK_RIGHT)
            {
                this->pushForce.x = 50 * PIXELS_PER_METER;
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_UP)
            {
                this->pushForce.y = 0;
            }
            else if (event.key.keysym.sym == SDLK_DOWN)
            {
                this->pushForce.y = 0;
            }
            else if (event.key.keysym.sym == SDLK_LEFT)
            {
                this->pushForce.x = 0;
            }
            else if (event.key.keysym.sym == SDLK_RIGHT)
            {
                this->pushForce.x = 0;
            }
            break;

        case SDL_MOUSEMOTION:
            this->mouseCursor.x = event.motion.x;
            this->mouseCursor.y = event.motion.y;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (!this->leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
            {
                this->leftMouseButtonDown = true;
                int x, y;
                SDL_GetMouseState(&x, &y);
                this->mouseCursor.x = x;
                this->mouseCursor.y = y;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (this->leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
            {
                int lastBody = this->NUM_OF_BODIES - 1;
                this->leftMouseButtonDown = false;
                Vec2 impulseDirection = (this->bodies[lastBody]->position - this->mouseCursor).UnitVector();
                float impulseMagnitude = (this->bodies[lastBody]->position - this->mouseCursor).Magnitude() * 5.0;
                this->bodies[lastBody]->velocity = impulseDirection * impulseMagnitude;
            }
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update()
{
    // Wait some time unti we reach the target frame time in milliseconds
    static int timePreviousFrame{0};
    int delay = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);

    if (delay > 0)
    {
        SDL_Delay(delay);
    }

    // Calculate delta time in  seconds
    float deltaTime = std::min((SDL_GetTicks() - timePreviousFrame) / 1000.0f, 0.016f); // it is capped at 0.016 to prevent invalid delta time values

    // Set time of current frame to be used in next one
    timePreviousFrame = SDL_GetTicks();

    for (auto Body : this->bodies)
    {
        // Body->addForce(pushForce);

        // Apply weight force
        // Vec2 weight = Vec2(0.0f, Body->mass * 9.8f * PIXELS_PER_METER);
        // Body->addForce(weight);

        // Apply torque
        float torque = 200.0f;
        Body->addTorque(torque);
    }

    for (auto Body : this->bodies)
    {
        // update bodies
        Body->update(deltaTime); 

        // confine Body within the boundary of the window
        // this is just a hack for now
        if (Body->shape->getType() == CIRCLE)
        {
            Circle *cirlce = dynamic_cast<Circle *>(Body->shape);

            if (Body->position.x <= cirlce->radius)
            {
                Body->position.x = cirlce->radius;
                Body->velocity.x *= -0.9;
            }
            else if (Body->position.x >= Graphics::Width() - cirlce->radius)
            {
                Body->position.x = Graphics::Width() - cirlce->radius;
                Body->velocity.x *= -0.9;
            }

            if (Body->position.y <= cirlce->radius)
            {
                Body->position.y = cirlce->radius;
                Body->velocity.y *= -0.9;
            }
            else if (Body->position.y >= Graphics::Height() - cirlce->radius)
            {
                Body->position.y = Graphics::Height() - cirlce->radius;
                Body->velocity.y *= -0.9;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
    Graphics::ClearScreen(0xFF0b2375);

    for (auto body : this->bodies)
    {
        if (body->shape->getType() == CIRCLE)
        {
            Circle *circle = dynamic_cast<Circle *>(body->shape);
            Graphics::DrawCircle((int)body->position.x, (int)body->position.y, circle->radius, body->rotation, 0xFFBBEE00);
        }

        if (body->shape->getType() == BOX)
        {
            Box *box = dynamic_cast<Box *>(body->shape);
            Graphics::DrawPolygon((int)body->position.x, (int)body->position.y, box->worldVertices, 0xFFFFFFFF);
        }
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
    for (auto body : this->bodies)
    {
        delete body;
    }

    Graphics::CloseWindow();
}