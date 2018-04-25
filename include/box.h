#ifndef H_BOX
#define H_BOX
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include "SOIL/SOIL.h"
#include <iostream>
using std::cerr;
using std::endl;
using std::string;
using std::vector;
class Box{
    GLuint VBOvertices,VBOindices;

    void Init(const float size){
        GLfloat box_vertices[] = {
            size,  size,  size,  
            size, -size,  size, 
            size,  size, -size,
            size, -size, -size,
           -size, -size, -size,
           -size,  size, -size, 
           -size, -size,  size,  
           -size,  size,  size   
        };

        GLuint box_indices[] = {  
            0, 1, 3,  
            3, 2, 0,
            0, 1, 7,
            7, 6, 1,
            1, 3, 6,
            6, 4, 3,
            3, 2, 4,
            4, 2, 5,
            5, 4, 6,
            6, 5, 7,
            7, 5, 2,
            2, 0, 7
        };  

	    glGenVertexArrays(1, &vao);
	    glGenBuffers(1, &VBOvertices);
	    glGenBuffers(1, &VBOindices);
    
	    glBindVertexArray(vao); GL_CHECK_ERRORS;
		
		//передаем в шейдерную программу атрибут координат вершин
	    glBindBuffer(GL_ARRAY_BUFFER, VBOvertices); GL_CHECK_ERRORS;
	    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices) ,box_vertices, GL_STATIC_DRAW); GL_CHECK_ERRORS;
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); GL_CHECK_ERRORS;
	    glEnableVertexAttribArray(0); GL_CHECK_ERRORS;

		//передаем в шейдерную программу индексы
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VBOindices); GL_CHECK_ERRORS;
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_indices), box_indices, GL_STATIC_DRAW); GL_CHECK_ERRORS;

	    glBindBuffer(GL_ARRAY_BUFFER, 0);

	    glBindVertexArray(0);
    }

    public:
        GLuint vao;

        Box(const float size = 1){
            Init(size);
        };

        void Draw(const ShaderProgram& shader, const float4x4& projection,const float4x4& view, const float3& campos){
            shader.StartUseShader();
            shader.SetUniform("projection", projection);
            shader.SetUniform("view", view);
            shader.SetUniform("campos", campos);

            glBindVertexArray(vao);
           
            glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, nullptr); GL_CHECK_ERRORS;
            glBindVertexArray(0); GL_CHECK_ERRORS;
            shader.StopUseShader();
        }


};



#endif