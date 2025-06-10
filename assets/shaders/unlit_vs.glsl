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
in vec2 vUv;
in vec4 vCol;

//  Outputs for our Pixel Shader
out vec2 uv;
out vec4 color;

//  The main function
void main()
{
    //  Set our fragment params
    uv = vUv;
    color = vCol;

    //  Store our final position
    gl_Position = perspective * view * model * vec4(vPos, 1.0);
}