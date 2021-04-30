#pragma once

#include <string>
#include "universal/types.h"
#include "platform/application.h"

#define MAX_CHILDREN 4

// I know all of them are not planets but I'm too lazy to go
// change every planet to celestialBody everywhere :P
struct PlanetData
{
    std::string name;
    std::string desc;
    Vec4 color;
    
    Vec3 position;
    f32  radius;
    f32  axialTilt;
    f32  surfaceTemp;

    // For info
    f32 rotationalPeriod;
    f32 orbitalPeriod;
    f32 orbitalRadius;

    // For simulation
    f32 spinSpeed;
    f32 orbitRadius;
    f32 orbitSpeed;
    
    s32 childrenCount;
    s32 children[MAX_CHILDREN];

    bool isSpecial;
    Vec3 cameraOffset;
};

void UpdatePlanets(Application* app, PlanetData planets[]);