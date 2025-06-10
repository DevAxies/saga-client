//  This is a solver for A Star using a Hex Grid, it exposes
//  a function that will run the solver and store out the 
//  optimal path, if no path is found, than the final path
//  will remain empty. 
//
//  Also exposes a way to mark up the Hex Grid with debug rendering
//  to show the final path, the hexes we visited, and hexes that were
//  queued but never visited. 

#ifndef ASTARSOLVER_H
#define ASTARSOLVER_H

#include "math_3d.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <queue>
#include <functional>

class Renderable;

//  Internally used Hex Coord, we need this for using it as a key in a few maps
struct HexCoord
{
    int x = -1;
    int y = -1;

    HexCoord(int inX, int inY) : x(inX), y(inY) { }
    bool operator==(const HexCoord& right) const { return x == right.x && y == right.y; }
};

//  Define our custom hashing method for our HexCoord, this lets us use the HexCoord as a key 
//  in our map structures. 
namespace std
{
    template<> 
    struct hash<HexCoord> 
    {
        size_t operator()(const HexCoord& hexCoord) const 
        {
            size_t xHash = hash<int>()(hexCoord.x);
            size_t yHash = hash<int>()(hexCoord.y);
            return xHash ^ (yHash << 1);
        }
    };
}

//  This is an internal data structure for tracking information about the tiles we have visited
struct NodeData
{
    NodeData() { }

    HexCoord myCoord = {-1, -1};
    HexCoord myLinkCoord = { -1, -1 };
    int g = 0;
    int h = 0;

    int F() const { return g + h; };
};

//  This is a custom compare function that takes our NodeData and follows an approach similar to
//  std::greater with the intention of being used with a priority_queue to create a min heap
struct NodeCompare
{
    bool operator()(const std::weak_ptr<NodeData>& left, const std::weak_ptr<NodeData>& right) const
    {
        auto leftPtr = left.lock();
        if (!leftPtr) return false; 

        auto rightPtr = right.lock();
        if (!rightPtr) return true;

        if (leftPtr->F() > rightPtr->F())
        {
            return true;
        }
        else if (leftPtr->F() < rightPtr->F())
        {
            return false;
        }

        return leftPtr->h > rightPtr->h;
    }
};

//  This is our A Star Solver, it captures a single path trace
class AStarSolver 
{
    public:
        AStarSolver();
        virtual ~AStarSolver();

        //  Takes the map we want to walk, and the start and end locations, outputs its result to finalPath
        void SolvePath(std::shared_ptr<Renderable> inRenderable, int startX, int startY, int targetX, int targetY);

        //  Takes a map and colors it with debug information about our path tracing
        void RenderDebug(std::shared_ptr<Renderable> inRenderable);

        //  Resets the map colors
        void ClearDebug(std::shared_ptr<Renderable> inRenderable);

    private:
        //  These maps are used as part of the Path Solver, visited are tiles we checked, queued are ones we want to check
        std::unordered_map<HexCoord, std::shared_ptr<NodeData>> visited;
        std::unordered_map<HexCoord, std::shared_ptr<NodeData>> queued;

        //  This is a min heap using our custom NodeCompare method defined above
        std::priority_queue<std::weak_ptr<NodeData>, std::vector<std::weak_ptr<NodeData>>, NodeCompare> sortedNodes;
        
        //  This is the final path that the solver determined to be the best
        std::vector<HexCoord> finalPath;
};


#endif