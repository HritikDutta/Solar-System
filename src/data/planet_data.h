#pragma once

#include "simulation/planet.h"

PlanetData planets[] = {
    PlanetData {
        "Sun", "The start at the center of the solar system.",
        { 1.0f, 0.9f, 0.5f, 1.0f },
        {}, 30.0f, 7.25f, 5505.0f,
        25.0f, 230e6f, 0.0f,
        0.0f, 0.0f,
        9, { 1, 2, 3, 5, 6, 7, 8, 9, 10 },
        true, { 0.0f, 30.0f, 150.0f }
    },

    PlanetData {
        "Mercury", "The planet closest to the sun.",
        { 0.6f, 0.6f, 0.6f, 1.0f },
        {}, 1.0f, 2.0f, 452.039f,
        58.646f, 0.2408467f, 57909227.0f,
        75.0f, 0.6f,
        0, {},
        false, { 0.0f, 0.2f, 0.1f }
    },

    PlanetData {
        "Venus", "The hottest planet in the solar system.",
        { 0.75f, 0.8f, 0.5f, 1.0f },
        {}, 3.0f, 3.0f, 737.0f,
        -243.018f, 0.61519726f, 108209475.0f,
        125.0f, 0.4f,
        0, {},
        false, { 0.0f, 0.5f, 1.5f }
    },

    PlanetData {
        "Earth", "The only known planet with life on it.",
        { 0.5f, 0.8f, 1.0f, 1.0f },
        {}, 3.1f, 23.5f, 288.0f,
        0.99726968f, 1.0000174f, 149598262.0f,
        200.0f, 0.2f,
        1, { 4 },
        true, { 0.0f, 0.5f, 1.5f }
    },

    PlanetData {
        "Moon", "The only natural satellite of planet Earth.",
        { 0.8f, 0.8f, 0.8f, 1.0f },
        {}, 0.5f, 1.5f, 10.0f,
        27.0f, 29.0f, 385000.0f,
        10.5f, 2.2f,
        0, {},
        false, { 0.0f, 0.1f, 5.5f }
    },
    
    PlanetData {
        "Mars", "The outermost of the inner planets.",
        { 0.8f, 0.3f, 0.3f, 1.0f },
        {}, 2.1f, 25.19f, 293.15f,
        1.026f, 1.8808476f, 227943824.0f,
        350.0f, 0.1f,
        0, {},
        false, { 0.0f, 2.0f, 1.5f }
    },
    
    PlanetData {
        "Jupiter", "The largest planet in the solar system.",
        {  0.85f, 0.6f, 0.5f, 1.0f },
        {}, 9.5f, 3.0f, -110.0f,
        0.4125f, 11.87123f, 778.6e6f,
        650.0f, 0.05f,
        0, {},
        false, { 0.0f, 2.0f, 55.0f }
    },

    PlanetData {
        "Saturn", "The planet with the largest rings.",
        {  0.75f, 0.75f, 0.6f, 1.0f },
        {}, 7.0f, 26.73f, -140.0f,
        0.4458f, 29.4767f, 1433.5e6f,
        1000.0f, 0.03f,
        0, {},
        false, { 0.0f, 2.0f, 50.0f }
    },

    PlanetData {
        "Uranus", "Also known as the sideways planet.",
        {  0.6f, 0.8f, 0.85f, 1.0f },
        {}, 4.0f, 97.77f, -195.0f,
        0.7167f, 84.0205f, 2872.5e6f,
        1500.0f, 0.03f,
        0, {},
        false, { 0.0f, 2.0f, 45.0f }
    },

    PlanetData {
        "Neptune", "The outermost planet of the solar system.",
        {  0.4f, 0.5f, 0.9f, 1.0f },
        {}, 4.8f, 28.32f, -200.0f,
        0.6713f, 164.8f, 4495.1e6f,
        2200.0f, 0.02f,
        0, {},
        false, { 0.0f, 2.0f, 40.0f }
    },

    PlanetData {
        "Pluto", "The largest dwarf planet in our solar system.",
        {  0.9f, 0.8f, 0.7f, 1.0f },
        {}, 0.65f, 120.0f, -225.0f,
        6.3875f, 247.94f, 5906.4e6f,
        2750.0f, 0.01f,
        0, {},
        false, { 0.0f, 0.02f, 0.01f }
    },
};