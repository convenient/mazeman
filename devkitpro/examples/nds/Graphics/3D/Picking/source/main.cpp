/*---------------------------------------------------------------------------------
Demonstrates how to use 3D picking on the DS

Author: Gabe Ghearing
Created: Feb 2007

This file is released into the public domain

Basic idea behind picking;
    Draw the scene a second time with a projection matrix that only renders what is
directly below the cursor. The GPU keeps track of how many polygons are drawn, so
if a polygon is drawn in this limited view the polygon is directly below the cursor.
Several polygons may be drawn under the cursor, so a position test is used for each
object(a collection of polygons) to tell which object is closest to the camera.
The object that is closest to the camera and under the cursor is the one that the
user is clicking on.

There are several optimizations that are not done in this example, such as:
   - Simplify models during the picking pass, the model needs to occupy the
       same area, but can usually use fewer polygons.
   - Save the projection matrix with glGetFixed() instead of recreating it
       every pass.

*--------------------------------------------------------------------------------*/

#include <nds.h>
#include <nds/arm9/postest.h>
#include "cone_bin.h"
#include "cylinder_bin.h"
#include "sphere_bin.h"

typedef enum {
	NOTHING,
	CONE,
	CYLINDER,
	SPHERE
} Clickable;

Clickable clicked; // what is being clicked
int closeW; // closest distace to camera
int polyCount; // keeps track of the number of polygons drawn

// run before starting to draw an object while picking
void startCheck() {
	while(PosTestBusy()); // wait for the position test to finish
	while(GFX_BUSY); // wait for all the polygons from the last object to be drawn
	PosTest_Asynch(0,0,0); // start a position test at the current translated position
	polyCount = GFX_POLYGON_RAM_USAGE; // save the polygon count
}

// run afer drawing an object while picking
void endCheck(Clickable obj) {
	while(GFX_BUSY); // wait for all the polygons to get drawn
	while(PosTestBusy()); // wait for the position test to finish
	if(GFX_POLYGON_RAM_USAGE>polyCount) // if a polygon was drawn
	{
		if(PosTestWresult()<=closeW) {
			// this is currently the closest object under the cursor!
			closeW=PosTestWresult();
			clicked=obj;
		}
	}
}


int main()
{	

	// initialize gl
	glInit();

	u32 rotateX = 0;
	u32 rotateY = 0;

	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);
	
	// used to hold touched position
	touchPosition touchXY;
	
	lcdMainOnBottom(); // we are going to be touching the 3D display

	
	// enable edge outlining, this will be used to show which object is selected
	glEnable(GL_OUTLINE);
	
	//set the first outline color to white
	glSetOutlineColor(0,RGB15(31,31,31));
	
	int viewport[]={0,0,255,191}; // used later for gluPickMatrix()
	
	// setup the rear plane
	glClearColor(0,0,0,0); // set BG to black and clear
	glClearPolyID(0); // the BG and polygons will have the same ID unless a polygon is highlighted
	glClearDepth(0x7FFF);
	
	// setup the camera
	gluLookAt(	0.0, 0.0, 1.0,		//camera possition 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
	
	glLight(0, RGB15(31,31,31) , 0, floattov10(-1.0), 0); // setup the light
	
	while(1) {
		// handle key input
		scanKeys();
		u16 keys = keysHeld();
		if(!(keys & KEY_UP)) rotateX += 3;
		if(!(keys & KEY_DOWN)) rotateX -= 3;
		if(!(keys & KEY_LEFT)) rotateY += 3;
		if(!(keys & KEY_RIGHT)) rotateY -= 3;
		
		// get touchscreen position
		touchRead(&touchXY);
		
		glViewport(0,0,255,191); // set the viewport to fullscreen
		
		// setup the projection matrix for regular drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, 256.0 / 192.0, 0.1, 20); 
		
		glMatrixMode(GL_MODELVIEW); // use the modelview matrix while drawing
		
		glPushMatrix(); // save the state of the current matrix(the modelview matrix)
		{
			glTranslatef32(0,0,floattof32(-6));
			glRotateXi(rotateX); // add X rotation to the modelview matrix
			glRotateYi(rotateY); // add Y rotation to the modelview matrix
			
			glPushMatrix(); // save the state of the modelview matrix while making the first pass
			{ 
				// draw the scene for displaying
				
				glTranslatef32(floattof32(2.9),floattof32(0),floattof32(0)); // translate the modelview matrix to the drawing location
				if(clicked==CONE) {
					glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_ID(1)); // set a poly ID for outlining
				} else {
					glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_ID(0)); // set a poly ID for no outlining (same as BG)
				}
				glCallList((u32*)cone_bin); // draw a green cone from a predefined packed command list
				
				
				glTranslatef32(floattof32(-3),floattof32(1.8),floattof32(2)); // translate the modelview matrix to the drawing location
				if(clicked==CYLINDER) {
					glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_ID(1)); // set a poly ID for outlining
				} else {
					glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_ID(0)); // set a poly ID for no outlining (same as BG)
				}
				glCallList((u32*)cylinder_bin); // draw a blue cylinder from a predefined packed command list
				
				
				glTranslatef32(floattof32(.5),floattof32(-2.6),floattof32(-4)); // translate the modelview matrix to the drawing location
				if(clicked==SPHERE) {
					glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_ID(1)); // set a poly ID for outlining
				} else {
					glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_ID(0)); // set a poly ID for no outlining (same as BG)
				}
				glCallList((u32*)sphere_bin); // draw a red sphere from a predefined packed command list

			}
			glPopMatrix(1); // restores the modelview matrix to where it was just rotated
			
			// draw the scene again for picking
			{
				
				clicked = NOTHING; //reset what was clicked on
				closeW = 0x7FFFFFFF; //reset the distance
				
				//set the viewport to just off-screen, this hides all rendering that will be done during picking
				glViewport(0,192,0,192);
				
				// setup the projection matrix for picking
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPickMatrix((touchXY.px),(191-touchXY.py),4,4,viewport); // render only what is below the cursor
				gluPerspective(60, 256.0 / 192.0, 0.1, 20); // this must be the same as the original perspective matrix
				
				glMatrixMode(GL_MODELVIEW); // switch back to modifying the modelview matrix for drawing
				
				glTranslatef32(floattof32(2.9),floattof32(0),floattof32(0)); // translate the modelview matrix to the drawing location
				startCheck();
				glCallList((u32*)cone_bin); // draw a cone from a predefined packed command list
				endCheck(CONE);
				
				glTranslatef32(floattof32(-3),floattof32(1.8),floattof32(2)); // translate the modelview matrix to the drawing location
				startCheck();
				glCallList((u32*)cylinder_bin); // draw a cylinder from a predefined packed command list
				endCheck(CYLINDER);
				
				glTranslatef32(floattof32(.5),floattof32(-2.6),floattof32(-4)); // translate the modelview matrix to the drawing location
				startCheck();
				glCallList((u32*)sphere_bin); // draw a sphere from a predefined packed command list
				endCheck(SPHERE);
			}
			
		}
		glPopMatrix(1); // restores the modelview matrix to its original state
		
		glFlush(0); // wait for everything to be drawn before starting on the next frame
	}

	return 0;
}//end main 
