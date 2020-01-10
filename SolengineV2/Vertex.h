#pragma once
#include <GL/glew.h>

namespace SolengineV2 
{
    struct Position
    {
        float x, y;
    };

    struct UVCoords
    {
        float u, v;
    };

    struct Colour
    {
        Colour() : r(0), g(0), b(0), a(0) { }
        Colour(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) { }
        GLubyte r, g, b, a;
    };

    struct Vertex 
    {
        //This is the position struct. When you store a struct or class
        //inside of another struct or class, it is called composition. This is
        //layed out exactly the same in memory as if we had a float position[2],
        //but doing it this way makes more sense.
        Position Pos;

        //4 bytes for r g b a color.
        Colour Col;

        //UV texture coordinates.
        UVCoords UV;
        
        void SetPosition(float x, float y) 
        {
            Pos.x = x;
            Pos.y = y;
        }

        void SetColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a) 
        {
            Col.r = r;
            Col.g = g;
            Col.b = b;
            Col.a = a;
        }

        void SetColour(Colour col)
        {
            Col = col;
        }

        void SetUV(float u, float v)
        {
            UV.u = u;
            UV.v = v;
        }
    };
}