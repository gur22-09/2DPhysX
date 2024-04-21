#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>

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

    Body *bigBall = new Body(Circle(200), Graphics::Width() / 2, Graphics::Height() / 2, 0.0f);
    // Body *smallBall = new Body(Circle(50), 500.0f, 100.0f, 1.0f);
    // this->bodies.push_back(bigBall);
    this->bodies.push_back(bigBall);
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
                running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int x, y;
            SDL_GetMouseState(&x, &y);
            Body *smallBall = new Body(Circle(20), x, y, 1.0);
            bodies.push_back(smallBall);
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update()
{
    Graphics::ClearScreen(0xFF0b2375);
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

    for (auto body : this->bodies)
    {
        // body->addForce(pushForce);

        // Apply weight force
        // Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
        // body->addForce(weight);

        // Apply torque
        //  float torque = 200.0f;
        // body->addTorque(torque);

        // wind force
        // Vec2 windForce = Vec2(20.0f * PIXELS_PER_METER, 0.0f);
        // body->addForce(windForce);
    }

    for (auto body : this->bodies)
    {
        body->isColliding = false;
    }

    for (auto body : this->bodies)
    {
        for (int i = 0; i < this->bodies.size() - 1; ++i)
        {
            for (int j = i + 1; j < this->bodies.size(); ++j)
            {
                Body *a = bodies[i];
                Body *b = bodies[j];
                a->isColliding = false;
                b->isColliding = false;

                Contact contact;

                if (CollisionDetection::isColliding(a, b, contact))
                {
                    // Resolve the collision using the projection method
                    contact.resolvePenetration();

                    // Draw debug contact information
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                    Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFF00FF);
                    a->isColliding = true;
                    b->isColliding = true;
                }
            }
        }
    }

    for (auto body : this->bodies)
    {
        // update bodies
        body->update(deltaTime);

        // confine body within the boundary of the window
        // this is just a hack for now
        if (body->shape->getType() == CIRCLE)
        {
            Circle *cirlce = dynamic_cast<Circle *>(body->shape);

            if (body->position.x <= cirlce->radius)
            {
                body->position.x = cirlce->radius;
                body->velocity.x *= -0.9;
            }
            else if (body->position.x >= Graphics::Width() - cirlce->radius)
            {
                body->position.x = Graphics::Width() - cirlce->radius;
                body->velocity.x *= -0.9;
            }

            if (body->position.y <= cirlce->radius)
            {
                body->position.y = cirlce->radius;
                body->velocity.y *= -0.9;
            }
            else if (body->position.y >= Graphics::Height() - cirlce->radius)
            {
                body->position.y = Graphics::Height() - cirlce->radius;
                body->velocity.y *= -0.9;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{

    for (auto body : this->bodies)
    {
        if (body->shape->getType() == CIRCLE)
        {
            Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFBBEE00;
            Circle *circle = dynamic_cast<Circle *>(body->shape);
            Graphics::DrawCircle((int)body->position.x, (int)body->position.y, circle->radius, body->rotation, color);
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