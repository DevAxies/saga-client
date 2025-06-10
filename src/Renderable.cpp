#include "Renderable.h"

Renderable::Renderable() : position(vec3(0.0f, 0.0f, 0.0f)), rotation(vec3(0.0f, 0.0f, 0.0f)), scale(1.0f)
{

}

Renderable::~Renderable()
{

}

mat4_t Renderable::GetWorldTransform()
{
    mat4_t scaleMat = m4_scaling(vec3(scale, scale, scale));
    mat4_t rotationMat = m4_mul(m4_mul(m4_rotation_x(rotation.x), m4_rotation_y(rotation.y)), m4_rotation_z(rotation.z));
    mat4_t translationMat = m4_translation(position);
    return m4_mul(m4_mul(translationMat, rotationMat), scaleMat);
}