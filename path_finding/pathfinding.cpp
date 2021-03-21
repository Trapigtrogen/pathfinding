#include <stdint.h>
#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <vector>
#include <list>


#ifdef WIN32
#include <glut/glut.h> // Windows
#else
#include </usr/include/GL/glut.h> // Linux
#endif

#include <memory.h>



namespace
{
	bool pathFound = false;

	struct Pixel {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	struct Position {
		int x;
		int y;
	};

	enum State {
		none,
		open,
		closed
	};

	class Node {
	public:
		bool wall = false;
		Position pos;
		State state = none;

		Node *parent;

		int gCost = 0;
		int hCost = 0;

		Node(int _posX, int _posY, Pixel _px) {
			if (_px.r == 0 && _px.g == 255 && _px.b == 0) { // Walls are green
				wall = true;
			}
			pos.x = _posX;
			pos.y = _posY;
		}

		int fCost() {
			return gCost + hCost;
		}
	};

	Pixel GetPixel(uint8_t x, uint8_t y, const uint8_t* data, int width) {
		Pixel px;
		px.r = data[3*x + y * 3 * width];
		px.g = data[3*x+1 + y * 3 * width];
		px.b = data[3*x+2 + y * 3 * width];
		return px;
	}

	void SetPixel(uint8_t x, uint8_t y, uint8_t* data, int width, Pixel px) {
		data[3*x + y * 3 * width] = px.r;
		data[3*x+1 + y * 3 * width] = px.g;
		data[3*x+2 + y * 3 * width] = px.b;
	}

	std::vector<Position> GetNeighbourPositions(Node *node, int width, int height) {
		std::vector<Position> neighbours;
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				if(x == 0 && y == 0) continue; // skip self

				int checkX = node->pos.x + x;
				int checkY = node->pos.y + y;

				if (checkX >= 0 && checkX <= width && checkY >= 0 && checkY <= height) {
					Position pos = {checkX, checkY};
					neighbours.push_back(pos);
				}
			}
		}
		return neighbours;
	}

	int GetDistance(Node *nodeA, Node *nodeB) {
		int dstX = abs(nodeA->pos.x - nodeB->pos.x);
		int dstY = abs(nodeA->pos.y - nodeB->pos.y);

		if(dstX > dstY) {
			return 14 * dstY + 10 * (dstX - dstY);
		}
		return 14 * dstX + 10 * (dstY - dstX);
	}

	void RetracePath(Node *startNode, Node *endNode, uint8_t* outputData, int width) {
		std::vector<Node*> path;
		Node *currentNode = endNode;

		Pixel px = {0,0,0};

		while(currentNode != startNode) {
			path.push_back(currentNode);
			SetPixel(currentNode->pos.x, currentNode->pos.y, outputData, width, px);
			currentNode = currentNode->parent;
		}
	}

	// STUDENT_TODO: Make implementation for doPathFinding function, which writes found path to outputData
	void doPathFinding(const uint8_t* inputData, int width, int height, uint8_t* outputData, int startX, int startY, int endX, int endY)
	{
		std::vector<Node> nodeList; // all nodes

		// Put all nodes in list (start and end first)
		nodeList.push_back( Node(startX, startY, GetPixel(startX, startY, inputData, width)) );
		nodeList.push_back( Node(endX, endY, GetPixel(endX, endY, inputData, width)) );
		nodeList[0].gCost = 0;
		nodeList[0].hCost = GetDistance(&nodeList[0], &nodeList[1]);
		nodeList[1].gCost = GetDistance(&nodeList[1], &nodeList[0]);
		nodeList[1].hCost = 0;

		for(int i = 0; i <= width; i++) {
			for(int j = 0; j <= height; j++) {
				if( (i != startX && j != startY) || (i != endX && j != endY) ) { // Skip start and end
					Node node( i, j, GetPixel(i, j, inputData, width) );
					//node.gCost = GetDistance(&node, &nodeList[0]);
					//node.hCost = GetDistance(&node, &nodeList[1]);
					nodeList.push_back(node);
				}
			}
		}

		nodeList[0].state = open;

		// Pathfinding loop
		while(!pathFound) {
			int currentNode = 0;
			for(int i = 0; i < nodeList.size(); ++i) {
				if(nodeList[i].state == open) { // Check the nodes in open list for cheapest walking cost

					//printf("current costs (gh): %d - %d - index costs (gh): %d - %d\n",  nodeList[currentNode].gCost, nodeList[currentNode].hCost, nodeList[i].gCost, nodeList[i].hCost);
					//printf("current fcost: %d - index fcost: %d\n",  nodeList[currentNode].fCost(), nodeList[i].fCost());
					//printf("current node: %d\n", currentNode);

					if(nodeList[i].fCost() < nodeList[currentNode].fCost() || nodeList[i].hCost < nodeList[currentNode].hCost) {
						currentNode = i;
					}
				}
			}			
			// set node to checked 
			nodeList[currentNode].state = closed;

			// Path found
			if(currentNode == 1) {
				RetracePath( &nodeList[0], &nodeList[1], outputData, width );
				pathFound = true;
				return;
			}

			// Check all neighbours and set proper ones open for checking
			for(Position neighbourPos : GetNeighbourPositions( &nodeList[currentNode], width, height) ) {
				for(int neighbour = 0; neighbour < nodeList.size(); neighbour++) {
					if(nodeList[neighbour].pos.x == neighbourPos.x && nodeList[neighbour].pos.y == neighbourPos.y) {

						// Skip iteration if neighbour is wall or in closedList
						if(nodeList[neighbour].wall || nodeList[neighbour].state == closed) continue;

						// Calculate how much it costs to get to the end for each neighbour
						int newGCost = nodeList[currentNode].gCost + GetDistance( &nodeList[currentNode], &nodeList[neighbour] );
						if(newGCost < nodeList[neighbour].gCost || nodeList[neighbour].state != open) {
							nodeList[neighbour].gCost = newGCost;
							nodeList[neighbour].hCost = GetDistance(&nodeList[neighbour], &nodeList[1]);
							nodeList[neighbour].parent = &nodeList[currentNode];

							if(nodeList[neighbour].state != open) {
								nodeList[neighbour].state = open;
							}
						}
					}
				}
			}
		}
	}

}


namespace
{
	// Quick and dirty function for reading bmp-files to opengl textures.
	GLuint loadBMPTexture(const char *fileName, int* w, int* h, uint8_t** data)
	{
		assert(w != 0);
		assert(h != 0);
		assert(data != 0);
		FILE *file;
		if ((file = fopen(fileName, "rb")) == NULL)
			return 0;
		fseek(file, 18, SEEK_SET);

		int width = 0;
		fread(&width, 2, 1, file);
		fseek(file, 2, SEEK_CUR);
		int height = 0;
		fread(&height, 2, 1, file);
		printf("Image \"%s\" (%dx%d)\n", fileName, width, height);

		*data = new uint8_t[3 * width * height];
		assert(data != 0);
		fseek(file, 30, SEEK_CUR);
		fread(*data, 3, width * height, file);
		fclose(file);

		GLuint  texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, *data);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (w) *w = width;
		if (h) *h = height;
		return texId;
	}

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Global variables

	// OpenGL texture ids for rendering.
	GLuint  inputTexture = 0;
	GLuint  outputTexture = 0;
	// Input and output data in pixels. outputData is updated to outputTexture each frame
	uint8_t* inputData = 0;
	uint8_t* outputData = 0;
	// width and height of the input and output datas
	int width = 0;
	int height = 0;
	// start and end position for path finding. These are found automatically from input file.
	int startX = -1;
	int startY = -1;
	int endX = -1;
	int endY = -1;

	// Initialization
	bool init()
	{
		glMatrixMode(GL_PROJECTION);
		glOrtho(0, 512 + 4, 256 + 2, 0, -1, 1);

		// Load input file
		inputTexture = loadBMPTexture("input.bmp", &width, &height, &inputData);
		if (0 == inputTexture)
		{
			printf("Error! Cannot open file: \"input.bmp\"\n");
			return false;
		}

		// Make outputTexture
		glGenTextures(1, &outputTexture);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Copy inputData also to outputData
		outputData = new uint8_t[3 * width*height];
		memcpy(outputData, inputData, 3 * width*height);

		// find start and end
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				uint8_t* pix = &inputData[3 * (y*width + x)]; // get pixel
				uint8_t r = pix[0];
				uint8_t g = pix[1];
				uint8_t b = pix[2];
				if (255 == r && g == 0 && b == 0) // Red?
				{
					// Start
					startX = x;
					startY = y;
					printf("Start position: <%d,%d>\n", x, y);
				}
				if (255 == b && r == 0 && g == 0) // Blue?
				{
					// End
					endX = x;
					endY = y;
					printf("End position: <%d,%d>\n", x, y);
				}
			}
		}

		if (startX < 0 || startY < 0)
		{
			printf("Error! Start position not found\n");
			return false;
		}

		if (endX < 0 || endY < 0)
		{
			printf("Error! End position not found\n");
			return false;
		}

		return true;
	}

	// Draw/Render
	void draw() 
	{
		doPathFinding(inputData, width, height, outputData, startX, startY, endX, endY);

		// Copy outputData to outputTexture
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, outputData);
		glBindTexture(GL_TEXTURE_2D, 0);

		glClear(GL_COLOR_BUFFER_BIT);

		// Draw input texture to left half of the screen
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, inputTexture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2d(1, 1);
		glTexCoord2d(0, 0); glVertex2d(1, 1 + 256);
		glTexCoord2d(1, 0); glVertex2d(1 + 256, 1 + 256);
		glTexCoord2d(1, 1); glVertex2d(1 + 256, 1);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Draw output texture to right half of the screen
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2d(2 + 256, 1);
		glTexCoord2d(0, 0); glVertex2d(2 + 256, 1 + 256);
		glTexCoord2d(1, 0); glVertex2d(2 + 512, 1 + 256);
		glTexCoord2d(1, 1); glVertex2d(2 + 512, 1);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glutSwapBuffers();
		glutPostRedisplay();
	}
} // end - anonymous namespace


// Main
int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(2*(512+4), 2*(256+2));
	glutCreateWindow("Pathfinding Demo");
	glutDisplayFunc(draw);
	if (!init()) return -1;
	glutMainLoop();
	delete inputData;
	delete outputData;
	return 0;
}