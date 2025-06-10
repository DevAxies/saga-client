 #version 300 es

//  Set our precision
precision mediump float;

//  Uniforms set by the program
uniform vec3 eye;

//  Inputs from our vertex shader
in vec3 position;
in vec3 normal;
in vec4 color;

//  Output Color
out vec4 fragmentColor;

//  The main pixel shader function
void main()
{
    //  Light Colors
    vec4 diffuse = vec4(0.75, 0.75, 0.75, 1.0);
    vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

    //  Light direction
    vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));
    float shininess = 64.0;

    //  Set the specular term to black
    vec4 spec = vec4(0.0, 0.0, 0.0, 0.0);
 
    //  Normalize both input vectors
    vec3 n = normalize(normal);
    vec3 e = normalize(eye - position);
 
    //  Calculate light intensity
    float intensity = max(dot(n, lightDir), 0.0);
 
    //  If the vertex is lit compute the specular color
    if (intensity > 0.0) 
    {
        //  Compute the half vector
        vec3 h = normalize(lightDir + e);  
        
        //  Compute the specular term into spec
        float intSpec = max(dot(h,n), 0.0);
        spec = specular * pow(intSpec,shininess);
    }

    fragmentColor = color * max(intensity * diffuse + spec, ambient);                  
}