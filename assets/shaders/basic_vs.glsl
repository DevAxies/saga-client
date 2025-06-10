 #version 300 es

//  Shader constants set before we render
uniform mat4 model;

uniform CameraBlock 
{
    mat4 view;
    mat4 perspective;
};

//  Attributes for our vertices
in vec3 vPos;
in vec3 vNorm;
in vec4 vCol;

//  Outputs for our Pixel Shader
out vec3 position;
out vec3 normal;
out vec4 color;

//  The main function
void main()
{
    //  Set our fragment params
    position = vec3(view * model * vec4(vPos, 1.0));
    normal = vec3(model * vec4(vNorm, 0.0));
    color = vCol;

    //  Store our final position
    gl_Position = perspective * view * model * vec4(vPos, 1.0);
}