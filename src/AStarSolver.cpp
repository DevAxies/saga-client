//  A Star Solve

#include "AStarSolver.h"
#include "HexMapRenderable.h"
#include <cmath>

AStarSolver::AStarSolver()
{

}

AStarSolver::~AStarSolver()
{

}

//  Given how our hex coordinate system works, we have slightly different neighbor calculations
//  based on if our Y value is even or odd, we capture the neighbors here and then select the 
//  correct array inside SolvePath to simplify the logic of walking our neighbors.
HexCoord OddHexGrid[6] = { 
   { -1, 0 },
   { 0, -1 },
   { 1, -1 },
   { 1, 0  },
   { 1, 1  },
   { 0, 1  }  
 };

 HexCoord EvenHexGrid[6] = { 
   { -1, 0 },
   { -1,-1 },
   { 0, -1 },
   { 1, 0  },
   { 0, 1  },
   { -1, 1 }  
 };

void AStarSolver::SolvePath(std::shared_ptr<Renderable> inRenderable, int startX, int startY, int targetX, int targetY)
{
    //  Cast to hex renderable... if not, bail
    HexMapRenderable* inGrid = dynamic_cast<HexMapRenderable*>(inRenderable.get());
    if (inGrid == nullptr)
    {
        printf("Invalid Renderable passed to A Star Solve.\n");
        return;
    } 

    //  Create our internal Hex Coords
    HexCoord start = { startX, startY };
    HexCoord target = { targetX, targetY };

    //  Allocate a new node for the Start
    auto firstNode = std::make_shared<NodeData>();
    firstNode->g = 0;
    firstNode->h = HexMapRenderable::CalculateHexDistance(startX, startY, targetX, targetY);
    firstNode->myCoord = start;

    //  Queue our first node
    queued[start] = firstNode;
    sortedNodes.push(firstNode);

    //  Sometimes we'll need to unfold our queue and rebuild it, this is a temp store
    //  thats up here so we avoid additional allocations in the loop
    std::vector<std::weak_ptr<NodeData>> TempNodeStore;

    //  Loop till we have visited our target node or we are out of nodes
    std::shared_ptr<NodeData> targetNode = nullptr;
    while (queued.size() > 0 && !targetNode)
    {
        //  Get the node with the next lowest F value
        auto weakNode = sortedNodes.top();
        sortedNodes.pop();
        auto strongNode = weakNode.lock();
        if (!strongNode) continue;

        //  Add this node to visited
        visited[strongNode->myCoord] = strongNode;
        queued.erase(strongNode->myCoord);

        //  If this is the target... bust out
        if (strongNode->myCoord == target)
        {
            targetNode = strongNode;
            continue;
        }

        //  Walk our neighbors
        HexCoord* GridToUse = (strongNode->myCoord.y % 2) ? OddHexGrid : EvenHexGrid;
        for (int i = 0; i < 6; ++i)
        {
            HexCoord neighbor = { strongNode->myCoord.x + GridToUse[i].x, strongNode->myCoord.y + GridToUse[i].y };

            //  Check if our neighbor has been visited
            auto findInVisited = visited.find(neighbor);
            if (findInVisited != visited.end())
            {
                //  It has already been visited... skip to next neigbor
                continue;
            }

            //  Get the tile cost of this tile, if its -1, its impassable
            int tileCost = inGrid->GetTileCost(neighbor.x, neighbor.y);
            if (tileCost == -1) continue;

            //  Calculcate the new G cost
            int gCost = strongNode->g + tileCost;

            //  Check if this node is already queued
            auto findInQueued = queued.find(neighbor);
            if (findInQueued != queued.end())
            {
                //  Check if we have a lower g cost to provide it
                if (findInQueued->second->g > gCost)
                {
                    //  Update the node with the new values
                    findInQueued->second->g = gCost;
                    findInQueued->second->myLinkCoord = strongNode->myCoord;

                    //  Priority queues do not allow for in place updates... but this is infrequent so we'll pop till
                    //  we find our matching weak_ptr
                    while (sortedNodes.size() > 0)
                    {
                        auto weakTestNode = sortedNodes.top();
                        sortedNodes.pop();
                        TempNodeStore.push_back(weakTestNode);
                        if (weakTestNode.lock() == findInQueued->second) break;
                    }

                    //  ...and now we reinsert all the nodes
                    while (TempNodeStore.size() > 0)
                    {
                        sortedNodes.push(TempNodeStore.back());
                        TempNodeStore.pop_back();
                    }
                }
            }
            else
            {
                //  Not already queued, so lets create it
                auto newNode = std::make_shared<NodeData>();
                newNode->g = gCost;
                newNode->h = HexMapRenderable::CalculateHexDistance(neighbor.x, neighbor.y, targetX, targetY);
                newNode->myCoord = neighbor;
                newNode->myLinkCoord = strongNode->myCoord;

                //  Queue our new node
                queued[neighbor] = newNode;
                sortedNodes.push(newNode);
            }
        }
    }

    //  If our target node is valid...
    if (targetNode)
    {
        //  Rebuild the path
        auto currentNode = targetNode;
        while (!(currentNode->myCoord == start))
        {
            finalPath.push_back(currentNode->myCoord);
            currentNode = visited[currentNode->myLinkCoord];
        }
        finalPath.push_back(start);

        //  Print the path out to the console
        printf("Found Path: ");
        for (int i = finalPath.size() - 1; i >= 0; --i)
        {
            printf("{%d, %d}", finalPath[i].x, finalPath[i].y);
            if (i > 0) 
            {
                printf(" -> ");
            }
        }
        printf("\n");
    }
    else
    {
        printf("Target Node Not Found.\n");
    }
}

void AStarSolver::RenderDebug(std::shared_ptr<Renderable> inRenderable)
{
    //  Cast to hex renderable... if not, bail
    HexMapRenderable* inGrid = dynamic_cast<HexMapRenderable*>(inRenderable.get());
    if (inGrid == nullptr)
    {
        printf("Invalid Renderable passed to A Star Solve.\n");
        return;
    } 

    //  Loop over all visited to determine min and max F
    int minF = std::numeric_limits<int>::max();
    int maxF = std::numeric_limits<int>::min();
    for(const auto& node : visited)
    {
        minF = std::min(minF, node.second->F());
        maxF = std::max(maxF, node.second->F());
    }

    //  Create a lambda for doing our lerp below
    auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };

    //  Loop over all visited and color based on how close they were to min F
    vec3_t closeColor = vec3(0, 0, 1);
    vec3_t farColor = vec3(1, 0, 0);
    for(const auto& node : visited)
    {
        int fValue = node.second->F();
        float interpVal = (float)(fValue - minF) / (float)(maxF - minF);
        vec3_t interpColor = vec3(lerp(closeColor.x, farColor.x, interpVal),
                                  lerp(closeColor.y, farColor.y, interpVal),
                                  lerp(closeColor.z, farColor.z, interpVal));
        
        inGrid->SetHexColor(node.first.x, node.first.y, interpColor);
    }

    //  Loop over the queued elements we never visited
    vec3_t queuedColor = vec3(0.8, 0.8, 0.8);
    for(const auto& node : queued)
    {
        inGrid->SetHexColor(node.first.x, node.first.y, queuedColor);
    }

    //  Color our final path
    vec3_t pathColor = vec3(0, 1, 0);
    for(const auto& node : finalPath)
    {
        inGrid->SetHexColor(node.x, node.y, pathColor);
    }
}

void AStarSolver::ClearDebug(std::shared_ptr<Renderable> inRenderable)
{
    //  Cast to hex renderable... if not, bail
    HexMapRenderable* inGrid = dynamic_cast<HexMapRenderable*>(inRenderable.get());
    if (inGrid == nullptr)
    {
        printf("Invalid Renderable passed to A Star Solve.\n");
        return;
    } 

    //  Reset color on our visited nodes
    vec3_t resetColor = vec3(1, 1, 1);
    for(const auto& node : visited)
    {
        inGrid->SetHexColor(node.first.x, node.first.y, resetColor);
    }

    //  Loop over the queued elements we never visited
    for(const auto& node : queued)
    {
        inGrid->SetHexColor(node.first.x, node.first.y, resetColor);
    }
}