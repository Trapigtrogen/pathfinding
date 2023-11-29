#include "pathfinding.hpp"

namespace Pathfinding {
	Node* Pathfinder::DoStep() {
		if(openSet.size() == 0) {
			printf("Open set is empty\n");
			ResetPath();
			return startNode;
		}
		if(pathFound) {
			printf("The path is known\n");
			return endNode;
		}


		// Find lowest distance as current node
		std::vector<Node*>::iterator currentNodeIt = openSet.begin();
		for(Node* node : openSet) {
			// if inside has "|| node->hCost < (*currentNodeIt)->hCost" faster to find but not shortest path
			if(node->fCost() < (*currentNodeIt)->fCost()) {
				currentNodeIt = std::find(openSet.begin(), openSet.end(), node);
			}
		}
		Node* currentNode = (*currentNodeIt);

		// Set current node checked
		closedSet.push_back( (*currentNodeIt));
		currentNodeIt = openSet.erase(currentNodeIt);

		// Path found
		if( currentNode == endNode) {
			pathFound = true;
			return currentNode;
		}


		// Go thorough all neighbours and set proper ones open for checking
		std::vector<Node*> neighbours = GetNeighbourNodes(currentNode);
		for(Node* neighbour : neighbours ) {
			neighbour->tested += 1; // For debugging heatmap

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
			if(neighbour == endNode) {
				pathFound = true;
				return neighbour;
			}
		}

		return currentNode;
	}

	void Pathfinder::SetData(std::vector<Pathfinding::Node*> data) {
		for(Node* node : data) {
			// Floor
			if( node->pixel->red == 255 && node->pixel->green == 255 && node->pixel->blue == 255 ) {
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
				pathdata.push_back(node);
			}
			// Walls are omitted
		}
		// Missing start or end
		if(startNode == nullptr || endNode == nullptr) {
			printf("ERROR: No Start (Blue) or End (Red) marked\n");
			exit(-1);
		}

		startNode->gCost = 0;
		startNode->hCost = GetDistance(startNode, endNode);
		endNode->hCost = 0;
		endNode->gCost = GetDistance(endNode, startNode);
	}

	void Pathfinder::ResetPath() {
		closedSet.clear();
		openSet.clear();
		openSet.push_back(startNode);

		// Reset costs // TODO can be recalculated
		for(std::vector<Node*>::iterator it = pathdata.begin(); it != pathdata.end(); it++) {
			(*it)->gCost = 0;
			(*it)->hCost = 0;
		}
		startNode->hCost = GetDistance(startNode, endNode);
		endNode->gCost = GetDistance(endNode, startNode);

		pathFound = false;
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
							// Skip closed neighbours
							std::vector<Node*>::iterator closedIt = std::find(closedSet.begin(),
													  closedSet.end(),
													  node);
							if(closedIt != closedSet.end()) { continue; }

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

	Position Pathfinder::GetStartNodePosition() {
		return startNode->pos;
	}

	Position Pathfinder::GetEndNodePosition() {
		return endNode->pos;
	}

	Node* Pathfinder::MoveStart(uint x, uint y) {
		Position newPos(x, y);
		if(newPos.x > mapSize.width-1 || newPos.y > mapSize.height-1) {
			printf("Invalid position\n");
			return startNode;
		}

		std::vector<Node*>::iterator nodeIt = pathdata.begin();
		for(; nodeIt != pathdata.end(); nodeIt++) {
			if( (*nodeIt)->pos == newPos) {
				startNode = (*nodeIt);
			}
		}
		return startNode;
	}

	Node* Pathfinder::MoveEnd(uint x, uint y) {
		Position newPos(x, y);
		if(newPos.x > mapSize.width-1 || newPos.y > mapSize.height-1) {
			printf("Invalid position\n");
			return endNode;
		}

		std::vector<Node*>::iterator nodeIt = pathdata.begin();
		for(; nodeIt != pathdata.end(); nodeIt++) {
			if( (*nodeIt)->pos == newPos) {
				endNode = (*nodeIt);
			}
		}
		return endNode;
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

	Node* Pathfinder::GetNode(uint x, uint y) {
		for(Node* node : pathdata) {
			if( node->pos.x == x && node->pos.y == y ) {
				return node;
			}
		}
		return new Node( 0, 0, new Color(0,0,0) );
	}
}
