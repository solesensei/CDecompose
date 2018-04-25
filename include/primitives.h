#ifndef H_PRIMITIVES
#define H_PRIMITIVES

#include "common.h"
#include "ShaderProgram.h"

using std::cerr;
using std::endl;
using std::string;
using std::vector;


class Box{
    GLuint VBOvertices, VBOindices;

    void Init(const float size);
    
public:
    GLuint vao;

    Box(const float size = 1){
        Init(size);
    };

    void Draw(
        const ShaderProgram& shader, const float4x4& projection,
        const float4x4& view, const float4& shift = float4()
    );

};

class Plane{
    GLuint VBOvertices,VBOindices;

    void Init(const float size);
    
public:
    GLuint vao;

    Plane(const float size = 1){
        Init(size);
    };

    void Draw(
        const ShaderProgram& shader, const float4x4& projection, 
        const float4x4& view, const float4& shift = float4()
    );

};

#endif