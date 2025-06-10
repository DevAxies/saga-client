//  Class that handles the management and rendering of a Hex Map

#ifndef HEXMAPRENDERABLE_H
#define HEXMAPRENDERABLE_H

#include "Renderable.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <cmath>

class UnlitShader;
class Texture2D;
class Mesh;

struct HexTile
{
    unsigned resourceID;
    vec3_t color;
};

class HexMapRenderable : public Renderable
{
public:
    HexMapRenderable();
    virtual ~HexMapRenderable();

    //  ---- Renderable Implementation ----
    void Update(float dt) override;
    void Render(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective) override;

    void MouseMoved(float x, float y) override;
    void MouseClicked(float x, float y) override;
    void ClearFocus() override;
    //  ---- Renderable Implementation ----

    //  Set the specified hex to a specific resource ID
    bool SetHexResourceID(int x, int y, int resourceID);

    //  Set the currently moused over hex that is highlighted to a specific resource ID
    bool SetActiveHexResourceID(int resourceID);

    //  Clears the entire map to the passed in Resource ID
    bool ClearMap(int resourceID);

    //  Sets a hex to a specific tint color
    void SetHexColor(int x, int y, vec3_t color);

    //  Returns an int value for the cost of this specific tile, or -1 if the tile isn't traversable
    int GetTileCost(int x, int y) const;

    //  Returns the world coordinates for a hex given its hex coordinate
    vec3_t GetWorldPositionForHex(int x, int y);

    //  Get the Manhatten distance for our Hex Grid
    static int CalculateHexDistance(int x1, int y1, int x2, int y2)
    {
        //  We have a squiggly coordinate, this straightens it
        int sY1 = y1;
        int sX1 = (sY1 % 2) + (x1 * 2);
        int sY2 = y2;
        int sX2 = (sY2 % 2) + (x2 * 2);

        //  Do some early out / quick options
        if (sY1 == sY2)
        {
            return std::abs(x2 - x1);
        }
        else if (sX1 == sX2)
        {
            return std::abs(y2 - y1);
        }
        else
        {
            //  Otherwise, determine which edge case we are dealing with...
            if ((sX1 < sX2 && sY1 < sY2) || (sX1 > sX2 && sY1 > sY2))
            {
                return std::max(std::abs(y2 - y1), std::abs((sX2 - sY1)*-1 - (sX1 - sY2)*-1) / 2);
            }
            else
            {
                return std::max(std::abs(y2 - y1), std::abs((sX2 - sY2)*-1 - (sX1 - sY1)*-1) / 2);
            }
        }
    }

private:
    std::shared_ptr<Mesh> basicMesh;
    std::shared_ptr<UnlitShader> unlitShader;
    std::vector<std::shared_ptr<Texture2D>> tiles;

    unsigned mapDimensions = 25;
    std::unique_ptr<HexTile[]> hexes;

    int highlightX = -1, highlightY = -1;

    //  Gets the closest hex based on the passed in location
    void GetClosestHexToPoint(float x, float y, int* outX, int* outY);
};

#endif