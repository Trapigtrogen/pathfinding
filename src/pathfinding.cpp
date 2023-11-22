#include "pathfinding.hpp"

namespace Pathfinding {
	Node* Pathfinder::DoStep() {
		if(pathFound) { return endNode; }

		std::vector<Node*>::iterator currentNodeIt = openSet.begin();

		// Find lowest distance as current node
		std::vector<Node*>::iterator it = openSet.begin();
		for(; it != openSet.end(); it++) {
			if((*it)->fCost() <  (*currentNodeIt)->fCost() || (*it)->hCost < (*currentNodeIt)->hCost) {
				 (*currentNodeIt) = (*it);
			}
		}

		Node* currentNode = (*currentNodeIt);

		// Set current node checked
		closedSet.push_back( currentNode);
		openSet.erase(currentNodeIt);


		// Path found
		if( currentNode == endNode) {
			pathFound = true;
			return  currentNode;
		}

		// Check all neighbours and set proper ones open for checking
		for(Node* neighbour : GetNeighbourNodes(currentNode) ) {
			// Skip closed neighbours
			std::vector<Node*>::iterator closedIt = std::find(closedSet.begin(), closedSet.end(), neighbour);
			if(closedIt != closedSet.end()) { continue; }

			// If iterator = end neighbour is not in openSet
			std::vector<Node*>::iterator openIt = std::find(openSet.begin(), openSet.end(), neighbour);
			// Calculate how much it costs to get to the end for each neighbour
			int newGCost = currentNode->gCost + GetDistance( currentNode, neighbour );
			if(newGCost < neighbour->gCost || openIt == openSet.end()) {
				neighbour->gCost = newGCost;
				neighbour->hCost = GetDistance(neighbour, endNode);
				neighbour->parent = currentNode;

				// Set neigbour as unchecked
				if(openIt == openSet.end()) {
					openSet.push_back(neighbour);
				}
			}
		}

		return currentNode;
	}

	void Pathfinder::SetData(std::vector<Pathfinding::Node*> data) {
		for(Node* node : data) {
			// Floor
			if( node->pixel->red == 255 && node->pixel->green == 255 && node->pixel->blue == 255 ) {
				pathdata.push_back(node);
				pathdata.push_back(node);
			}
			// Start
			else if( node->pixel->red == 0 && node->pixel->green == 0 && node->pixel->blue == 255 ) {
				startNode = node;

				openSet.push_back(node);
			}
			// End
			else if( node->pixel->red == 255 && node->pixel->green == 0 && node->pixel->blue == 0 ) {
				endNode = node;
			}
		}
		// Missing start or end
		if(startNode == nullptr || endNode == nullptr) {
			printf("ERROR: No Start or End marked");
			exit(-1);
		}

		startNode->gCost = 0;
		startNode->hCost = GetDistance(startNode, endNode);
		endNode->hCost = 0;
		endNode->gCost = GetDistance(endNode, startNode);
	}

	std::vector<Node*> Pathfinder::GetNeighbourNodes(Node* node) {
		std::vector<Node*> neighbours;
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				if(x == 0 && y == 0) continue; // skip self

				Position checkPos = node->pos;
				checkPos.x += x;
				checkPos.y += y;

				if (checkPos.x <= mapSize.width-1 && checkPos.y <= mapSize.height-1) {
					for(Node* node : pathdata) {
						if( node->pos == checkPos ) {
							neighbours.push_back(node);
						}
					}
				}
			}
		}
		return neighbours;
	}

	void Pathfinder::SetMapSize(uint width, uint height) {
		mapSize.width = width;
		mapSize.height = height;
	}

	Size Pathfinder::GetMapSize() {
		return mapSize;
	}


	int Pathfinder::GetDistance(Node *nodeA, Node *nodeB) {
		int dstX = abs((int)nodeA->pos.x - (int)nodeB->pos.x);
		int dstY = abs((int)nodeA->pos.y - (int)nodeB->pos.y);

		if(dstX > dstY) {
			return 14 * dstY + 10 * (dstX - dstY);
		}
		return 14 * dstX + 10 * (dstY - dstX);
	}

	std::vector<Node*> Pathfinder::RetracePath(Node* currentNode) {
		std::vector<Node*> path;

		while(currentNode != startNode) {
			path.push_back(currentNode);
			currentNode = currentNode->parent;
		}
		path.push_back(startNode);
		return path;
	}

	Node* Pathfinder::GetStartNode() {
		return startNode;
	}

	Node* Pathfinder::GetEndNode() {
		return endNode;
	}
}
