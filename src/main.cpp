// Disclaimer: The simulation is not accurate. Pls don't come at me!

#include <cmath>
#include <string>
#include <glad/glad.h>
#include "platform/application.h"
#include "universal/types.h"
#include "universal/math.h"
#include "engine/ui.h"
#include "engine/mesh.h"
#include "engine/texture.h"
#include "simulation/planet.h"
#include "simulation/planet_data.h"

UI::Font font;
Vec4 white { 1.0f, 1.0f, 1.0f, 1.0f };
s32 selectedPlanet = 0;

Texture skyTexture;
Texture texs[sizeof(planets) / sizeof(PlanetData) + 1];
Mesh sphere;
Mat4 v, p;

Shader sunShader,
       planetShader,
       earthShader;

const Vec3 cameraOffset { 0.0f, 0.0f, 0.1f };
Vec3 cameraPos { 0.0f, 200.0f, 0.0f };

int main()
{
    Application app("Solar System", 1920, 1080, true, false);
    app.SetWindowIcon("res/icons/solar-system-icon.png");
    app.SetVsync(true);

    app.onInit = [](Application* app)
    {
        font.Load("res/fonts/Inconsolata.ttf", 25.0f);
        sphere.LoadFromOBJ("res/models/sphere.obj");

        skyTexture.Load("res/models/sky.jpg");

        texs[0].Load("res/models/sun.jpg");
        texs[1].Load("res/models/mercury.jpg");
        texs[2].Load("res/models/venus.jpg");
        texs[3].Load("res/models/earth_day.jpg");
        texs[4].Load("res/models/moon.jpg");
        texs[5].Load("res/models/mars.jpg");
        texs[6].Load("res/models/earth_night.jpg");

        sunShader.LoadShader("res/shaders/sun.vert", Shader::Type::VERTEX_SHADER);
        sunShader.LoadShader("res/shaders/sun.frag", Shader::Type::FRAGMENT_SHADER);
        sunShader.Compile();

        earthShader.LoadShader("res/shaders/earth.vert", Shader::Type::VERTEX_SHADER);
        earthShader.LoadShader("res/shaders/earth.frag", Shader::Type::FRAGMENT_SHADER);
        earthShader.Compile();

        planetShader.LoadShader("res/shaders/planet.vert", Shader::Type::VERTEX_SHADER);
        planetShader.LoadShader("res/shaders/planet.frag", Shader::Type::FRAGMENT_SHADER);
        planetShader.Compile();

        p = Mat4::Perspective(
            45.0f,
            (f32) app->screenWidth / (f32) app->screenHeight,
            1.0f, 1000.0f
        );
    };

    app.onUpdate = [](Application* app)
    {
        if (app->GetKeyDown(KEY(ESCAPE)))
        {
            app->Exit();
            return;
        }

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
        {   // Sky
            // The back face is being used to act like a cube map
            glCullFace(GL_FRONT);
            glDisable(GL_DEPTH_TEST);

            sunShader.Bind();

            Vec3 scale = Vec3 { 200.0f, 200.0f, 200.0f };
            Mat4 m = Mat4::Scaling(scale).Translate(cameraPos);
            sunShader.SetUniformMat4("u_mvp", false, p * v * m);
            
            skyTexture.Bind(0);
            sunShader.SetUniform1i("u_tex", 0);

            sphere.Draw();

            glEnable(GL_DEPTH_TEST);
            glCullFace(GL_BACK);
        }

        {   // All planets

            {   // Sun
                PlanetData& sun = planets[0];
                sunShader.Bind();

                Vec3 scale = Vec3 { 1.0f, 1.0f, 1.0f } * sun.radius;
                f32 rotationalSpeed = 1.0f / sun.rotationalPeriod;
                Mat4 m = Mat4::Scaling(scale).Rotate({ 0.0f, 1.0f, 0.0f }, app->time * rotationalSpeed).Rotate({ 0.0f, 0.0f, 1.0f }, ToRad(-sun.axialTilt));
                sunShader.SetUniformMat4("u_mvp", false, p * v * m);

                texs[0].Bind(0);
                sunShader.SetUniform1i("u_tex", 0);

                sphere.Draw();
            }

            {   // Earth
                PlanetData& earth = planets[3];
                earthShader.Bind();

                texs[3].Bind(0);
                earthShader.SetUniform1i("u_dayTex", 0);
                texs[6].Bind(1);
                earthShader.SetUniform1i("u_nightTex", 1);

                Vec3 scale = Vec3 { 1.0f, 1.0f, 1.0f } * earth.radius;
                f32 rotationalSpeed = 1.0f / earth.rotationalPeriod;
                Mat4 m = Mat4::Scaling(scale).Rotate({ 0.0f, 1.0f, 0.0f }, app->time * rotationalSpeed).Rotate({ 0.0f, 0.0f, 1.0f }, ToRad(-earth.axialTilt)).Translate(earth.position);
                earthShader.SetUniformMat4("u_model", false, m);
                earthShader.SetUniformMat4("u_vp", false, p * v);
    
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
                    planetShader.SetUniformMat4("u_vp", false, p * v);

                    texs[i].Bind(0);
                    planetShader.SetUniform1i("u_tex", 0);

                    planetShader.SetUniform3f("u_lightPos", planets[0].position.x, planets[0].position.y, planets[0].position.z);

                    sphere.Draw();
                }
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