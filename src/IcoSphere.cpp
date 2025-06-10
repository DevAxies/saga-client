#include "IcoSphere.h"
#include "math_3d.h"
#include "Mesh.h"
#include <vector>
#include <map>

unsigned short AddVertex(std::vector<SimpleVertex>& vertexData, vec3_t position)
{
    vec3_t normalizedPos = v3_norm(position);
    vertexData.push_back({ normalizedPos.x, normalizedPos.y, normalizedPos.z, normalizedPos.x, normalizedPos.y, normalizedPos.z, 1.0f, 1.0f, 1.0f, 1.0f });
    return (unsigned short)(vertexData.size() - 1);
}

unsigned short GetMiddlePoint(std::map<unsigned, unsigned short>& cache, std::vector<SimpleVertex>& vertexData, unsigned p1, unsigned p2)
{
    //  Create our key
    unsigned long long key = (p1 << 16) + p2;
    if (p2 < p1)
    {
        key = (p2 << 16) + p1;
    }

    //  Check if the key exists
    auto toFind = cache.find(key);
    if (toFind != cache.end())
    {
        return toFind->second;
    }

    //  Calculate new point
    vec3_t v1 = vec3(vertexData[p1].x, vertexData[p1].y, vertexData[p1].z);
    vec3_t v2 = vec3(vertexData[p2].x, vertexData[p2].y, vertexData[p2].z);
    vec3_t middle = vec3((v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f);

    //  Add our vertex and store the index in out map
    unsigned short i = AddVertex(vertexData, middle);
    cache[key] = i;
    return i; 
}

std::shared_ptr<Mesh> IcoSphere::Create(unsigned recursionLevel)
{
    //  Temporary data caches
    std::map<unsigned, unsigned short> middleCache;
    std::vector<SimpleVertex> vertexData;
    std::vector<unsigned short> indexData;

    //  Prelim. vertex data
    float t = (1.0f + sqrtf(5.0f)) / 2.0f;
    AddVertex(vertexData, vec3(-1.0f,  t, 0.0f));
    AddVertex(vertexData, vec3( 1.0f,  t, 0.0f));
    AddVertex(vertexData, vec3(-1.0f, -t, 0.0f));
    AddVertex(vertexData, vec3( 1.0f, -t, 0.0f));
    AddVertex(vertexData, vec3(0.0f, -1.0f,  t));
    AddVertex(vertexData, vec3(0.0f,  1.0f,  t));
    AddVertex(vertexData, vec3(0.0f, -1.0f, -t));
    AddVertex(vertexData, vec3(0.0f,  1.0f, -t));
    AddVertex(vertexData, vec3( t, 0.0f, -1.0f));
    AddVertex(vertexData, vec3( t, 0.0f,  1.0f));
    AddVertex(vertexData, vec3(-t, 0.0f, -1.0f));
    AddVertex(vertexData, vec3(-t, 0.0f,  1.0f));

    //  Prelim. Index Data
    indexData.push_back(0); indexData.push_back(11); indexData.push_back(5);
    indexData.push_back(0); indexData.push_back(5); indexData.push_back(1);
    indexData.push_back(0); indexData.push_back(1); indexData.push_back(7);
    indexData.push_back(0); indexData.push_back(7); indexData.push_back(10);
    indexData.push_back(0); indexData.push_back(10); indexData.push_back(11);
    indexData.push_back(1); indexData.push_back(5); indexData.push_back(9);
    indexData.push_back(5); indexData.push_back(11); indexData.push_back(4);
    indexData.push_back(11); indexData.push_back(10); indexData.push_back(2);
    indexData.push_back(10); indexData.push_back(7); indexData.push_back(6);
    indexData.push_back(7); indexData.push_back(1); indexData.push_back(8);
    indexData.push_back(3); indexData.push_back(9); indexData.push_back(4);
    indexData.push_back(3); indexData.push_back(4); indexData.push_back(2);
    indexData.push_back(3); indexData.push_back(2); indexData.push_back(6);
    indexData.push_back(3); indexData.push_back(6); indexData.push_back(8);
    indexData.push_back(3); indexData.push_back(8); indexData.push_back(9);
    indexData.push_back(4); indexData.push_back(9); indexData.push_back(5);
    indexData.push_back(2); indexData.push_back(4); indexData.push_back(11);
    indexData.push_back(6); indexData.push_back(2); indexData.push_back(10);
    indexData.push_back(8); indexData.push_back(6); indexData.push_back(7);
    indexData.push_back(9); indexData.push_back(8); indexData.push_back(1);

    //  Refine IcoSphere
    for (unsigned i = 0; i < recursionLevel; ++i)
    {
        std::vector<unsigned short> newIndexData;
        for (unsigned j = 0; j < indexData.size(); j += 3)
        {
            //  Create our mid-points
            short a = GetMiddlePoint(middleCache, vertexData, indexData[j], indexData[j+1]);
            short b = GetMiddlePoint(middleCache, vertexData, indexData[j+1], indexData[j+2]);
            short c = GetMiddlePoint(middleCache, vertexData, indexData[j+2], indexData[j]);

            //  Output 4 new triangles
            newIndexData.push_back(indexData[j]); newIndexData.push_back(a); newIndexData.push_back(c);
            newIndexData.push_back(indexData[j+1]); newIndexData.push_back(b); newIndexData.push_back(a);
            newIndexData.push_back(indexData[j+2]); newIndexData.push_back(c); newIndexData.push_back(b);
            newIndexData.push_back(a); newIndexData.push_back(b); newIndexData.push_back(c);
        }

        //  Replace our old index list
        indexData = std::move(newIndexData);
    }

    //  Create and return our new Mesh
    return Mesh::CreateMesh(VertexFormat::VERTEX_SIMPLE, (void*)vertexData.data(), vertexData.size(), (void*)indexData.data(), indexData.size());
}