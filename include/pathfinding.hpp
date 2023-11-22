#pragma once
#include <drawpp.hpp>

namespace Pathfinding {
	struct Position {
		uint x;
		uint y;

		bool operator ==(const Position& pos) {
			if(x == pos.x && y == pos.y) {
				return true;
			}
			return false;
		};

		bool operator !=(const Position& pos) {
			if(x != pos.x || y != pos.y) {
				return true;
			}
			return false;
		};
	};
	struct Size {
		uint width;
		uint height;
	};

	class Node {
	public:
		Position pos;
		Color* pixel;

		Node *parent = nullptr;

		int gCost = 0;
		int hCost = 0;

		Node(uint _posX, uint _posY, Color* _px) {
			pos.x = _posX;
			pos.y = _posY;
			pixel = _px;
		}

		int fCost() {
			return this->gCost + this->hCost;
		}
	};


	class Pathfinder {
	public:
		Pathfinder() = default;

		void SetData(std::vector<Pathfinding::Node*> data);

		Node* DoStep();

		void SetMapSize(uint width, uint height);
		Size GetMapSize();

		std::vector<Node*> RetracePath(Node* endNode);

		bool pathFound = false;

	private:
		Size mapSize;

		Node* startNode;
		Node* endNode;
		// std::vector<Node*> wallNodes;

		std::vector<Pathfinding::Node*> closedSet;
		std::vector<Pathfinding::Node*> openSet;
		std::vector<Pathfinding::Node*> pathdata;

		std::vector<Node*> GetNeighbourNodes(Node* node);

		int GetDistance(Node *nodeA, Node *nodeB);

		Node* GetStartNode();
		Node* GetEndNode();
	};
}
