#include "planet.h"

static void UpdatePlanet(Application* app, PlanetData planets[], const Vec3 parentPosition, s32 index)
{
    PlanetData& planet = planets[index];
    f32 angle = planet.orbitSpeed * app->time;
    planet.position = Vec3 { cosf(angle), 0.0f, sinf(angle) } * planet.orbitRadius + parentPosition;

    for (int i = 0; i < planet.childrenCount; i++)
        UpdatePlanet(app, planets, planet.position, planet.children[i]);
}

void UpdatePlanets(Application* app, PlanetData planets[])
{
    UpdatePlanet(app, planets, Vec3 {}, 0);
}