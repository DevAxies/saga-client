//  Provides a bunch of functions for creating an IcoSphere

#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include <memory>

class Mesh;

class IcoSphere
{
public:
    ~IcoSphere() { }

    static std::shared_ptr<Mesh> Create(unsigned recursionLevel);

private:

    IcoSphere() { }
};

#endif