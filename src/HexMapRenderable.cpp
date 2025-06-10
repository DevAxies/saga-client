#include "HexMapRenderable.h"
#include <stdlib.h>
#include "Mesh.h"
#include "UnlitShader.h"
#include "Texture2D.h"
#include <emscripten.h>

//  Triangle vertex data
static const UnlitVertex vertexData[4] = 
{
    { -0.5f,  0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f,  0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {  0.5f, -0.75f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { -0.5f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
};

static const short indexData[6] = {  0,1,2,
                                     3,2,0 };

HexMapRenderable::HexMapRenderable()
{
    printf("Creating HexMapRenderable.\n");

    //  Create the shaders used by this renderable
    unlitShader = std::make_shared<UnlitShader>();

    //  Create our mesh from the above vertex data
    basicMesh = Mesh::CreateMesh(VertexFormat::VERTEX_UNLIT, (void*)vertexData, 4, (void*)indexData, 6);
    
    //  Load all of the possible textures the tiles could be into our tiles list
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexBase00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertDunesPyramids00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowDirtDunes00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowBase00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowCactiForest00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowCrater00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowDirt01.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowHills00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowHillsOasis01.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowMesaLargeCave01.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowMesaLargeOasis00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowMesas00.png"));
    tiles.push_back(Texture2D::CreateTexture("assets/textures/tiles/hexDesertYellowSaltFlat00.png"));

    //  Allocate the map of Hexes, clear it to ResourceID 0
    hexes = std::unique_ptr<HexTile[]>(new HexTile[mapDimensions * mapDimensions]);
    ClearMap(0);
}

HexMapRenderable::~HexMapRenderable()
{

}

void HexMapRenderable::Update(float dt)
{
    //  Currently unused
}

void HexMapRenderable::Render(const vec3_t& eye, const mat4_t& view, const mat4_t& perspective)
{
    // Draw these tiles with transparency
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //  Precalculate some of our transforms
    mat4_t scaleMat = m4_scaling(vec3(scale, scale, scale));
    mat4_t rotationMat = m4_mul(m4_mul(m4_rotation_x(rotation.x), m4_rotation_y(rotation.y)), m4_rotation_z(rotation.z));

    //  Render our triangle
    unlitShader->BindProgram();
    unlitShader->SetCameraBlock(view, perspective);

    //  Loop over our textures
    float baseOffsetX = ((float)mapDimensions / 2.0f) - 0.5f;
    float baseOffsetY = (((float)mapDimensions * 0.75f) / 2.0f) - 0.125;
    for (unsigned y = 0; y < mapDimensions; ++y)
    {
        float xOffset = (y % 2 == 0) ? 0.0f : 0.5f;
        for (unsigned x = 0; x < mapDimensions; ++x)
        {
            //  Calculate Tile Position
            mat4_t translationMat = m4_translation(vec3(baseOffsetX - ((float)x + xOffset), baseOffsetY, 0.0f));
            mat4_t model = m4_mul(m4_mul(translationMat, rotationMat), scaleMat);
            unlitShader->SetModel((const GLfloat*) &model);

            //  Set the highlight
            vec3_t color = hexes[(y * mapDimensions) + x].color;
            if (x == highlightX && y == highlightY)
            {
                unlitShader->SetTint(vec3(color.x + 0.25f, color.y + 0.25f, color.z + 0.25f), 1.0f);
            }
            else
            {
                unlitShader->SetTint(color, 1.0f);
            }
            

            //  Bind the resource for this tile
            tiles[hexes[(y * mapDimensions) + x].resourceID]->BindTexture();

            //  Draw our mesh
            unlitShader->RenderMesh(basicMesh);
        }
        baseOffsetY -= 0.75f;
    }

    // reenable depth and kill blending
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void HexMapRenderable::SetHexColor(int x, int y, vec3_t color)
{
    if (x < 0 || x >= mapDimensions || y < 0 || y >= mapDimensions) return;
    hexes[(y * mapDimensions) + x].color = color;
}

//  TODO: 
//  Right now this is fixed costs based on the current test environment, we would want to load
//  some kind of resource file that had the costs of each tile and use that instead.
int HexMapRenderable::GetTileCost(int x, int y) const
{
    if (x < 0 || x >= mapDimensions || y < 0 || y >= mapDimensions) return -1;
    switch(hexes[(y * mapDimensions) + x].resourceID)
    {
        case 1:
        case 2:
            return 1;
        case 11:
            return 10;
        default:
            return -1;
    }
}

void HexMapRenderable::MouseMoved(float x, float y) 
{ 
    int hexX = -1, hexY = -1;
    GetClosestHexToPoint(x, y, &hexX, &hexY);

    if (hexX != highlightX || hexY != highlightY)
    {
        highlightX = hexX;
        highlightY = hexY;
    }
}

void HexMapRenderable::MouseClicked(float x, float y)
{
    int hexX = -1, hexY = -1;
    GetClosestHexToPoint(x, y, &hexX, &hexY);
    printf("Hex Tile Clicked On: {%d, %d}\n", hexX, hexY);

    EM_ASM_({ 
        Module.onMapClicked($0, $1);
    }, hexX, hexY);
}

void HexMapRenderable::ClearFocus()
{
    highlightX = highlightY = -1;
}

//  TODO:
//  Not super efficient, but it does the job for the current implmenetation, plenty of room
//  to optimize the ever living junk out of this.
void HexMapRenderable::GetClosestHexToPoint(float posX, float posY, int* outX, int* outY)
{
    if (outX == nullptr || outY == nullptr) return;

    float closestDist = 10000000.0f;
    vec3_t inPos = vec3(posX, posY, 0.0f);

    //  Loop over our textures
    float baseOffsetX = ((float)mapDimensions / 2.0f) - 0.5f;
    float baseOffsetY = (((float)mapDimensions * 0.75f) / 2.0f) - 0.375;
    for (unsigned y = 0; y < mapDimensions; ++y)
    {
        float xOffset = (y % 2 == 0) ? 0.0f : 0.5f;
        for (unsigned x = 0; x < mapDimensions; ++x)
        {
            //  Calculate Tile Position
            vec3_t tilePos = vec3(baseOffsetX - ((float)x + xOffset), baseOffsetY, 0.0f);

            //  distance from tile to mouse position
            vec3_t delta = v3_sub(inPos, tilePos);
            float distSquared = v3_dot(delta, delta);

            //  if it is less then our closest distance, set this guy
            if (distSquared < closestDist)
            {
                closestDist = distSquared;
                *outX = x;
                *outY = y;
            }
        }
        baseOffsetY -= 0.75f;
    }
}

vec3_t HexMapRenderable::GetWorldPositionForHex(int x, int y)
{
    float baseOffsetX = ((float)mapDimensions / 2.0f) - 0.5f;
    float baseOffsetY = (((float)mapDimensions * 0.75f) / 2.0f) - 0.125;
    float xOffset = (y % 2 == 0) ? 0.0f : 0.5f;
    
    vec3_t position = vec3(baseOffsetX - ((float)x + xOffset), baseOffsetY - (y * 0.75f), 0.0f);
    printf("Hex World Position: {%.2f, %.2f, %.2f}\n", position.x, position.y, position.z);
    return position;
}

bool HexMapRenderable::SetHexResourceID(int x, int y, int resourceID)
{
    if (x < 0 || x >= mapDimensions || y < 0 || y >= mapDimensions) return false;
    hexes[(y * mapDimensions) + x].resourceID = resourceID;
    return true;
}

bool HexMapRenderable::SetActiveHexResourceID(int resourceID)
{
    if (highlightX != -1 && highlightY != -1)
    {
        hexes[(highlightY * mapDimensions) + highlightX].resourceID = resourceID;
        return true;
    }

    return false;
}

bool HexMapRenderable::ClearMap(int resourceID)
{
    for (int i = 0; i < (mapDimensions * mapDimensions); ++i)
    {
        hexes[i].resourceID = resourceID;
        hexes[i].color = vec3(1, 1, 1);
    }
    return true;
}