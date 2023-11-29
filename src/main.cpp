#include <drawpp.hpp>
#include "pathfinding.hpp"
#include <iostream>

// Global variables
DImage map;
DImage result;

Pathfinding::Pathfinder pathfinder;
std::vector<Pathfinding::Node*> pathdata;
Color pathColor = Color(0, 0, 0, 255);

int imageScale = 6;

float timer = 0;
float winTime = 0;

// Setup is called once before the application loop starts
void setup() {
	// Map is original image, for clearing drawn path every frame
	map = loadImage("/home/trapie/projects/pathfinding/build/bin/assets/input.bmp");
	result = createImage(map.pixels(), map.width(), map.height());

	// Create Node objects out of pixels
	for (int i = 0; i < result.width() * result.height(); i++) {
		uint x = i % result.width();
		uint y = i / result.width(); // Float to uint -> floored automatically
		pathdata.push_back( new Pathfinding::Node(x, y, &result[i]) );
	}
	// Send node data to pathfinder object
	pathfinder.SetData(pathdata);
	pathfinder.SetMapSize( map.width(), map.height() );

	// Resize window to fit two maps
	size(map.width()*imageScale, map.height()*imageScale);

	// Load font from assets folder and set to render black
    	textFont(loadFont("/home/trapie/projects/pathfinding/build/bin/assets/tuffy.ttf", 30) );
	fill(0,0,0);
}


// Draw is called once every frame
void draw(float deltaTime) {
	// Do one A* step. DoStep does nothing if path is complete
	// TODO If in the future there's more algorithms, they could be selected with SetAlgorithm() and this would do step for selected one
	Pathfinding::Node* currentNode = pathfinder.DoStep();
	if (!pathfinder.pathFound) {
		timer += deltaTime;
	}
	else {
		winTime = timer;
	}

	// Clear previous frame path
	result = createImage(map.pixels(), map.width(), map.height());

	// TODO DEBUG Tested nodes heatmap
	// for(Pathfinding::Node* node : pathdata) {
	// 	result[node->pos.x + node->pos.y * map.width()].blue -= node->tested * 10;
	// 	result[node->pos.x + node->pos.y * map.width()].green -= node->tested * 10;
	// }

	// TODO DEBUG Open and closed set colours (need to move sets to public in pathfinding.hpp)
	// for(Pathfinding::Node* node : pathfinder.openSet) {
	// 	result[node->pos.x + node->pos.y * map.width()] = Color(70, 70, 255);
	// }
	// for(Pathfinding::Node* node : pathfinder.closedSet) {
	// 	result[node->pos.x + node->pos.y * map.width()] = Color(100, 100, 200);
	// }

	// Print path
	std::vector<Pathfinding::Node*> path = pathfinder.RetracePath(currentNode);
	for(Pathfinding::Node* node : path) {
		result[node->pos.x + node->pos.y * map.width()] = pathColor;
	}

	// apply new pixels to texture for GPU
	result.apply();

	// Render image
	image( result, 0, 0, result.width()*imageScale, result.height()*imageScale );

	// Render timer
	text("Time: " + std::to_string(timer), map.width()*imageScale - 250, 30);

}


// Register keybinds
void keyPressed() {
	Pathfinding::Position origStart = pathfinder.GetStartNodePosition();
	Pathfinding::Position origEnd = pathfinder.GetEndNodePosition();
	Pathfinding::Node* newNode;
	timer = 0;

	switch (key) {
		case VK_ESC:
			exit();
		break;
		// WASD moves start node
		case VK_W:
			map[origStart.x + origStart.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveStart(origStart.x, origStart.y - 1);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(0, 0, 255);
		break;
		case VK_A:
			map[origStart.x + origStart.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveStart(origStart.x - 1, origStart.y);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(0, 0, 255);
		break;
		case VK_S:
			map[origStart.x + origStart.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveStart(origStart.x, origStart.y + 1);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(0, 0, 255);
		break;
		case VK_D:
			map[origStart.x + origStart.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveStart(origStart.x + 1, origStart.y);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(0, 0, 255);
		break;
		// IJKL moves end node
		case VK_I:
			map[origEnd.x + origEnd.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveEnd(origEnd.x, origEnd.y - 1);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(255, 0, 0);
		break;
		case VK_J:
			map[origEnd.x + origEnd.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveEnd(origEnd.x - 1, origEnd.y);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(255, 0, 0);
		break;
		case VK_K:
			map[origEnd.x + origEnd.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveEnd(origEnd.x, origEnd.y + 1);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(255, 0, 0);
		break;
		case VK_L:
			map[origEnd.x + origEnd.y * map.width()] = Color(255, 255, 255);
			newNode = pathfinder.MoveEnd(origEnd.x + 1, origEnd.y);
			pathfinder.ResetPath();
			map[newNode->pos.x + newNode->pos.y * map.width()] = Color(255, 0, 0);
		break;
	}
}


int main() {
	// Create the application object
	Application app(1000, 1000, "Pathfinding");
	app.setKeyPressed(keyPressed);

	// Run the application
	return app.run(draw,setup);
}
