// Disclaimer: The simulation is not accurate. Pls don't come at me!

#include <cmath>
#include <string>
#include "platform/application.h"
#include "universal/types.h"
#include "universal/math.h"
#include "engine/ui.h"
#include "engine/mesh.h"
#include "engine/texture.h"
#include "engine/sprite.h"
#include "engine/uniform_buffer.h"
#include "simulation/planet.h"
#include "data/planet_data.h"
#include "data/texture_data.h"

UI::Font font;
Vec4 white { 1.0f, 1.0f, 1.0f, 1.0f };
s32 selectedPlanet = 0;

Texture texs[sizeof(planets) / sizeof(PlanetData) + 2];     // Including earth_night and sky
SpriteAtlas saturnRingAtlas;
Sprite saturnRingsSprite;

Mesh sphere;
Mat4 v, p;

Shader sunShader,
       planetShader,
       earthShader,
       spriteShader;

Vec3 cameraPos { 0.0f, 200.0f, 0.0f };
UniformBuffer<Mat4> vpUniformBuffer;

#ifdef DEBUG
bool showStats = false;
#endif

int main()
{
#   ifdef DEBUG
    Application app("Solar System", 1280, 720, false, true);
    app.SetReferenceResolution(1080);
#   else
    Application app("Solar System", 1920, 1080, true, false);
#   endif
    app.SetWindowIcon("res/icons/solar-system-icon.png");
    app.SetVsync(true);

    app.onInit = [](Application* app)
    {
        saturnRingAtlas.Load("res/images/saturn_ring.png");
        saturnRingsSprite.Set({ 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });

        for (int i = 0; i < sizeof(texs) / sizeof(Texture); i++)
            texs[i].Load(texture_files[i]);

        font.Load("res/fonts/Inconsolata.ttf", 25.0f);
        sphere.LoadFromOBJ("res/models/sphere.obj");
        
        sunShader.LoadShader("res/shaders/sun.vert", Shader::Type::VERTEX_SHADER);
        sunShader.LoadShader("res/shaders/sun.frag", Shader::Type::FRAGMENT_SHADER);
        sunShader.Compile();

        earthShader.LoadShader("res/shaders/earth.vert", Shader::Type::VERTEX_SHADER);
        earthShader.LoadShader("res/shaders/earth.frag", Shader::Type::FRAGMENT_SHADER);
        earthShader.Compile();

        planetShader.LoadShader("res/shaders/planet.vert", Shader::Type::VERTEX_SHADER);
        planetShader.LoadShader("res/shaders/planet.frag", Shader::Type::FRAGMENT_SHADER);
        planetShader.Compile();

        spriteShader.LoadShader("res/shaders/sprite.vert", Shader::Type::VERTEX_SHADER);
        spriteShader.LoadShader("res/shaders/sprite.frag", Shader::Type::FRAGMENT_SHADER);
        spriteShader.Compile();

        p = Mat4::Perspective(
            45.0f,
            (f32) app->screenWidth / (f32) app->screenHeight,
            1.0f, 4400.0f
        );

        vpUniformBuffer = UniformBuffer<Mat4>(0);
    };

    app.onUpdate = [](Application* app)
    {
        if (app->GetKeyDown(KEY(ESCAPE)))
        {
            app->Exit();
            return;
        }

#       ifdef DEBUG
        if (app->GetKeyDown(KEY(GRAVE_ACCENT)))
        {
            showStats = !showStats;
        }
#       endif

        if (app->GetKeyDown(KEY(E)) ||
            app->GetKeyDown(KEY(RIGHT)))
        {
            selectedPlanet = (selectedPlanet + 1) % (sizeof(planets) / sizeof(PlanetData));
        }

        if (app->GetKeyDown(KEY(Q)) ||
            app->GetKeyDown(KEY(LEFT)))
        {
            selectedPlanet = (selectedPlanet - 1);
            if (selectedPlanet < 0)
            {
                selectedPlanet = (sizeof(planets) / sizeof(PlanetData)) - 1;
            }
        }

        {   // Update planet positions
            UpdatePlanets(app, planets);
        }

        {   // Update Camera Position
            PlanetData& planet = planets[selectedPlanet];
            cameraPos = Lerp(planet.position + planet.cameraOffset, cameraPos, (7.5f * planet.orbitSpeed + 1.0f) * app->deltaTime);
            v = Mat4::LookAt(
                cameraPos,
                planet.position,
                Vec3 { 0.0f, 1.0f, 0.0f }
            );
        }
    };

    app.onRender = [](Application* app)
    {
        {   // All planets
            sphere.Bind();

            // This will work for both sky and sun
            sunShader.Bind();

            // Set View Projection
            Mat4 viewProj = p * v;
            vpUniformBuffer.SetData(viewProj);  // For Earth and other planets

            {   // Sky
                // The back face is being used to act like a cube map
                glCullFace(GL_FRONT);
                glDisable(GL_DEPTH_TEST);

                Vec3 scale = Vec3 { 10.0f, 10.0f, 10.0f };
                Mat4 m = Mat4::Scaling(scale).Translate(cameraPos);
                sunShader.SetUniformMat4("u_mvp", false, viewProj * m);
                
                texs[(sizeof(texs) / sizeof(Texture)) - 1].Bind(0);
                sunShader.SetUniform1i("u_tex", 0);
                sphere.Draw();

                glEnable(GL_DEPTH_TEST);
                glCullFace(GL_BACK);
            }


            {   // Sun
                PlanetData& sun = planets[0];
                sunShader.Bind();

                Vec3 scale = Vec3 { 1.0f, 1.0f, 1.0f } * sun.radius;
                f32 rotationalSpeed = 1.0f / sun.rotationalPeriod;
                Mat4 m = Mat4::Scaling(scale).Rotate({ 0.0f, 1.0f, 0.0f }, app->time * rotationalSpeed).Rotate({ 0.0f, 0.0f, 1.0f }, ToRad(-sun.axialTilt));
                sunShader.SetUniformMat4("u_mvp", false, viewProj * m);

                texs[0].Bind(0);
                sunShader.SetUniform1i("u_tex", 0);

                sphere.Draw();
            }

            {   // Earth
                PlanetData& earth = planets[3];
                earthShader.Bind();

                texs[3].Bind(0);
                earthShader.SetUniform1i("u_dayTex", 0);
                texs[(sizeof(texs) / sizeof(Texture)) - 2].Bind(1);
                earthShader.SetUniform1i("u_nightTex", 1);

                Vec3 scale = Vec3 { 1.0f, 1.0f, 1.0f } * earth.radius;
                f32 rotationalSpeed = 1.0f / earth.rotationalPeriod;
                Mat4 m = Mat4::Scaling(scale).Rotate({ 0.0f, 1.0f, 0.0f }, app->time * rotationalSpeed).Rotate({ 0.0f, 0.0f, 1.0f }, ToRad(-earth.axialTilt)).Translate(earth.position);
                earthShader.SetUniformMat4("u_model", false, m);
    
                Vec3 sunToEarthVector = planets[0].position - earth.position;
                earthShader.SetUniform3f("u_sunToEarthVector", sunToEarthVector.x, sunToEarthVector.y, sunToEarthVector.z);

                sphere.Draw();
            }

            {   // Rest of the planets
                planetShader.Bind();
                
                for (int i = 1; i < sizeof(planets) / sizeof(PlanetData); i++)
                {
                    PlanetData& planet = planets[i];

                    if (planet.isSpecial)
                        continue;

                    Vec3 scale = Vec3 { 1.0f, 1.0f, 1.0f } * planet.radius;
                    f32 rotationalSpeed = 1.0f / planet.rotationalPeriod;
                    Mat4 m = Mat4::Scaling(scale).Rotate({ 0.0f, 1.0f, 0.0f }, app->time * rotationalSpeed).Rotate({ 0.0f, 0.0f, 1.0f }, ToRad(-planet.axialTilt)).Translate(planet.position);
                    planetShader.SetUniformMat4("u_model", false, m);

                    texs[i].Bind(0);
                    planetShader.SetUniform1i("u_tex", 0);

                    planetShader.SetUniform3f("u_lightPos", planets[0].position.x, planets[0].position.y, planets[0].position.z);

                    sphere.Draw();
                }
            }
            
            {   // Saturn's Rings
                glDisable(GL_CULL_FACE);

                spriteShader.Bind();

                PlanetData& saturn = planets[7];
                Mat4 m = Mat4::Scaling( Vec3 { 1.0f, 1.0f, 1.0f } * (saturn.radius * 5.0f)).Rotate({ 0.0f, 1.0f, 0.0f }, _PI / 2.0f).Rotate({ 0.0f, 0.0f, 1.0f }, -_PI / 2.0f + ToRad(-saturn.axialTilt)).Translate(saturn.position);
                spriteShader.SetUniformMat4("u_mat", false, viewProj * m);

                saturnRingAtlas.Bind(0);
                spriteShader.SetUniform1i("u_atlas", 0);

                saturnRingsSprite.Draw();

                glEnable(GL_CULL_FACE);
            }
        }

        UI::Begin();

        {   // Details
            Vec2 topLeft { 10.0f, 10.0f };
            f32 gap = 10.0f;
            char buffer[64];

            {   // Name
                Vec2 size = UI::GetRenderedTextSize(planets[selectedPlanet].name, font);
                UI::RenderText(app, planets[selectedPlanet].name, font, planets[selectedPlanet].color, topLeft, 0.0f);
                topLeft.y += size.y + gap;
            }

            {   // Description
                Vec2 size = UI::GetRenderedTextSize(planets[selectedPlanet].desc, font);
                UI::RenderText(app, planets[selectedPlanet].desc, font, white, topLeft, 0.0f);
                topLeft.y += size.y + gap;
            }

            topLeft.y += gap + font.fontHeight;

            {
                std::string text = "More Information:";
                Vec2 size = UI::GetRenderedTextSize(text, font);
                UI::RenderText(app, text, font, planets[selectedPlanet].color, topLeft, 0.0f);
                topLeft.y += size.y + gap;
            }

            {   // Orbital Period
                sprintf(buffer, "Orbital Period      : %.3f earth years", planets[selectedPlanet].orbitalPeriod);
                std::string text = buffer;
                Vec2 size = UI::GetRenderedTextSize(text, font);
                UI::RenderText(app, text, font, white, topLeft, 0.0f);
                topLeft.y += size.y + gap;
            }

            {   // Rotational Period
                sprintf(buffer, "Rotational Period   : %.3f earth days", planets[selectedPlanet].rotationalPeriod);
                std::string text = buffer;
                Vec2 size = UI::GetRenderedTextSize(text, font);
                UI::RenderText(app, text, font, white, topLeft, 0.0f);
                topLeft.y += size.y + gap;
            }

            {   // Average Temperature
                sprintf(buffer, "Average Temperature : %.2f K", planets[selectedPlanet].surfaceTemp);
                std::string text = buffer;
                Vec2 size = UI::GetRenderedTextSize(buffer, font);
                UI::RenderText(app, text, font, white, topLeft, 0.0f);
                topLeft.y += size.y + gap;
            }

            {   // Axial Tilt
                sprintf(buffer, "Axial Tilt          : %.2f degrees", planets[selectedPlanet].axialTilt);
                std::string text = buffer;
                Vec2 size = UI::GetRenderedTextSize(text, font);
                UI::RenderText(app, text, font, white, topLeft, 0.0f);
                topLeft.y += size.y + gap;
            }
        }

        {   // Controls
            std::string controls = "[->]/[E] Next Planet\n[<-]/[Q] Prev Planet\n[ESC]    Exit";
            Vec2 size = UI::GetRenderedTextSize(controls, font);
            Vec2 topLeft { app->refScreenWidth - size.x - 10.0f, 10.0f };
            UI::RenderText(app, controls, font, white, topLeft, 0.0f);
        }

        {   // Planet Selection
            f32 height = 10.0f;
            f32 gap = 10.0f;

            s32 newSelection = selectedPlanet;
            for (int i = sizeof(planets) / sizeof(PlanetData) - 1; i >= 0; i--)
            {
                std::string text = ((i == selectedPlanet) ? std::string("> ") : std::string("  ")) + planets[i].name;
                Vec2 size = UI::GetRenderedTextSize(text, font);
                Vec2 padding { 10.0f, 10.0f };
                Vec2 topLeft;
                topLeft.x = 0.0f;
                topLeft.y = app->refScreenHeight - height - size.y - 2 * padding.y;

                if (UI::RenderTextButton(app, GenUIIDWithSec(i), text, font,
                                         padding, topLeft, 0.0f))
                {
                    if (i != selectedPlanet)
                        newSelection = i;
                }

                height += size.y + 2 * padding.y + gap;
            }

            std::string text = "Select Body:";
            Vec2 size = UI::GetRenderedTextSize(text, font);
            Vec2 topLeft;
            topLeft.x = gap;
            topLeft.y = app->refScreenHeight - height - size.y;
            UI::RenderText(app, text, font, white, topLeft, 0.0f);

            selectedPlanet = newSelection;
        }

#       ifdef DEBUG
        {   // Frame count
            static u64 frameCount = 0;
            static f64 prevTick = app->time;
            static f64 frameTime = app->deltaTime;

            static char buffer[64];

            if (showStats)
            {
                if (app->time - prevTick >= 1.0)
                {
                    frameTime = (app->time - prevTick) / (f64) frameCount;
                    frameCount = 0;
                    prevTick = app->time;
                }

                sprintf(buffer, "Frame Time: %f\nFPS: %.0f\nSelected Index: %d",
                        frameTime, 1.0 / frameTime, selectedPlanet);
            }
            else
                sprintf(buffer, "Press ` to show stats");

            std::string text = buffer;
            Vec2 size = UI::GetRenderedTextSize(text, font);
            Vec2 topLeft;
            topLeft.x = app->refScreenWidth - size.x - 10.0f;
            topLeft.y = app->refScreenHeight - size.y - 10.0f;
            UI::RenderText(app, buffer, font, { 0.75f, 0.75f, 0.75f, 0.75f },
                           topLeft, 0.0f);

            frameCount++;
        }
#       endif

        UI::End();
    };

    app.onWindowResize = [](Application* app)
    {
        p = Mat4::Perspective(
            45.0f,
            (f32) app->screenWidth / (f32) app->screenHeight,
            1.0f, 1000.0f
        );
    };

    app.Run();
}