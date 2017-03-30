/* hull 3D

Initializes a set of points in 3D and renders them (allowing to
translate/rotate when user presses l/r/u/d/x/X,y/Y,z/Z).

Needs to compute the convex hull.

OpenGL 1.x
Laura Toma
*/

#include "geom.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
//this allows this code to compile both on apple and linux platforms
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <vector>

using namespace std;



/* global variables */


//the array of n points; note: this variable needs to be global
//because it needs to be rendered
vector<point3d>  points;

int n;  //desired number of points


//the convex hull, stored as a list. note: this variable needs to be
//global because it needs to be rendered
vector<triangle3d>  hull;


const int WINDOWSIZE = 500;

//we predefine some colors for convenience
GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};


//keep track of global translation and rotation
GLfloat pos[3] = {0,0,0};
GLfloat theta[3] = {0,0,0};

//cube drawn line or filled
GLint fillmode = 0;

/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
void initialize_points_random();
void draw_points();
void draw_hull();
void draw_xy_rect(GLfloat z, GLfloat* col);
void draw_xz_rect(GLfloat y, GLfloat* col);
void draw_yz_rect(GLfloat x, GLfloat* col);
void cube(GLfloat side);
void filledcube(GLfloat side);
void draw_axes();
void initialize_points_spring();
void draw_sphere(float rad, float x, float y, float z);
void initialize_points_heart();
void initialize_points_house();
void initialize_points_random_vertlines();
void draw_sphereOfSpheres();
void initialize_points_personal();
void initialize_points_cross();
void initialize_points_pyramid();
void beautiful_diamond();
void initialize_points_droplet();

int main(int argc, char** argv) {

  //read number of points from user
  if (argc!=2) {
    printf("usage: hull3d <nbPoints>\n");
    exit(1);
  }
  n = atoi(argv[1]);
  printf("you entered n=%d\n", n);
  assert(n>0);

  //initialize_points_random();
  initialize_points_droplet();

  for (int i = 0; i < points.size(); ++i) {
    printf("point: %d %d %d\n", points[i].x, points[i].y, points[i].z);
  }

  hull = brute_force_hull(points);
  //print_hull(hull);

  /* open a window and initialize GLUT stuff */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display);
  glutKeyboardFunc(keypress);

  /* OpenGL init */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);

  /* Enable transparency */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* setup the camera (i.e. the projection transformation) */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1 /* aspect */, 1, 10.0); /* the frustrum is from z=-1 to z=-10 */
  /* camera is at (0,0,0) looking along negative y axis */

  //initialize the translation to bring the points in the view frustrum which is [-1, -10]
  pos[2] = -3;

  //move it down, look at it from above
  //pos[1] = -1.3;

  /* start the event handler */
  glutMainLoop();

  return 0;
}




/* this function is called whenever the window needs to be rendered */
void display(void) {

  //clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //clear all modeling transformations
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* The default GL window is x=[-1,1], y= [-1,1] with the origin in
  the center.  The view frustrum was set up from z=-1 to z=-10. The
  camera is at (0,0,0) looking along negative z axis.
  */

  /* First we translate our local reference system. pos[] represents
  the cumulative translation entered by the user, and theta[] the
  rotation */
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(theta[0], 1,0,0); //rotate theta[0] around x-axis, etc
  glRotatef(theta[1], 0,1,0);
  glRotatef(theta[2], 0,0,1);

  /* Now we draw the object in the local reference system. Note that
  we draw the object in the local system, and we translate
  the system. */
  draw_points();
  draw_hull();

  //don't need to draw a cube but I found it cool for perspective
  cube(1);


  glFlush();
}



/* this function is called whenever  key is pressed */
void keypress(unsigned char key, int x, int y) {

  switch(key) {

    case 'i':
    //re-initialize
    initialize_points_random();
    //re-compute
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;
    case 'j':
    initialize_points_pyramid();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;
    case 'k':
    initialize_points_cross();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;

    case 'm':
    beautiful_diamond();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;

    case 'n':
    initialize_points_spring();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;

    case 'p':
    draw_sphereOfSpheres();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;

    case 's':
    initialize_points_random_vertlines();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;

    case 't':
    initialize_points_heart();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;

    case 'w':
    initialize_points_droplet();
    hull = brute_force_hull(points);
    glutPostRedisplay();
    break;
    //ROTATIONS
    case 'x':
    theta[0] += 5.0;
    glutPostRedisplay();
    break;
    case 'y':
    theta[1] += 5.0;
    glutPostRedisplay();
    break;
    case 'z':
    theta[2] += 5.0;
    glutPostRedisplay();
    break;
    case 'X':
    theta[0] -= 5.0;
    glutPostRedisplay();
    break;
    case 'Y':
    theta[1] -= 5.0;
    glutPostRedisplay();
    break;
    case 'Z':
    theta[2] -= 5.0;
    glutPostRedisplay();
    break;

    //TRANSLATIONS
    //backward (zoom out)
    case 'b':
    pos[2] -= 0.1;
    glutPostRedisplay();
    break;
    //forward (zoom in)
    case 'f':
    pos[2] += 0.1;
    //glTranslatef(0,0, 0.5);
    glutPostRedisplay();
    break;
    //down
    case 'd':
    pos[1] -= 0.1;
    //glTranslatef(0,0.5,0);
    glutPostRedisplay();
    break;
    //up
    case 'u':
    pos[1] += 0.1;
    //glTranslatef(0,-0.5,0);
    glutPostRedisplay();
    break;
    //left
    case 'l':
    pos[0] -= 0.1;
    glutPostRedisplay();
    break;
    //right
    case 'r':
    pos[0] += 0.1;
    glutPostRedisplay();
    break;

    //fillmode
    case 'c':
    fillmode = !fillmode;
    glutPostRedisplay();
    break;

    case 'q':
    exit(0);
    break;
  }
}//keypress

/* Jack's spiral/spring initializer */
void initialize_points_spring() {

  //clear the vector just to be safe
  points.clear();

  int i;
  point3d p;
  float step = WINDOWSIZE / n;
  for (i = 0; i < n; i++) {
    p.x = (int)(WINDOWSIZE * ((cos(i * step) + 1) * .5));
    p.y = (int)(WINDOWSIZE * ((sin(i * step) + 1) * .5));
    p.z = (int)(i * step);
    points.push_back(p);
  }
}


//draw points sampled on a sphere of radius rad with
//an orgin at x, y, z as input parameters
void draw_sphere(float rad, float x, float y, float z) {

  point3d p;

  int N = 6;
  float u = 2*M_PI/N;
  float v = M_PI/N;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      p.x = x + rad * cos(u * i) * sin(v * j);
      p.y = y + rad * sin(u * i) * sin(v * j);
      p.z = z + rad * cos(j*v);
      points.push_back(p);
    }
  }
}


void initialize_points_droplet() {
  points.clear();

  point3d p;

  int offset = 200;
  int scaleX = 120;
  int scaleY = 250;
  int scaleZ = 520;

  for (int i = 0; i < n; ++i) {
    p.x = (int)scaleX*(1 - sin(i)) * cos(i) + offset;
    p.y = (int)scaleY*(sin(1-i)) + offset;
    p.z = -random() % scaleZ;

    points.push_back(p);
  }

}

/* heart test case */
void initialize_points_heart() {
  //clear the points just to be safe
  points.clear();

  int i;
  point3d p;

  float R = 100;

  for (i = 0; i < n; i++) {
    p.x = R*4.f*pow(sin(i),3.f);
    p.y = R*0.25f*(13*cos(i)-5*cos(2.f*i)-2.f*cos(3.f*i)-cos(4.f*i));
    p.z = random() % 200;
    points.push_back(p);
  }
}

void initialize_points_house(){
  points.clear();
  //draw a house
  point3d p;
  for (int j = 1; j < 11; j++){
    int i = j;
    int ymult = 1;
    if (i>5){
      i = i-5;
      ymult = 2;
    }
    if(i < 3){
      p.x = 125;
      p.y = 125*ymult;
      p.z = 125*i;
    }
    if (i == 3 ){
      p.x = 250;
      p.y = 125*ymult;
      p.z = 375;
    }
    if (i > 3){
      p.x = 375;
      p.y = 125*ymult;
      p.z = 125*(i-3);
    }
    points.push_back(p);
  }

}

//creates random location vetical lines
//that are five points in length
//Ryan St. Pierre testcase
void initialize_points_random_vertlines() {

  //clear the vector just to be safe                                                                                                                                                          \

  points.clear();

  int i;
  point3d p;
  int x;
  int y;
  //outer loop for each line
  for(int i = 0; i< n/5; i++){
    x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    y = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    //inner loop to determin z-coordinate of each point in the line
    for (int k=0; k<5; k++) {
      p.x = x;
      p.y = y;
      p.z=  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
      points.push_back(p);
    }
  }
}

void draw_sphereOfSpheres() {

  points.clear();

  point3d p;
  int NR = 2;
  float rMax = 200.0;
  float dr = rMax / NR;
  int NPhi = 3;
  int NTheta = 3;
  float dphi = 2*M_PI/NPhi;
  float dtheta = M_PI/NTheta;
  float x;
  float y;
  float z;

  //For each point IN and ON sphere, draw a spherical shell
  for (int i = 0; i < NTheta; i++) {
    for (int j = 0; j < NPhi; j++){
      for (int k = 1; k < NR; k++){

        x = WINDOWSIZE/2 + (dr * k) * sin(dtheta * i)*cos(dphi * j);
        y = WINDOWSIZE/2 + (dr * k) * sin(dtheta * i)*sin(dphi * j);
        z = WINDOWSIZE/2 + (dr * k) * cos(dtheta * i);
        draw_sphere(rMax,x,y,z);
      }
    }
  }

  // where is the part where you add the point? Oh, so you are just drawing them for fun?
}

void initialize_points_personal() {

  glTranslatef(0,0,0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100, 1 /* aspect */, 1, 10.0);

  points.clear();
  point3d p;

  glColor3fv(blue);

  float rad = 400;
  float u = 2*M_PI/n;
  float v = 2*M_PI/n;

  int i;
  for (i=0; i<n; i++) {

    p.x = rad * cos (6i*u) * sin(i*v) + 330;
    p.y = rad * sin (3*i*u) * sin(i*v) + 50;
    p.z = rad * cos(i*v);

    glTranslatef(p.x,p.y,p.z);
    cube(.01);
    glTranslatef(-p.x,-p.y,-p.z);

    points.push_back(p);

  }
}







/* initialize the array of points stored in global variable points[]
with random points in the range x= [0,WINDOWSIZE],
y=[0,WINDOWSIZE], z=[0,WINDOWSIZE] */
void initialize_points_random() {

  //clear the vector just to be safe
  points.clear();

  int i;
  point3d p;
  for (i=0; i<n; i++) {
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    p.z=  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    points.push_back(p);
  }
}

void initialize_points_cross() {

  //clear the vector just to be safe
  points.clear();

  int i;
  point3d p;
  for (i=0; i<n/2; i++) {
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.5*WINDOWSIZE));
    p.y = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.5*WINDOWSIZE));
    p.z =  250;
    points.push_back(p);
  }
  for (i=0; i<n/2; i++){
    p.x = (int)(.5*WINDOWSIZE)/2 + random() % ((int)(.5*WINDOWSIZE));
    p.y = 250;
    p.z = (int)(.5*WINDOWSIZE)/2 + random() % ((int)(.5*WINDOWSIZE));
    points.push_back(p);
  }
}




/* x is a value in [0,WINDOWSIZE] is mapped to [-1,1] */
GLfloat windowtoscreen(GLfloat x) {
  return (-1 + 2*x/WINDOWSIZE);
}




/* ****************************** */
/* Draw the array of points stored in global variable points[] each
point is drawn as a small square.  SHOULD BE A SPHERE/cube not a square

NOTE: The points are in the range x=[0, WINDOWSIZE], y=[0,
WINDOWSIZE], z=[0, WINDOWSIZE] and they must be mapped back into x=[-1,1],
y=[-1, 1], z=[-1,1]
*/
void draw_points(){

  const int R= 1;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set color
  glColor3fv(yellow);

  int i;
  for (i=0; i < points.size(); i++) {
    //draw a small filled cube centered at (points[i].x, points[i].y, points[i].z)
    //save local coordinate system
    glPushMatrix();
    //translate our local coordinate system to the point
    glTranslatef(windowtoscreen(points[i].x),
    windowtoscreen(points[i].y),
    windowtoscreen(points[i].z));
    //draw the cube
    filledcube(.01);
    //move local system back to where we were
    glPopMatrix();
  }

}//draw_points

void initialize_points_pyramid(){
  //clear the vector just to be safe
  points.clear();
  float maxZ = WINDOWSIZE;
  float minZ = WINDOWSIZE/5;
  float midZ = (maxZ - minZ)/2;
  float maxX = WINDOWSIZE;
  float minX = WINDOWSIZE/5;
  float midX = (maxX + minX)/2;
  float maxY = WINDOWSIZE;
  float minY = WINDOWSIZE/5;
  float midY = (maxY + minY)/2;
  int i;
  point3d p;
  //first do the 5 endpoints
  p.x = maxX;
  p.y = minY;
  p.z = minZ;
  points.push_back(p);

  p.x = maxX;
  p.y = maxY;
  p.z = minZ;
  points.push_back(p);

  p.x = minX;
  p.y = minY;
  p.z = minZ;
  points.push_back(p);

  p.x = minX;
  p.y = maxY;
  p.z = minZ;
  points.push_back(p);

  p.x = midX;
  p.y = midY;
  p.z = maxZ;
  points.push_back(p);

  for (i=0; i<n-5; i++) {
    p.x = (midX+minX)/2 + ((maxX - minX)/2) * ((float)rand()) / ((float)(RAND_MAX));
    p.y = (midY+minY)/2 + ((maxY - minY)/2) * ((float)rand()) / ((float)(RAND_MAX));
    p.z = minZ + midZ * ((float)rand()) / ((float)(RAND_MAX));
    points.push_back(p);
  }
}



/* ****************************** */
/* draw the list of points stored in global variable hull[].

NOTE: the points are in the range x=[0, WINDOWSIZE], y=[0,
WINDOWSIZE], z=[0, 10] and they must be mapped back into x=[-1,1],
y=[-1, 1] */
void draw_hull(){

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  if (hull.size() >0) {
    int i;
    for (i=0; i< hull.size(); i++) {

      glColor4f(0.1, 0.2, 0.9, 0.4);
      glBegin(GL_POLYGON);

      glVertex3f(windowtoscreen(hull[i].a->x), windowtoscreen(hull[i].a->y), windowtoscreen(hull[i].a->z));

      glVertex3f(windowtoscreen(hull[i].b->x), windowtoscreen(hull[i].b->y), windowtoscreen(hull[i].b->z));

      glVertex3f(windowtoscreen(hull[i].c->x), windowtoscreen(hull[i].c->y), windowtoscreen(hull[i].c->z));
      //  glPopMatrix();

      glEnd();

      //draw the triangle

    }
  }//if (hull not empty)
}



void beautiful_diamond() {

  //clear the vector
  points.clear();

  //make top of pyramid
  point3d top;
  top.x = 0;
  top.y = 200;
  top.z = 0;
  points.push_back(top);

  //make bottom of pyramid
  point3d bottom;
  bottom.x = 0;
  bottom.y = 100;
  bottom.z = 0;
  points.push_back(bottom);

  //make point1 of square
  point3d point1;
  point1.x = 50;
  point1.y = 150;
  point1.z = 50;
  points.push_back(point1);

  //make point2 of square
  point3d point2;
  point2.x = -50;
  point2.y = 150;
  point2.z = 50;
  points.push_back(point2);

  //make point3 of square
  point3d point3;
  point3.x = 50;
  point3.y = 150;
  point3.z = -50;
  points.push_back(point3);

  //make point4 of square
  point3d point4;
  point4.x = -50;
  point4.y = 150;
  point4.z = -50;
  points.push_back(point4);

  //put all remaining points inside the pyramid
  int remainingPoints = n - 6;
  for(int i = 0; i < remainingPoints; ++i){
    point3d temp;
    temp.x = 0;
    temp.y = 150;
    temp.z = 0;
    points.push_back(temp);
  }


}




//draw a square x=[-side,side] x y=[-side,side] at depth z
void draw_xy_rect(GLfloat z, GLfloat side, GLfloat* col) {

  glColor3fv(col);
  glBegin(GL_POLYGON);
  glVertex3f(-side,-side, z);
  glVertex3f(-side,side, z);
  glVertex3f(side,side, z);
  glVertex3f(side,-side, z);
  glEnd();
}


//draw a square y=[-side,side] x z=[-side,side] at given x
void draw_yz_rect(GLfloat x, GLfloat side, GLfloat* col) {

  glColor3fv(col);
  glBegin(GL_POLYGON);
  glVertex3f(x,-side, side);
  glVertex3f(x,side, side);
  glVertex3f(x,side, -side);
  glVertex3f(x,-side, -side);
  glEnd();
}


//draw a square x=[-side,side] x z=[-side,side] at given y
void draw_xz_rect(GLfloat y, GLfloat side, GLfloat* col) {

  glColor3fv(col);
  glBegin(GL_POLYGON);
  glVertex3f(-side,y, side);
  glVertex3f(-side,y, -side);
  glVertex3f(side,y, -side);
  glVertex3f(side,y, side);
  glEnd();
}

//draw a cube
void cube(GLfloat side) {
  GLfloat f = side, b = -side;

  if (fillmode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }


  /* back face  BLUE*/
  draw_xy_rect(b,side, blue);
  /* front face  RED*/
  draw_xy_rect(f,side, red);
  /* side faces  GREEN*/
  draw_yz_rect(b, side, green);
  draw_yz_rect(f, side, green);
  //up, down faces missing to be able to see inside

  /* middle z=0 face CYAN*/
  draw_xy_rect(0, side, cyan);
  /* middle x=0 face WHITE*/
  draw_yz_rect(0,side, gray);
  /* middle y=0 face  pink*/
  draw_xz_rect(0, side, magenta);
}



//draw a filled cube  [-side,side]^3
void filledcube(GLfloat side) {

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /* back, front faces */
  draw_xy_rect(-side,side, yellow);
  draw_xy_rect(side,side, yellow);

  /* left, right faces*/
  draw_yz_rect(-side, side, yellow);
  draw_yz_rect(side, side, yellow);

  /* up, down  faces  */
  draw_xz_rect(side,side, yellow);
  draw_xz_rect(-side,side, yellow);
}
