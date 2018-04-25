#version 330 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;


void main()
{
  vec3 lightDir = vec3(1.0f, 1.0f, 0.0f); 

  vec3 col = vec3(0.0f, 0.9f, 0.75f);

  float kd = max(dot(vNormal, lightDir), 0.0);

  color = vec4(kd * col, 1.0f);
}