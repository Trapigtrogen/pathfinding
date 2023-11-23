#include <drawpp.hpp>
#include "pathfinding.hpp"
#include <iostream>

// Global variables
DImage map;
DImage result;
Pathfinding::Pathfinder pathfinder;
std::vector<Pathfinding::Node*> pathdata;

int imageScale = 6;

Color pathColor = Color(0, 0, 0, 255);


// Setup is called once before the application loop starts
void setup() {
	// Map is original, for clearing path trace
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

	// TODO Start timer to see how long it takes to find complete route

}

// Draw is called once every frame
void draw(float) {
	// Clean canvas
	background(255, 255, 255);

	// Do one A* step. DoStep does nothing if path is complete
	Pathfinding::Node* currentNode = pathfinder.DoStep();

	// Clear previous frame path
	result = createImage(map.pixels(), map.width(), map.height());
	// Print path as different color
	std::vector<Pathfinding::Node*> path = pathfinder.RetracePath(currentNode);
	for(Pathfinding::Node* node : path) {
		result[node->pos.x + node->pos.y*map.width()] = pathColor;
	}
	// TODO DEBUG Tested nodes heatmap
	// for(Pathfinding::Node* node : pathdata) {
	// 	result[node->pos.x + node->pos.y*map.width()].blue -= node->tested*5;
	// 	result[node->pos.x + node->pos.y*map.width()].green -= node->tested*5;
	// }

	// apply new pixels to texture for GPU
	result.apply();

	// Render image
	image( result, 0, 0, result.width()*imageScale, result.height()*imageScale );

	// TODO Render current time
	// TODO Stop timer when done
}

// Register keybinds
void keyPressed() {
	switch (key) {
		case VK_ESC:
			exit();
		break;
		// TODO WASD movement, reset tracing and draw new endpoint
		// case VK_W:
		// 	setup();
		// break;
		// case VK_A:
		// 	setup();
		// break;
		// case VK_S:
		// 	setup();
		// break;
		// case VK_D:
		// 	setup();
		// break;
	}
}

int main() {
	// Create the application object
	Application app(1000, 1000, "Pathfinding");
	app.setKeyPressed(keyPressed);

	// Run the application
	return app.run(draw,setup);
}
