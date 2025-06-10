 #version 300 es

//  Set our precision
precision mediump float;

//  Uniforms set by the program
uniform vec4 tint;

//  Inputs from our vertex shader
in vec2 uv;
in vec4 color;

//  The input texture
uniform sampler2D tex;

//  Output Color
out vec4 fragmentColor;

//  The main pixel shader function
void main()
{
    fragmentColor = color * tint * texture(tex, uv);                  
}