#pragma once

#include <vector>
#include "./Graphics.h"
#include "./Physics/Particle.h"

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles; 
        Vec2 pushForce =  Vec2(0, 0);
        Vec2 mouseCursor = Vec2(0, 0);
        bool leftMouseButtonDown = false;

        SDL_Rect liquid;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

