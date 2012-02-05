////////
//Wandering Points
//Jason Paller-Rzepka
////////

#include <iostream>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <cmath>
#include "cstdio"
#include <ctime>

#define PI 3.14159265

/*Prototypes*/
float len(float,float);
void display(void);
void reshape(int width, int height);
void timefunc(int);
float timeDiff(void);
/*End Prototypes*/

int width = 640;
int height = 400;
bool randInit = false;
int lastFrameTime = 0;
float diff;
int pointcount = 5;

class Point
{
public:
    static int PointIDCounter;
    int id;
    float vx, vy;
    char cr, cg, cb;
    float x, y;
    float vmax;
    bool active;
    float Cx, Cy, Ax, Ay, Tx, Ty;
    float EPx, EPy, EIx, EIy, IPx, IPy;
    float phi,sign;
    Point();
    void move(void);
    void drawPoint(void);
    void process(void);
private:
    int E; //Enemy
    int P; //Protector

    
    //float VEx, VEy, VPx, VPy // Might not use these, as they exist as E.vx, and P.vx etc...
    
    
};

int Point::PointIDCounter = 0;


Point::Point(void){
    id = Point::PointIDCounter;
    Point::PointIDCounter++;
    if (!randInit)
    {
        srand((unsigned)time(0));
        rand(); //For some reason, this is not ver random... flush it out, so next call is random.
        randInit = true;
    }
    
    x = float(rand())/float(RAND_MAX)*float(width)*0.2+width*0.4;
    y = float(rand())/float(RAND_MAX)*float(height)*0.2+height*0.4;
    vx = 0;
    vy = 0;
    vmax = 50.0;
    if (id == 0) {
        cg = 0; cg = 127; cb = 0;
    } else if (id == 1) {
        cr = 127; cg = 0; cb = 0; //enemy red
    } else if (id == 2) {
        cr = 0; cg = 0; cb = 127; //prot blue
    } else {
        cr = rand()%127;
        cg = rand()%127;
        cb = rand()%127;
    }
    
    active = true;
    E = rand()%pointcount;
    P = rand()%pointcount;
    while (E==id)
    {
        E = rand()%pointcount;
    }
    while (P == E || P == id)
    {
        P = rand()%pointcount;
    }
}
Point* swarm = new Point[pointcount];

void Point::process(void){
    //CODE FOR MOVEMENT
    for(int I=0; I<pointcount; I++){
        if (swarm[I].active){
            swarm[I].EPx = swarm[P].x-swarm[E].x;
            swarm[I].EPy = swarm[P].y-swarm[E].y;
            //float EP = len(swarm[I].EPx,swarm[I].EPy);
            
            swarm[I].EIx = swarm[I].x-swarm[E].x;
            swarm[I].EIy = swarm[I].y-swarm[E].y;
            float EI = len(swarm[I].EIx,swarm[I].EIy);
            
            swarm[I].IPx = swarm[P].x-swarm[I].x;
            swarm[I].IPy = swarm[P].y-swarm[I].y;
            float IP = len(swarm[I].IPx,swarm[I].IPy);

            swarm[I].phi = acosf((swarm[I].EPx*swarm[I].EIx+swarm[I].EPy*swarm[I].EIy)/sqrt((swarm[I].EPx*swarm[I].EPx+swarm[I].EPy*swarm[I].EPy)*(swarm[I].EIx*swarm[I].EIx+swarm[I].EIy*swarm[I].EIy)));
            if (swarm[I].EIy*swarm[I].EPx>=swarm[I].EIx*swarm[I].EPy)
                swarm[I].sign = 1.0;
            else
                swarm[I].sign = -1.0;
            
            swarm[I].Cx = swarm[I].IPx/IP;
            swarm[I].Cy = swarm[I].IPy/IP;
            
            swarm[I].Ax = swarm[I].EIx/EI;
            swarm[I].Ay = swarm[I].EIy/EI;
            
            swarm[I].Tx = swarm[I].Ay*swarm[I].sign;
            swarm[I].Ty = -1.0*swarm[I].Ax*swarm[I].sign;

            swarm[I].vx = (swarm[I].Ax+swarm[I].Cx+swarm[I].Tx)/3.0*swarm[I].vmax;
            swarm[I].vy = (swarm[I].Ay+swarm[I].Cy+swarm[I].Ty)/3.0*swarm[I].vmax;
            
            
            /*glColor3b(127,127,127);
            glBegin(GL_POINTS);
            glVertex2f(swarm[I].x+20.0*swarm[I].Cx,swarm[I].y+20.0*swarm[I].Cy);
            glVertex2f(swarm[I].x+20.0*swarm[I].Ax,swarm[I].y+20.0*swarm[I].Ay);
            glVertex2f(swarm[I].x+20.0*swarm[I].Tx,swarm[I].y+20.0*swarm[I].Ty);
            glEnd();*/
            
            
        }
    }
    
    

}

void Point::move(void){
    x+=diff*vx;
    y+=diff*vy;
    //BOUNDARY BOUNCE:
    /*if (x<0){
        x=-x;
        vx=-vx;
    }
    if (x>width){
        x = 2*width-x;
        vx=-vx;
    }
    if (y<0){
        y=-y;
        vy=-vy;
    }
    if (y>height){
        y=2*height-y;
        vy=-vy;
    }*/
}
 
void Point::drawPoint(void){
    glColor3b(cr,cg,cb);
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
}


float len(float a, float b){
    return sqrt(a*a+b*b);
}
float timeDiff(void)
{
    //Only call once per frame!
    if (lastFrameTime == 0)
        lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
    int now = glutGet(GLUT_ELAPSED_TIME);
    int elapsedMilliseconds = now - lastFrameTime;
    float elapsedTime = elapsedMilliseconds / 1000.0f;
    lastFrameTime = now;
    return elapsedTime;
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    diff = timeDiff();
    
    for(int j=0; j<pointcount; j++){
        if (swarm[j].active==true)
        {
            swarm[j].process();
            swarm[j].move();
        }
        swarm[j].drawPoint();
    }
    glutSwapBuffers();
}

void reshape(int w, int h){
    width = w;
    height = h;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glMatrixMode(GL_MODELVIEW);
}

void timefunc(int){
    glutPostRedisplay();
    glutTimerFunc(30,timefunc,0);
}

int main (int argc, char** argv)
{    
    /*//Inactivate certain points for testing.
    for(int j=1; j<=2; j++){
        swarm[j].active = false;
    }*/
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width,height);
    glutCreateWindow("Enemy and Protector");
    glPointSize(8.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(30,timefunc,0);
    
    glutMainLoop();
    return EXIT_SUCCESS;

}

