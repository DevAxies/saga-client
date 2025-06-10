#include "Vertex.h"

unsigned GetFormatSize(VertexFormat format)
{
    switch (format)
    {
        case VertexFormat::VERTEX_SIMPLE:
            return sizeof(SimpleVertex);
        case VertexFormat::VERTEX_UNLIT:
            return sizeof(UnlitVertex);
        default:
            return 0;
    }
}