#include <GL/glut.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <cmath>
using namespace std;

#define checkImageWidth 64
#define checkImageHeight 64
static unsigned int texture[2]; // Array of texture indices.
static int id = 0; // Currently displayed texture id.
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;

// Struct of bitmap file.
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;
  
   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);
 
   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4); 
   
   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);
   
   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3)
   { 
      temp = bmp->data[i];
	  bmp->data[i] = bmp->data[i+2];
	  bmp->data[i+2] = temp;
   }

   return bmp;
}

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image;
   
   // Load the texture.
   image = getBMPData("launch.bmp"); 

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
}

// Routine to load a program-generated image as a texture. 
void loadProceduralTextures()			
{
   // Activate texture index texture[1]. 
   glBindTexture(GL_TEXTURE_1D, texture[1]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}


/* Create the Checker Image */
void makeCheckImage(void) {
	for (int i = 0; i < checkImageHeight; i += 8) {
		for (int j = 0; j < checkImageWidth; j += 8) {
			int c = (((( i & 8 ) == 0) ^ (( j & 8 ) == 0))) * 255;
			GLubyte r = rand() % 256;
			GLubyte g = rand() % 256;
			GLubyte b = rand() % 256;
			for (int k = i; k < i + 8; k++) {
				for (int l = j; l < j + 8; l++) {
					if (c == 255) {
						checkImage[k][l][0] = r;
						checkImage[k][l][1] = g;
						checkImage[k][l][2] = b;
					} else {
						checkImage[k][l][0] = (GLubyte) c;
						checkImage[k][l][1] = (GLubyte) c;
						checkImage[k][l][2] = (GLubyte) c;
					}
					checkImage[k][l][3] = (GLubyte) 100;
				}
			}
			
		}
	}
}

void init(void) {
	glClearColor(0.8, 0.8, 0.8, 0.0); 

   // Create texture index array.
   glGenTextures(2, texture); 

   // Load external texture and generate and load procedural texture.
   loadExternalTextures();
   makeCheckImage();
   loadProceduralTextures();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D);

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  	glBindTexture(GL_TEXTURE_2D, texture[id]);  // Activate a texture.
	
	glBegin(GL_QUADS);
		// sets the current texture coordinates
		// any subsequent vertex command has those texture coordinates associated with it
		// until glTexCoord2f is called again
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, -4);
		glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, -4);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, -4);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, -4);
	
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -4);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, -4);
		glTexCoord2f(1.0, 1.0); glVertex3f(2, 1.0, -5);
		glTexCoord2f(1.0, 0.0); glVertex3f(2, -1.0, -5);
	glEnd();
	
	glutSwapBuffers();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case ' ':
         id++;
         id %= 2;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

void printInteraction(void)
{
   cout << "Interaction:" << endl
	    << "Press space to toggle between textures." << endl;
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);  // Sets the shading model
}

int main(int argc, char** argv) {
	printInteraction();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyInput);
	glutMainLoop();
	return 0; 
}


