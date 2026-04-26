#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

//void drawFlagStand(float x, float y1, float y2);
void drawSolidFlag(float x, float y, float r, float g, float b);
void drawBangladeshFlag(float x, float y);
void drawFlagStand(float x, float y1, float y2);
void drawFlagStand(float x, float y1, float y2) {
    glColor3f(0.4f, 0.26f, 0.13f);
    glBegin(GL_LINES);
    glVertex2f(x, y1);
    glVertex2f(x, y2);
    glEnd();
}
void drawSolidFlag(float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 60, y);
    glVertex2f(x + 60, y + 40);
    glVertex2f(x, y + 40);
    glEnd();
}
void drawBangladeshFlag(float x, float y) {
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 60, y);
    glVertex2f(x + 60, y + 40);
    glVertex2f(x, y + 40);
    glEnd();

    glColor3f(0.8f, 0.0f, 0.0f);
    float cx = x + 30;
    float cy = y + 20;
    float radius = 10;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 50; i++) {
        float angle = 2.0f * M_PI * i / 50;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}

// --- Scene selector: 0 = Annex/Field/Gallery, 1 = Dome/Road/Trees ---
int currentScene = 0;
bool domeInitialized = false;
bool campusSceneInitialized = false;
bool isNightMode = false;


// --- All your existing variables and functions for Annex/Field/Gallery ---
// ... (keep all your code as is) ...
// --- Day/Night/Rain/Flower Trees Toggle ---
bool isDay = true;
bool isRain = false;
bool showFlowerTrees = false;

// --- Star positions for night sky ---
const int NUM_STARS = 60;
float starX[NUM_STARS];
float starY[NUM_STARS];
bool starsInitialized = false;

// --- Rain and Cloud Animation ---
const int NUM_RAIN = 200;
float rainX[NUM_RAIN];
float rainY[NUM_RAIN];
bool rainInitialized = false;

float cloudOffset = 0.0f;
float cloudSpeed = 0.002f;



// Animation variables for campus scene
float airplaneX = -100.0f;
float airplaneY = 600.0f;
float cloudX1 = -60.0f;
float cloudX2 = -200.0f;
float cloudX3 = -350.0f;
float cloudX4 = 100.0f;
float cloudSpeed1 = 0.3f;
float cloudSpeed2 = 0.3f;
float cloudSpeed3 = 0.3f;
float cloudSpeed4 = 0.3f;

// --- Utility functions for campus scene ---
void drawEllipseCampus(float cx, float cy, float rx, float ry, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = rx * cosf(theta);
        float y = ry * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}
void drawCircleCampus(float cx, float cy, float r) {
    int num_segments = 50;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}
void drawCloudCampus(float x, float y) {
    if (isNightMode) glColor3f(0.7f, 0.7f, 0.7f);
    else glColor3f(1.0f, 1.0f, 1.0f);
    drawCircleCampus(x, y, 20);
    drawCircleCampus(x + 25, y + 10, 25);
    drawCircleCampus(x + 50, y, 20);
}
void drawSunMoonCampus(float x, float y) {
    if (isNightMode) {
        glColor3f(0.85f, 0.85f, 0.85f);
        drawCircleCampus(x, y, 35);
    } else {
        glColor3f(1.0f, 0.84f, 0.0f);
        drawCircleCampus(x, y, 40);
    }
}
void drawAirplaneRelative() {
    glColor3f(0.85f, 0.85f, 0.85f);
    glBegin(GL_POLYGON);
    glVertex2f(-100, 0); glVertex2f(60, 0); glVertex2f(70, 5); glVertex2f(80, 15);
    glVertex2f(70, 25); glVertex2f(60, 30); glVertex2f(-100, 30); glVertex2f(-85, 15);
    glEnd();
    glColor3f(0.2f, 0.4f, 0.6f);
    glBegin(GL_POLYGON);
    glVertex2f(60, 20); glVertex2f(75, 17); glVertex2f(70, 10); glVertex2f(60, 12);
    glEnd();
    glColor3f(0.1f, 0.2f, 0.3f);
    for (int i = 0; i < 6; ++i) {
        float windowLeftX = -85 + i * 25;
        float windowBottomY = 12;
        float windowWidth = 10;
        float windowHeight = 8;
        glBegin(GL_QUADS);
        glVertex2f(windowLeftX, windowBottomY);
        glVertex2f(windowLeftX + windowWidth, windowBottomY);
        glVertex2f(windowLeftX + windowWidth, windowBottomY + windowHeight);
        glVertex2f(windowLeftX, windowBottomY + windowHeight);
        glEnd();
    }
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_POLYGON);
    glVertex2f(30, 5); glVertex2f(-30, -40); glVertex2f(-60, -30); glVertex2f(-20, 5);
    glEnd();
    glBegin(GL_QUADS);
     glVertex2f(-55, 7); glVertex2f(-85, -25); glVertex2f(-100, -15); glVertex2f(-75, 10);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(30,30); glVertex2f(-20, 30); glVertex2f(-50, 50); glVertex2f(-30, 60);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-55,30); glVertex2f(-75, 30); glVertex2f(-100, 50); glVertex2f(-85, 55);
    glEnd();
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(-90, 30); glVertex2f(-80, 75); glVertex2f(-75, 75); glVertex2f(-65, 30);
    glEnd();
}
void drawTreeCampus(float x, float y, float trunkHeight, float canopyRadius) {
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(x - 5, y); glVertex2f(x + 5, y); glVertex2f(x + 5, y + trunkHeight); glVertex2f(x - 5, y + trunkHeight);
    glEnd();
    if (isNightMode) glColor3f(0.05f, 0.3f, 0.05f);
    else glColor3f(0.1f, 0.5f, 0.1f);
    drawCircleCampus(x, y + trunkHeight + canopyRadius / 2, canopyRadius);
}
void drawCenteredTextCampus(const char* text, float boxX, float boxY, float boxWidth, float yOffset, void* font) {
    int textWidth = 0;
    int len = (int)strlen(text);
    for (int i = 0; i < len; ++i) textWidth += glutBitmapWidth(font, text[i]);
    float x = boxX + (boxWidth - textWidth) / 2.0f;
    glRasterPos2f(x, boxY + yOffset);
    for (int i = 0; i < len; ++i) glutBitmapCharacter(font, text[i]);
}
void drawFlagStandCampus(float x, float y1, float y2) {
    glColor3f(0.4f, 0.26f, 0.13f);
    glBegin(GL_LINES); glVertex2f(x, y1); glVertex2f(x, y2); glEnd();
}
void drawBangladeshFlagCampus(float x, float y) {
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS); glVertex2f(x, y); glVertex2f(x + 60, y); glVertex2f(x + 60, y + 40); glVertex2f(x, y + 40); glEnd();
    glColor3f(0.8f, 0.0f, 0.0f);
    float cx = x + 30, cy = y + 20, radius = 10;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 50; i++) {
        float angle = 2.0f * M_PI * i / 50;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}
void drawSolidFlagCampus(float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS); glVertex2f(x, y); glVertex2f(x + 60, y); glVertex2f(x + 60, y + 40); glVertex2f(x, y + 40); glEnd();
}
void displayCampusScene() {
    if (isNightMode) glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    else glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSunMoonCampus(800, 620);
    drawCloudCampus(cloudX1, 600);
    drawCloudCampus(cloudX2, 650);
    drawCloudCampus(cloudX3, 625);
    drawCloudCampus(cloudX4, 650);

    if (isNightMode) glColor3f(0.15f, 0.3f, 0.15f);
    else glColor3f(0.3f, 0.6f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f); glVertex2f(0.0f, 250.0f); glVertex2f(900.0f, 250.0f); glVertex2f(900.0f, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawEllipseCampus(500.0f, 130.0f, 180.0f, 40.0f, 100);
    if (isNightMode) glColor3f(0.0f, 0.5f, 0.5f);
    else glColor3f(0.0f, 1.0f, 1.0f);
    drawEllipseCampus(500.0f, 130.0f, 160.0f, 30.0f, 100);
    glColor3f(0.529f, 0.808f, 0.980f);
    glBegin(GL_QUADS);
        glVertex2f(310.0f, 250.0f);
        glVertex2f(310.0f, 450.0f);
        glVertex2f(600.0f, 450.0f);
        glVertex2f(600.0f, 250.0f);
    glEnd();


    glColor3f(0.15f, 0.4f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(300.0f, 480.0f);
        glVertex2f(300.0f, 450.0f);
        glVertex2f(600.0f, 450.0f);
        glVertex2f(600.0f, 480.0f);
    glEnd();

    glColor3f(0.2745f, 0.5098f, 0.7059f); // Steel Blue

    glBegin(GL_QUADS);
        glVertex2f(300.0f, 250.0f);
        glVertex2f(310.0f, 250.0f);
        glVertex2f(310.0f, 450.0f);
        glVertex2f(300.0f, 450.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(380.0f, 250.0f);
        glVertex2f(390.0f, 250.0f);
        glVertex2f(390.0f, 450.0f);
        glVertex2f(380.0f, 450.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(520.0f, 250.0f);
        glVertex2f(530.0f, 250.0f);
        glVertex2f(530.0f, 450.0f);
        glVertex2f(520.0f, 450.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(590.0f, 250.0f);
        glVertex2f(590.0f, 450.0f);
        glVertex2f(600.0f, 450.0f);
        glVertex2f(600.0f, 250.0f);
    glEnd();

    glColor3f(0.2745f, 0.5098f, 0.7059f);

    glBegin(GL_POLYGON);
        glVertex2f(610.0f, 480.0f);
        glVertex2f(610.0f, 470.0f);
        glVertex2f(450.0f, 550.0f);
        glVertex2f(450.0f, 560.0f);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(290.0f, 470.0f);
        glVertex2f(290.0f, 480.0f);
        glVertex2f(450.0f, 560.0f);
        glVertex2f(450.0f, 550.0f);
    glEnd();

    glColor3f(0.2745f, 0.5098f, 0.7059f);
    glBegin(GL_POLYGON);
    glVertex2f(320.0f, 250.0f);
    glVertex2f(320.0f, 450.0f);
    glVertex2f(322.0f, 450.0f);
    glVertex2f(322.0f, 250.0f);
    glEnd();


    glBegin(GL_POLYGON);
    glVertex2f(340.0f, 250.0f);
    glVertex2f(340.0f, 450.0f);
    glVertex2f(342.0f, 450.0f);
    glVertex2f(342.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(360.0f, 250.0f);
    glVertex2f(360.0f, 450.0f);
    glVertex2f(362.0f, 450.0f);
    glVertex2f(362.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(400.0f, 250.0f);
    glVertex2f(400.0f, 450.0f);
    glVertex2f(402.0f, 450.0f);
    glVertex2f(402.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(420.0f, 250.0f);
    glVertex2f(420.0f, 450.0f);
    glVertex2f(422.0f, 450.0f);
    glVertex2f(422.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(440.0f, 250.0f);
    glVertex2f(440.0f, 450.0f);
    glVertex2f(442.0f, 450.0f);
    glVertex2f(442.0f, 250.0f);
    glEnd();


    glBegin(GL_POLYGON);
    glVertex2f(460.0f, 250.0f);
    glVertex2f(460.0f, 450.0f);
    glVertex2f(462.0f, 450.0f);
    glVertex2f(462.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(480.0f, 250.0f);
    glVertex2f(480.0f, 450.0f);
    glVertex2f(482.0f, 450.0f);
    glVertex2f(482.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(500.0f, 250.0f);
    glVertex2f(500.0f, 450.0f);
    glVertex2f(502.0f, 450.0f);
    glVertex2f(502.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(540.0f, 250.0f);
    glVertex2f(540.0f, 450.0f);
    glVertex2f(542.0f, 450.0f);
    glVertex2f(542.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(560.0f, 250.0f);
    glVertex2f(560.0f, 450.0f);
    glVertex2f(562.0f, 450.0f);
    glVertex2f(562.0f, 250.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(580.0f, 250.0f);
    glVertex2f(580.0f, 450.0f);
    glVertex2f(582.0f, 450.0f);
    glVertex2f(582.0f, 250.0f);
    glEnd();


    glColor3f(0.2745f, 0.5098f, 0.7059f);
    glBegin(GL_POLYGON);
    glVertex2f(300.0f, 390.0f);
    glVertex2f(300.0f, 400.0f);
    glVertex2f(600.0f, 400.0f);
    glVertex2f(600.0f, 390.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(300.0f, 320.0f);
    glVertex2f(300.0f, 330.0f);
    glVertex2f(600.0f, 330.0f);
    glVertex2f(600.0f, 320.0f);
    glEnd();

    glLineWidth(1.5f);
    glColor3f(0.15f, 0.4f, 0.6f); // Darker blue for outline

    glBegin(GL_LINE_LOOP);
        glVertex2f(610.0f, 480.0f);
        glVertex2f(610.0f, 470.0f);
        glVertex2f(450.0f, 550.0f);
        glVertex2f(450.0f, 560.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex2f(290.0f, 470.0f);
        glVertex2f(290.0f, 480.0f);
        glVertex2f(450.0f, 560.0f);
        glVertex2f(450.0f, 550.0f);
    glEnd();

    glColor3f(0.690f, 0.768f, 0.870f);
    glBegin(GL_TRIANGLES);
        glVertex2f(300.0f, 480.0f);
        glVertex2f(450.0f, 550.0f);
        glVertex2f(600.0f, 480.0f);
    glEnd();
drawCenteredTextCampus("AMERICAN INTERNATIONAL UNIVERSITY-BANGLADESH", 300.0f, 450.0f, 300.0f, 10.0f, GLUT_BITMAP_HELVETICA_10);

    glPushMatrix();
    glTranslatef(airplaneX, airplaneY, 0.0f);
    drawAirplaneRelative();
    glPopMatrix();
    // Building Annex
    glColor3f(0.7686f, 0.3843f, 0.0627f);
    glBegin(GL_QUADS);
        glVertex2f(600.0f, 250.0f);
        glVertex2f(600.0f, 460.0f);
        glVertex2f(680.0f, 460.0f);
        glVertex2f(680.0f, 250.0f);
    glEnd();

    glColor3f(0.7529f, 0.7529f, 0.7529f);
    glBegin(GL_QUADS);
        glVertex2f(600.0f, 460.0f);
        glVertex2f(600.0f, 470.0f);
        glVertex2f(700.0f, 470.0f);
        glVertex2f(700.0f, 460.0f);
    glEnd();

    glColor3f(0.7529f, 0.7529f, 0.7529f);
    glBegin(GL_QUADS);
        glVertex2f(680.0f, 250.0f);
        glVertex2f(680.0f, 460.0f);
        glVertex2f(690.0f, 460.0f);
        glVertex2f(690.0f, 250.0f);
    glEnd();

    glColor3f(0.7686f, 0.3843f, 0.0627f);
    glBegin(GL_QUADS);
        glVertex2f(690.0f, 250.0f);
        glVertex2f(690.0f, 430.0f);
        glVertex2f(770.0f, 430.0f);
        glVertex2f(770.0f, 250.0f);
    glEnd();

    glColor3f(0.7529f, 0.7529f, 0.7529f);
    glBegin(GL_QUADS);
        glVertex2f(690.0f, 430.0f);
        glVertex2f(690.0f, 440.0f);
        glVertex2f(780.0f, 440.0f);
        glVertex2f(780.0f, 430.0f);
    glEnd();


    glBegin(GL_QUADS);
        glVertex2f(620.0f, 390.0f);
        glVertex2f(620.0f, 440.0f);
        glVertex2f(660.0f, 440.0f);
        glVertex2f(660.0f, 390.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(620.0f, 300.0f);
        glVertex2f(620.0f, 350.0f);
        glVertex2f(660.0f, 350.0f);
        glVertex2f(660.0f, 300.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(710.0f, 370.0f);
        glVertex2f(710.0f, 420.0f);
        glVertex2f(750.0f, 420.0f);
        glVertex2f(750.0f, 370.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(710.0f, 290.0f);
        glVertex2f(710.0f, 340.0f);
        glVertex2f(750.0f, 340.0f);
        glVertex2f(750.0f, 290.0f);
    glEnd();

        glColor3f(0.7686f, 0.3843f, 0.0627f);
    glBegin(GL_QUADS);
      glVertex2f(770.0f, 400.0f);
        glVertex2f(900.0f, 450.0f);
        glVertex2f(900.0f, 250.0f);
        glVertex2f(770.0f, 250.0f);


    glColor3f(0.7529f, 0.7529f, 0.7529f);
    glBegin(GL_QUADS);
        glVertex2f(770.0f, 400.0f);
        glVertex2f(770.0f, 410.0f);
        glVertex2f(920.0f, 460.0f);
        glVertex2f(920.0f, 450.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex2f(770.0f, 400.0f);
        glVertex2f(770.0f, 410.0f);
        glVertex2f(920.0f, 460.0f);
        glVertex2f(920.0f, 450.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(770.0f, 320.0f);
        glVertex2f(770.0f, 330.0f);
        glVertex2f(920.0f, 380.0f);
        glVertex2f(920.0f, 370.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex2f(770.0f, 320.0f);
        glVertex2f(770.0f, 330.0f);
        glVertex2f(920.0f, 380.0f);
        glVertex2f(920.0f, 370.0f);
    glEnd();

    glColor3f(0.7529f, 0.7529f, 0.7529f);
      glBegin(GL_POLYGON);
      glVertex2f(765.0f, 250.0f);
      glVertex2f(765.0f, 430.0f);
      glVertex2f(770.0f, 430.0f);
      glVertex2f(770.0f, 250.0f);
    glEnd();

// left Annex
    glColor3f(0.7529f, 0.7529f, 0.7529f);

    glBegin(GL_QUADS);
        glVertex2f(130.0f, 390.0f);
        glVertex2f(130.0f, 410.0f);
        glVertex2f(300.0f, 410.0f);
        glVertex2f(300.0f, 390.0f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(100.0f, 310.0f);
        glVertex2f(100.0f, 330.0f);
        glVertex2f(300.0f, 330.0f);
        glVertex2f(300.0f, 310.0f);
    glEnd();

     glColor3f(0.7686f, 0.3843f, 0.0627f);

    glBegin(GL_QUADS);
        glVertex2f(140.0f, 330.0f);
        glVertex2f(140.0f, 390.0f);
        glVertex2f(300.0f, 390.0f);
        glVertex2f(300.0f, 330.0f);
    glEnd();

        glBegin(GL_QUADS);
        glVertex2f(120.0f, 310.0f);
        glVertex2f(120.0f, 250.0f);
        glVertex2f(300.0f, 250.0f);
        glVertex2f(300.0f, 310.0f);
    glEnd();


    glColor3f(0.7529f, 0.7529f, 0.7529f);

    glBegin(GL_QUADS);
        glVertex2f(180.0f, 340.0f);
        glVertex2f(180.0f, 370.0f);
        glVertex2f(250.0f, 370.0f);
        glVertex2f(250.0f, 340.0f);
    glEnd();

     glBegin(GL_QUADS);
        glVertex2f(180.0f, 260.0f);
        glVertex2f(180.0f, 290.0f);
        glVertex2f(250.0f, 290.0f);
        glVertex2f(250.0f, 260.0f);
    glEnd();



    // Flags
    drawFlagStand(370, 225, 375);
    drawFlagStand(410, 225, 375);
    drawFlagStand(490, 225, 375);
    drawFlagStand(530, 225, 375);

    drawSolidFlag(370, 325, 1.0f, 0.5f, 0.0f); // Orange flag
    drawSolidFlag(410, 325, 1.0f, 0.0f, 0.0f); // Red flag
    drawSolidFlag(490, 325, 1.0f, 1.0f, 1.0f); // White flag
    drawSolidFlag(530, 325, 0.0f, 1.0f, 0.0f); // Green flag
    drawFlagStand(450, 225, 400);
    drawBangladeshFlag(450, 350); // Bangladesh flag

     // Road
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(0.0f, 180.0f);
        glVertex2f(0.0f, 230.0f);
        glVertex2f(900.0f, 230.0f);
        glVertex2f(900.0f, 180.0f);
    glEnd();


    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        for (float i = 50.0f; i < 850.0f; i += 80.0f) {
            glVertex2f(i, 205.0f);
            glVertex2f(i + 40.0f, 205.0f);
        }
    glEnd();
    glLineWidth(1.0f);
     // Trees
    drawTreeCampus(350, 155, 110, 40);
    drawTreeCampus(470, 170, 110, 40);
    drawTreeCampus(580, 170, 110, 45);
    drawTreeCampus(680, 150, 110, 40);

    glutSwapBuffers();
    glFlush();
}

// --- Animation update for campus scene ---
void updateCampusScene(int value) {
    if (currentScene != 2) return; // Only animate if campus scene is active
    cloudX1 += cloudSpeed1; cloudX2 += cloudSpeed2; cloudX3 += cloudSpeed3; cloudX4 += cloudSpeed4;
    airplaneX += 1.0f;
    if (airplaneX > 900) airplaneX = -300.0f;
    if (cloudX1 > 900) cloudX1 = -60;
    if (cloudX2 > 900) cloudX2 = -60;
    if (cloudX3 > 900) cloudX3 = -60;
    if (cloudX4 > 900) cloudX4 = -60;
    glutPostRedisplay();
    glutTimerFunc(16, updateCampusScene, 0);
}
void keyboardCampusScene(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A') {
        isNightMode = !isNightMode;
        glutPostRedisplay();
    }
}
// Utility: Draw filled circle at (cx,cy) with radius r and color (r,g,b)
void drawCircle(float cx, float cy, float r, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 40; ++i) {
        float angle = 2.0f * 3.1415926f * i / 40;
        glVertex2f(cx + cosf(angle) * r, cy + sinf(angle) * r);
    }
    glEnd();
}

// --- Draw a single flower at (cx,cy) with a given scale and color ---
void drawFlower(float cx, float cy, float scale, float r, float g, float b) {
    // Petals
    for (int i = 0; i < 5; ++i) {
        float angle = 2.0f * 3.1415926f * i / 5;
        float px = cx + cos(angle) * 0.018f * scale;
        float py = cy + sin(angle) * 0.018f * scale;
        drawCircle(px, py, 0.012f * scale, r, g, b);
    }
    // Center (yellow)
    drawCircle(cx, cy, 0.011f * scale, 1.0f, 0.9f, 0.1f);
}

// --- Draw a flower tree at (base_x, base_y) with given height, branch spread, and color offset ---
void drawFlowerTree(float base_x, float base_y, float height, float spread, int colorOffset) {
    // Draw trunk (root to top)
    glLineWidth(7.0f);
    glColor3f(0.45f, 0.26f, 0.13f); // brown
    glBegin(GL_LINES);
    glVertex2f(base_x, base_y);
    glVertex2f(base_x, base_y + height * 0.65f);
    glEnd();

    // Draw root/base (ellipse)
    glColor3f(0.35f, 0.18f, 0.08f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 40; ++i) {
        float angle = 2.0f * 3.1415926f * i / 40;
        float rx = 0.018f * cos(angle);
        float ry = 0.012f * sin(angle);
        glVertex2f(base_x + rx, base_y - 0.012f + ry);
    }
    glEnd();

    // Draw branches
    glLineWidth(4.0f);
    glColor3f(0.45f, 0.26f, 0.13f);
    glBegin(GL_LINES);
    // Left branch
    glVertex2f(base_x, base_y + height * 0.45f);
    glVertex2f(base_x - spread * 0.5f, base_y + height * 0.75f);
    // Right branch
    glVertex2f(base_x, base_y + height * 0.55f);
    glVertex2f(base_x + spread * 0.5f, base_y + height * 0.80f);
    // Left upper branch
    glVertex2f(base_x - spread * 0.2f, base_y + height * 0.70f);
    glVertex2f(base_x - spread * 0.6f, base_y + height * 0.95f);
    // Right upper branch
    glVertex2f(base_x + spread * 0.2f, base_y + height * 0.78f);
    glVertex2f(base_x + spread * 0.6f, base_y + height * 1.00f);
    glEnd();
    glLineWidth(1.0f);

    // Draw flowers on branches (colorful)
    float colors[6][3] = {
        {1.0f, 0.2f, 0.2f}, // red
        {1.0f, 0.5f, 0.0f}, // orange
        {1.0f, 0.8f, 0.2f}, // yellow
        {0.2f, 0.8f, 0.2f}, // green
        {0.2f, 0.4f, 1.0f}, // blue
        {0.8f, 0.2f, 1.0f}  // violet
    };
    // Main branch top
    drawFlower(base_x, base_y + height * 0.65f, 1.0f, colors[(0+colorOffset)%6][0], colors[(0+colorOffset)%6][1], colors[(0+colorOffset)%6][2]);
    // Left branch tip
    drawFlower(base_x - spread * 0.5f, base_y + height * 0.75f, 0.9f, colors[(1+colorOffset)%6][0], colors[(1+colorOffset)%6][1], colors[(1+colorOffset)%6][2]);
    // Right branch tip
    drawFlower(base_x + spread * 0.5f, base_y + height * 0.80f, 0.9f, colors[(2+colorOffset)%6][0], colors[(2+colorOffset)%6][1], colors[(2+colorOffset)%6][2]);
    // Left upper branch tip
    drawFlower(base_x - spread * 0.6f, base_y + height * 0.95f, 0.8f, colors[(3+colorOffset)%6][0], colors[(3+colorOffset)%6][1], colors[(3+colorOffset)%6][2]);
    // Right upper branch tip
    drawFlower(base_x + spread * 0.6f, base_y + height * 1.00f, 0.8f, colors[(4+colorOffset)%6][0], colors[(4+colorOffset)%6][1], colors[(4+colorOffset)%6][2]);
    // Some flowers along branches
    drawFlower(base_x - spread * 0.2f, base_y + height * 0.70f, 0.7f, colors[(5+colorOffset)%6][0], colors[(5+colorOffset)%6][1], colors[(5+colorOffset)%6][2]);
    drawFlower(base_x + spread * 0.2f, base_y + height * 0.78f, 0.7f, colors[(1+colorOffset)%6][0], colors[(1+colorOffset)%6][1], colors[(1+colorOffset)%6][2]);
    drawFlower(base_x, base_y + height * 0.55f, 0.6f, colors[(2+colorOffset)%6][0], colors[(2+colorOffset)%6][1], colors[(2+colorOffset)%6][2]);
}

// --- Clouds ---
void drawCloud(float cx, float cy, float scale = 1.0f) {
    glColor3f(0.85f, 0.85f, 0.85f);
    for (int i = 0; i < 5; ++i) {
        float dx = (i - 2) * 0.07f * scale;
        float r = 0.09f * scale - 0.015f * abs(i - 2);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx + dx, cy);
        for (int j = 0; j <= 40; ++j) {
            float angle = 2.0f * 3.1415926f * j / 40;
            glVertex2f(cx + dx + cos(angle) * r, cy + sin(angle) * r * 0.7f);
        }
        glEnd();
    }
}

void drawClouds() {
    float baseY = 0.85f;
    drawCloud(-0.7f + cloudOffset, baseY, 1.0f);
    drawCloud(-0.2f + cloudOffset * 0.7f, baseY + 0.05f, 0.7f);
    drawCloud(0.4f + cloudOffset * 1.2f, baseY - 0.03f, 1.1f);
    drawCloud(0.85f + cloudOffset * 0.5f, baseY + 0.04f, 0.6f);
}

// --- Rain ---
float randFloat(float a, float b) {
    return a + (b - a) * (float(rand()) / float(RAND_MAX));
}

void initRain() {
    for (int i = 0; i < NUM_RAIN; ++i) {
        rainX[i] = randFloat(-1.2f, 1.2f);
        rainY[i] = randFloat(-1.0f, 1.2f);
    }
    rainInitialized = true;
}

void drawRain() {
    glColor3f(0.6f, 0.7f, 1.0f);
    glLineWidth(2.0f);
    for (int i = 0; i < NUM_RAIN; ++i) {
        glBegin(GL_LINES);
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i], rainY[i] - 0.08f);
        glEnd();
    }
    glLineWidth(1.0f);
}

void rainTimer(int value) {
    if (!isRain) return;
    for (int i = 0; i < NUM_RAIN; ++i) {
        rainY[i] -= 0.04f;
        if (rainY[i] < -1.0f) {
            rainY[i] = randFloat(0.9f, 1.2f);
            rainX[i] = randFloat(-1.2f, 1.2f);
        }
    }
    cloudOffset += cloudSpeed;
    if (cloudOffset > 2.0f) cloudOffset -= 2.0f;
    glutPostRedisplay();
    glutTimerFunc(30, rainTimer, 0);
}

// Draw the sun in the top-right
void drawSun() {
    if (!isRain) // Sun is not visible during rain
        drawCircle(1.0f - 0.18f, 0.95f - 0.18f, 0.11f, 1.0f, 0.95f, 0.2f);
}

// Draw the moon in the top-right
void drawMoon() {
    drawCircle(1.0f - 0.18f, 0.95f - 0.18f, 0.10f, 0.95f, 0.95f, 0.95f);
    // Crescent effect
    drawCircle(1.0f - 0.15f, 0.95f - 0.18f, 0.09f, 0.0f, 0.0f, 0.2f);
}

// Draw stars in the night sky
void drawStars() {
    if (!starsInitialized) {
        for (int i = 0; i < NUM_STARS; ++i) {
            // X: [-1.3, 1.3], Y: [0.1, 0.95]
            starX[i] = -1.3f + static_cast<float>(rand()) / RAND_MAX * (2.6f);
            starY[i] = 0.1f + static_cast<float>(rand()) / RAND_MAX * (0.85f);
        }
        starsInitialized = true;
    }
    for (int i = 0; i < NUM_STARS; ++i) {
        drawCircle(starX[i], starY[i], 0.012f, 1, 1, 1);
    }
}

// Draw the D building (background, behind everything)
void d_building() {
    glPushMatrix();
    glScalef(1.6f, 1.1f, 1.0f); // Make it larger
    glTranslatef(-0.045f, -0.05f, 0.0f); // Move it slightly left and down

    // Main Building
    glBegin(GL_QUADS);
        glColor3ub(200, 200, 200);
        glVertex2f(-0.52, -0.5);
        glVertex2f(0.4, -0.5);
        glVertex2f(0.4, 0.5);
        glVertex2f(-0.52, 0.5);
    glEnd();

    // Left Main Building
    glBegin(GL_QUADS);
        glColor3ub(180, 180, 180);
        glVertex2f(-0.8, -0.5);
        glVertex2f(-0.52, -0.5);
        glVertex2f(-0.52, 0.5);
        glVertex2f(-0.8, 0.47);
    glEnd();

    // Left Side
    glBegin(GL_POLYGON);
        glColor3ub(120, 180, 220);
        glVertex2f(-0.79, -0.48);
        glVertex2f(-0.52, -0.48);
        glVertex2f(-0.52, 0.18);
        glVertex2f(-0.6, 0.17);
        glVertex2f(-0.6, 0.47);
        glVertex2f(-0.79, 0.45);
    glEnd();

    // Middle Side1
    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(-0.35, 0.41);
        glVertex2f(0.34, 0.41);
        glVertex2f(0.34, 0.47);
        glVertex2f(-0.35, 0.47);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(-0.35, 0.33);
        glVertex2f(0.34, 0.33);
        glVertex2f(0.34, 0.39);
        glVertex2f(-0.35, 0.39);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(-0.35, 0.25);
        glVertex2f(0.34, 0.25);
        glVertex2f(0.34, 0.31);
        glVertex2f(-0.35, 0.31);
    glEnd();

    // Middle side2 (blue strips)
    glBegin(GL_QUADS);
        glColor3ub(120, 180, 220);
        glVertex2f(-0.48, 0.12);
        glVertex2f(0.34, 0.12);
        glVertex2f(0.34, 0.18);
        glVertex2f(-0.48, 0.18);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(120, 180, 220);
        glVertex2f(-0.48, 0.08);
        glVertex2f(0.34, 0.08);
        glVertex2f(0.34, 0.02);
        glVertex2f(-0.48, 0.02);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(120, 180, 220);
        glVertex2f(-0.48, -0.02);
        glVertex2f(0.34, -0.02);
        glVertex2f(0.34, -0.08);
        glVertex2f(-0.48, -0.08);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(120, 180, 220);
        glVertex2f(0.34, -0.12);
        glVertex2f(-0.48, -0.12);
        glVertex2f(-0.48, -0.18);
        glVertex2f(0.34, -0.18);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(120, 180, 220);
        glVertex2f(0.34, -0.22);
        glVertex2f(-0.48, -0.22);
        glVertex2f(-0.48, -0.28);
        glVertex2f(0.34, -0.28);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(120, 180, 220);
        glVertex2f(0.34, -0.32);
        glVertex2f(-0.48, -0.32);
        glVertex2f(-0.48, -0.38);
        glVertex2f(0.34, -0.38);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(120, 180, 220);
        glVertex2f(0.34, -0.42);
        glVertex2f(-0.48, -0.42);
        glVertex2f(-0.48, -0.48);
        glVertex2f(0.34, -0.48);
    glEnd();

    // Right Side
    glBegin(GL_QUADS);
        glColor3ub(170, 170, 170);
        glVertex2f(0.4, -0.47);
        glVertex2f(0.48, -0.47);
        glVertex2f(0.48, 0.45);
        glVertex2f(0.4, 0.45);
    glEnd();

    // Right Quads (windows)
    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, 0.35);
        glVertex2f(0.47, 0.35);
        glVertex2f(0.47, 0.43);
        glVertex2f(0.41, 0.43);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, 0.25);
        glVertex2f(0.47, 0.25);
        glVertex2f(0.47, 0.33);
        glVertex2f(0.41, 0.33);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, 0.15);
        glVertex2f(0.47, 0.15);
        glVertex2f(0.47, 0.23);
        glVertex2f(0.41, 0.23);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, 0.05);
        glVertex2f(0.47, 0.05);
        glVertex2f(0.47, 0.13);
        glVertex2f(0.41, 0.13);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, -0.05);
        glVertex2f(0.47,-0.05);
        glVertex2f(0.47, 0.03);
        glVertex2f(0.41, 0.03);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, -0.15);
        glVertex2f(0.47, -0.15);
        glVertex2f(0.47, -0.07);
        glVertex2f(0.41, -0.07);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, -0.25);
        glVertex2f(0.47, -0.25);
        glVertex2f(0.47, -0.17);
        glVertex2f(0.41, -0.17);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, -0.35);
        glVertex2f(0.47, -0.35);
        glVertex2f(0.47, -0.27);
        glVertex2f(0.41, -0.27);
    glEnd();

    glBegin(GL_QUADS);
        glColor3ub(90, 90, 100);
        glVertex2f(0.41, -0.45);
        glVertex2f(0.47, -0.45);
        glVertex2f(0.47, -0.37);
        glVertex2f(0.41, -0.37);
    glEnd();

    glPopMatrix();
}

// Draw the annex building
void annex() {
    glColor3f(0.96, 0.94, 0.85);
    glBegin(GL_POLYGON);
    glVertex2f(-1.34, -0.25);
    glVertex2f(-0.995, -0.27);
    glVertex2f(-0.99, -0.29);
    glVertex2f(-1.34, -0.27);
    glEnd();
    glColor3f(0.86, 0.45, 0.26);
    glBegin(GL_POLYGON);
    glVertex2f(-1.34, -0.27);
    glVertex2f(-1.02, -0.292);
    glVertex2f(-1.02, -0.31);
    glVertex2f(-1.34, -0.298);
    glEnd();
    glColor3f(0.80, 0.45, 0.26);
    glBegin(GL_POLYGON);
    glVertex2f(-1.36, -0.31);
    glVertex2f(-0.902, -0.322);
    glVertex2f(-0.994, -0.31);
    glVertex2f(-1.34, -0.298);
    glEnd();

    glColor3f(0.859, 0.859, 0.859);
    glBegin(GL_POLYGON);
    glVertex2f(-1.35, -0.31);
    glVertex2f(-0.902, -0.323);
    glVertex2f(-0.9, -0.4);
    glVertex2f(-1.34, -0.412);
    glEnd();
}

// Draw the field
void field() {
    glColor3f(0.2f,0.6f,0.17f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.344, -0.412);
    glVertex2f(-0.87, -0.383);
    glColor3f(0.54f,0.86f,0.26f);
    glVertex2f(0.185, -0.383);
    glColor3f(0.63f,0.96f,0.33f);
    glVertex2f(0.309, -0.398);
    glVertex2f(0.867, -0.472);
    glVertex2f(1.04, -0.475);
    glVertex2f(1.3395, -0.513);
    glVertex2f(1.341, -0.72);
    glVertex2f(-1.344, -0.72);
    glEnd();
}

// Draw the gallery
void gallery() {
    // ... unchanged, as in your code ...
    // (Omitted here for brevity, but keep your full gallery() code)
    // Paste your full gallery() code here.
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.378, -0.378);
    glVertex2f(0.356, -0.376);
    glVertex2f(0.355, -0.318);
    glVertex2f(0.645, -0.197);
    glVertex2f(0.764, -0.1898);
    glEnd();
    glColor3f(0.18, 0.824, 0.972);
    glBegin(GL_POLYGON);
    glVertex2f(0.378, -0.378);
    glVertex2f(0.378, -0.328);
    glVertex2f(0.7, -0.2);
    glVertex2f(0.764, -0.1898);
    glVertex2f(1.339, -0.074);
    glVertex2f(1.3399, -0.169);
    glVertex2f(0.948, -0.327);
    glVertex2f(0.95, -0.416);
    glVertex2f(0.867, -0.416);
    glEnd();

    glColor3f(0.859, 0.859, 0.859);
    glBegin(GL_POLYGON);
    glVertex2f(0.309, -0.398);
    glVertex2f(0.867, -0.472);
    glVertex2f(1.041, -0.475);
    glVertex2f(1.041, -0.414);
    glVertex2f(0.867, -0.416);
    glVertex2f(0.3096, -0.375);
    glEnd();

    glColor3f(0.0, 0.7, 0.86);
    glBegin(GL_POLYGON);
    glVertex2f(1.339, -0.1);
    glVertex2f(0.826, -0.369);
    glVertex2f(0.698, -0.364);
    glVertex2f(1.28, -0.0844);
    glVertex2f(1.339, -0.074);
    glEnd();

    glColor3f(0.42, 0.84, 0.96);
    glBegin(GL_POLYGON);
    glVertex2f(0.586, -0.364);
    glVertex2f(0.992, -0.165);
    glVertex2f(1.184, -0.115);
    glVertex2f(0.688, -0.364);
    glEnd();

    glColor3f(0.42, 0.84, 0.96);
    glBegin(GL_POLYGON);
    glVertex2f(0.406, -0.358);
    glVertex2f(0.78, -0.2);
    glVertex2f(0.9, -0.17);
    glVertex2f(0.451, -0.359);
    glEnd();

    glColor3f(0.0, 0.7, 0.86);
    glBegin(GL_POLYGON);
    glVertex2f(0.366, -0.351);
    glVertex2f(0.7, -0.2);
    glVertex2f(0.764, -0.1898);
    glVertex2f(0.4, -0.35);
    glEnd();

    glColor3f(0.96, 0.94, 0.85);
    glBegin(GL_POLYGON);
    glVertex2f(0.776, -0.131);glVertex2f(0.4, -0.117);
    glVertex2f(0.226, -0.099);glVertex2f(0.241, -0.057);
    glVertex2f(0.21, -0.056);glVertex2f(0.214, -0.025);
    glVertex2f(0.217, -0.0056);glVertex2f(0.2259, 0.0046);
    glVertex2f(0.2325, -0.001);glVertex2f(0.24, -0.02);
    glVertex2f(0.2496, 0.025); glVertex2f(0.259, 0.055);
    glVertex2f(0.2696, 0.0768); glVertex2f(0.282, 0.0903);
    glVertex2f(0.297, 0.085); glVertex2f(0.3095, 0.0602);
    glVertex2f(0.314, 0.085); glVertex2f(0.3226, 0.121);
    glVertex2f(0.337, 0.159); glVertex2f(0.352, 0.172);
    glVertex2f(0.365, 0.162); glVertex2f(0.374, 0.143);
    glVertex2f(0.481, 0.136); glVertex2f(0.5, 0.2);
    glVertex2f(0.514, 0.233); glVertex2f(0.537, 0.273);
    glVertex2f(0.594, 0.258);glVertex2f(1.338, 0.2196);
    glVertex2f(1.34, 0.0199);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.224, 0.005);glVertex2f(0.245, 0.002);
    glVertex2f(0.24, -0.02);glVertex2f(0.233, -0.001);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.282, 0.0903);glVertex2f(0.314, 0.086);
    glVertex2f(0.3095, -0.0602);glVertex2f(0.297, 0.085);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.35216, 0.17166);glVertex2f(0.48891, 0.16164);
    glVertex2f(0.48098, 0.13604);glVertex2f(0.37396, 0.14329);
    glVertex2f(0.36491, 0.16221);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.53684, 0.27255);glVertex2f(1.3407, 0.25123);
    glVertex2f(1.3407, 0.21957);glVertex2f(0.5942, 0.25805);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.776, -0.131);glVertex2f(0.4, -0.117);
    glVertex2f(0.226, -0.099);glVertex2f(0.241, -0.057);
    glVertex2f(0.21, -0.056);glVertex2f(0.214, -0.025);
    glVertex2f(0.217, -0.0056);glVertex2f(0.2259, 0.0046);
    glVertex2f(0.2325, -0.001);glVertex2f(0.24, -0.02);
    glVertex2f(0.2496, 0.025); glVertex2f(0.259, 0.055);
    glVertex2f(0.2696, 0.0768); glVertex2f(0.282, 0.0903);
    glVertex2f(0.297, 0.085); glVertex2f(0.3095, 0.0602);
    glVertex2f(0.314, 0.085); glVertex2f(0.3226, 0.121);
    glVertex2f(0.337, 0.159); glVertex2f(0.352, 0.172);
    glVertex2f(0.365, 0.162); glVertex2f(0.374, 0.143);
    glVertex2f(0.481, 0.136); glVertex2f(0.5, 0.2);
    glVertex2f(0.514, 0.233); glVertex2f(0.537, 0.273);
    glVertex2f(0.594, 0.258);glVertex2f(1.338, 0.2196);
    glVertex2f(1.34, 0.0199);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(0.224, 0.005);glVertex2f(0.245, 0.002);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(0.282, 0.0903);glVertex2f(0.314, 0.086);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(0.35216, 0.17166);glVertex2f(0.48891, 0.16164);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(0.53684, 0.27255);glVertex2f(1.3407, 0.25123);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0.24104, -0.05708);glVertex2f(0.846, -0.108);
    glVertex2f(0.24, -0.02);glVertex2f(0.95, -0.086);
    glVertex2f(0.30951, 0.06019);glVertex2f(1.25, 0);
    glVertex2f(0.48098, 0.13604);glVertex2f(1.33924, 0.09047);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.3096, -0.373);
    glVertex2f(0.3104, -0.3301);
    glVertex2f(0.868, -0.309);
    glVertex2f(0.867, -0.416);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.868, -0.309);
    glVertex2f(1.021, -0.307);
    glVertex2f(1.021, -0.416);
    glVertex2f(0.867, -0.416);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(0.312, -0.344);
    glVertex2f(0.868, -0.341);
    glEnd();

    glColor3f(0.96, 0.94, 0.85);
    glBegin(GL_POLYGON);
    glVertex2f(0.95, -0.416);
    glVertex2f(0.948, -0.327);
    glVertex2f(1.3399, -0.169);
    glVertex2f(1.339, -0.202);
    glVertex2f(0.979, -0.348);
    glVertex2f(0.9796, -0.415);
    glEnd();
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.903, -0.417);
    glVertex2f(0.904, -0.267);
    glVertex2f(1.341, -0.092);
    glVertex2f(1.34, -0.169);
    glVertex2f(0.948, -0.327);
    glVertex2f(0.95, -0.416);
    glEnd();

    glColor3f(0.471,0.235,0.165);
    glBegin(GL_POLYGON);
    glVertex2f(1.33929, -0.20179);glVertex2f(1.3411, -0.46258);
    glVertex2f(1.11015, -0.48282);
    glVertex2f(1.04299, -0.41048);glVertex2f(0.97841, -0.41183);
    glEnd();

    glColor3f(0.63f,0.96f,0.33f);
    glBegin(GL_TRIANGLES);
    glVertex2f(1.11015, -0.48282);glVertex2f(1.3411, -0.46258);
    glVertex2f(1.33953, -0.5134);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(1.021, -0.416);
    glVertex2f(1.021, -0.307);
    glVertex2f(1.161, -0.372);
    glVertex2f(1.161, -0.474);
    glVertex2f(1.161, -0.483);
    glVertex2f(1.14, -0.486);
    glVertex2f(1.041, -0.475);
    glVertex2f(1.041, -0.414);
    glEnd();

    glColor3f(0.859, 0.859, 0.89);
    glBegin(GL_POLYGON);
    glVertex2f(1.074, -0.439);
    glVertex2f(1.113, -0.439);
    glVertex2f(1.113, -0.458);
    glVertex2f(1.14, -0.46);
    glVertex2f(1.14, -0.486);
    glVertex2f(1.041, -0.475);
    glVertex2f(1.041, -0.414);
    glVertex2f(1.074, -0.415);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(1.036, 0.237);
    glVertex2f(1.036, -0.419);
    glVertex2f(1.0497, -0.419);
    glVertex2f(1.0497, 0.237);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.8, -0.312);
    glVertex2f(0.8, 0.12);
    glVertex2f(0.7863, 0.12);
    glVertex2f(0.7863, -0.312);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.674, -0.327);
    glVertex2f(0.674, 0.0337);
    glVertex2f(0.6603, 0.0337);
    glVertex2f(0.6603, -0.327);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.536, -0.332);
    glVertex2f(0.536, -0.043);
    glVertex2f(0.5223,  -0.043);
    glVertex2f(0.5223, -0.332);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.4, -0.325);
    glVertex2f(0.4, -0.117);
    glVertex2f(0.3863,  -0.117);
    glVertex2f(0.3863, -0.325);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.7, -0.193);
    glVertex2f(0.7, -0.124);
    glVertex2f(0.6863,  -0.124);
    glVertex2f(0.6863, -0.193);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.77595, -0.19);
    glVertex2f(0.77595, -0.13101);
    glVertex2f(0.76225,  -0.13101);
    glVertex2f(0.76225,  -0.19);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.95, -0.153);
    glVertex2f(0.95, -0.086);
    glVertex2f(0.9363, -0.086);
    glVertex2f(0.9363,  -0.154);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(1.25, -0.09487);
    glVertex2f(1.25, 0);
    glVertex2f(1.2363, 0);
    glVertex2f(1.2363,   -0.09487);
    glEnd();
}

// Draw the green fence in front of D building (between annex and gallery)
void fence() {
    float fence_left = -0.93f;   // Just after annex
    float fence_right = 0.82f;   // Just before gallery
    float fence_bottom = -0.38f;
    float fence_top = -0.25f;
    int num_bars = 28;

    glColor3ub(34, 139, 34); // Green color

    // Draw vertical bars
    for (int i = 0; i <= num_bars; ++i) {
        float x = fence_left + (fence_right - fence_left) * i / num_bars;
        glBegin(GL_QUADS);
        glVertex2f(x - 0.002f, fence_bottom);
        glVertex2f(x + 0.002f, fence_bottom);
        glVertex2f(x + 0.002f, fence_top);
        glVertex2f(x - 0.002f, fence_top);
        glEnd();
    }

    // Draw top and bottom horizontal bars
    glBegin(GL_QUADS);
    glVertex2f(fence_left, fence_top + 0.005f);
    glVertex2f(fence_right, fence_top + 0.005f);
    glVertex2f(fence_right, fence_top -0.005f);
    glVertex2f(fence_left, fence_top - 0.005f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(fence_left, fence_bottom +0.005f);
    glVertex2f(fence_right, fence_bottom + 0.005f);
    glVertex2f(fence_right, fence_bottom - 0.005f);
    glVertex2f(fence_left, fence_bottom - 0.005f);
    glEnd();

    // Draw flower trees if enabled
    if (showFlowerTrees) {
        int num_cells = num_bars;
        float tree_height = 0.22f;
        float tree_spread = (fence_right - fence_left) / num_cells * 0.7f;
        for (int i = 0; i < num_cells; ++i) {
            float left_x = fence_left + (fence_right - fence_left) * i / num_bars;
            float right_x = fence_left + (fence_right - fence_left) * (i+1) / num_bars;
            float center_x = (left_x + right_x) / 2.0f;
            drawFlowerTree(center_x, fence_bottom, tree_height, tree_spread, i % 6);
        }
    }
}

// Draw the football goalpost in the field, in front of fence, between bars 11 and 13
void goalpost() {
    float fence_left = -0.93f;
    float fence_right = 0.82f;
    float fence_bottom = -0.38f;
    int num_bars = 28;

    // Calculate x positions for bars 11 and 13
    float x11 = fence_left + (fence_right - fence_left) * 11 / num_bars;
    float x13 = fence_left + (fence_right - fence_left) * 13 / num_bars;

    float post_width = 0.012f; // thickness of post
    float post_height = 0.09f; // height above fence bottom
    float crossbar_bottom = fence_bottom + post_height;

    glColor3f(1.0f, 1.0f, 1.0f); // White color

    // Left vertical post
    glBegin(GL_QUADS);
    glVertex2f(x11 - post_width/2, fence_bottom);
    glVertex2f(x11 + post_width/2, fence_bottom);
    glVertex2f(x11 + post_width/2, crossbar_bottom);
    glVertex2f(x11 - post_width/2, crossbar_bottom);
    glEnd();

    // Right vertical post
    glBegin(GL_QUADS);
    glVertex2f(x13 - post_width/2, fence_bottom);
    glVertex2f(x13 + post_width/2, fence_bottom);
    glVertex2f(x13 + post_width/2, crossbar_bottom);
    glVertex2f(x13 - post_width/2, crossbar_bottom);
    glEnd();

    // Crossbar
    glBegin(GL_QUADS);
    glVertex2f(x11 - post_width/2, crossbar_bottom - post_width/2);
    glVertex2f(x13 + post_width/2, crossbar_bottom - post_width/2);
    glVertex2f(x13 + post_width/2, crossbar_bottom + post_width/2);
    glVertex2f(x11 - post_width/2, crossbar_bottom + post_width/2);
    glEnd();
}

// Draw the sky background and sun/moon/stars
void sky() {
    // Rainy sky is grayish, otherwise normal
    if (isRain) {
        glColor3f(0.65f, 0.7f, 0.75f);
    } else {
        glColor3f(0.67, 0.82, 0.93);
    }
    glBegin(GL_POLYGON);
    glVertex2f(-1.34493, 0.43746); glVertex2f(1.34132, 0.43749);
    glColor3f(0.88,0.88,0.88);
    glVertex2f(1.33953, -0.5134); glVertex2f(-1.34426, -0.41209);
    glEnd();

    if (isRain) {
        drawClouds();
    } else if (isDay) {
        drawSun();
    } else {
        drawMoon();
        drawStars();
    }
}



// --- Rename your display and keyboard functions for Scene 1 ---
void displayAnnexScene() {
    // Save current projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1); // Set to NDC

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (isRain) {
        glClearColor(0.65f, 0.7f, 0.75f, 1.0f); // Rainy sky
    } else if (isDay) {
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // Day sky blue
    } else {
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // Night navy blue
    }
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw sky (sun/moon/stars/clouds)
    sky();

    // Draw all scene elements
    d_building();
    annex();
    field();
    fence(); // flower trees are drawn inside fence() if enabled
    goalpost();
    gallery();

    // Draw rain overlay last so it appears over everything
    if (isRain) {
        drawRain();
    }

    glutSwapBuffers();
    glFlush();

    // ...existing code for drawing the Annex/Field/Gallery scene...

    // Restore previous matrices
    glPopMatrix(); // MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW); // Restore mode
    // Copy your entire display() code here

}

void keyboardAnnexScene(unsigned char key, int x, int y) {
    // Copy your entire keyboard() code here
    if (key == 'd' || key == 'D') {
        isDay = !isDay;
        glutPostRedisplay();
    }
    if (key == 'r' || key == 'R') {
        isRain = !isRain;
        if (isRain) {
            if (!rainInitialized) {
                srand((unsigned int)time(0));
                initRain();
            }
            glutTimerFunc(30, rainTimer, 0);
        }
        glutPostRedisplay();
    }
    if (key == 's' || key == 'S') {
        showFlowerTrees = !showFlowerTrees;
        glutPostRedisplay();
    }
}


// --- Placeholders for Dome/Road/Trees scene (Scene 2) ---
// Paste the dome/road/trees code here, and rename its display() to displayDomeScene()
// and its keyboard() to keyboardDomeScene()
// Window size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 1000;

// --- Sun Animation Variables ---
float sunAngle = 0.0f; // 0 = left horizon, PI = right horizon

// --- Scene Modes ---
enum SceneMode { MODE_DAY, MODE_NIGHT, MODE_RAINY };
SceneMode sceneMode = MODE_DAY;

// --- Star Animation Data ---
const int DAY_STAR_COUNT = 7;
const int NIGHT_STAR_COUNT = 38;
struct Star {
    float x, y, baseAlpha, twinkleSpeed, twinklePhase;
    float twinkle3D;
};
std::vector<Star> stars;
float moonGlowPhase = 0.0f;

// --- Rain Data ---
const int RAIN_DROP_COUNT = 320; // Much more rain
struct RainDrop {
    float x, y, len, speed;
};
std::vector<RainDrop> rainDrops;

// --- Cloud Data for Rainy Mode ---
struct Cloud {
    float x, y, scale, speed;
};
std::vector<Cloud> clouds;

// Utility: random float in [a, b]
float randf(float a, float b) {
    return a + (b - a) * (float(rand()) / RAND_MAX);
}

// --- Data structures for static tree/bush geometry ---
struct Leaf {
    float px, py, rx, ry, angle;
    float r, g, b;
    float swayPhase; // For wind effect
};
struct FlowerPetal {
    float px, py, rx, ry, angle;
    float r, g, b;
};
struct FlowerCenter {
    float px, py, rx, ry;
    float r, g, b;
};
struct TreeCluster {
    std::vector<Leaf> leaflets;
};
struct TreeFlower {
    std::vector<FlowerPetal> petals;
    FlowerCenter center;
};
struct PoincianaTreeData {
    float base_x, base_y, trunk_height, trunk_width, canopy_radius;
    bool whiteFlowers;
    std::vector<TreeCluster> clusters;
    std::vector<TreeFlower> flowers;
};
struct BushLeaf {
    float cx, cy, rx, ry, angle;
    float r, g, b;
    float swayPhase; // For wind effect
};
struct BushFlower {
    std::vector<FlowerPetal> petals;
    FlowerCenter center;
};
struct BushData {
    float base_x, base_y, bush_radius;
    std::vector<BushLeaf> leaves;
    std::vector<BushFlower> flowers;
};

// --- Store all static trees and bushes here ---
std::vector<PoincianaTreeData> allTrees;
std::vector<BushData> allBushes;

void drawEllipse(float cx, float cy, float rx, float ry, float angle = 0, int segments = 18) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0, 0, 1);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * M_PI * i / segments;
        glVertex2f(rx * cos(theta), ry * sin(theta));
    }
    glEnd();
    glPopMatrix();
}

// Draw a realistic sun with glow at (cx, cy)
void drawSun(float cx, float cy, float radius) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < 32; ++i) {
        float r = radius * (1.0f + 2.5f * i / 32.0f);
        float alpha = 0.18f * (1.0f - float(i) / 32.0f);
        glColor4f(1.0f, 0.93f, 0.45f, alpha);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int j = 0; j <= 36; ++j) {
            float theta = 2.0f * M_PI * j / 36.0f;
            glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
        }
        glEnd();
    }
    glColor3f(1.0f, 0.98f, 0.72f);
    drawEllipse(cx, cy, radius, radius, 0, 40);
    glDisable(GL_BLEND);
}

// Draw a filled ellipse at (cx, cy) with radii rx, ry and rotation angle (degrees)


// Draw an ellipse at (cx, cy) with radii rx, ry and segments (for clouds)
void drawEllipseCloud(float cx, float cy, float rx, float ry, int num_segments = 100) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = rx * cosf(theta);
        float y = ry * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Draw a cartoon cloud at (x, y) with the given scale
void drawCloudDome(float x, float y, float scale) {
    // Blue outline (drawn first, slightly larger)
    glColor3f(0.85f, 0.93f, 1.0f);
    drawEllipseCloud(x - 0.13f * scale, y,         0.23f * scale, 0.15f * scale);
    drawEllipseCloud(x + 0.09f * scale, y + 0.03f * scale, 0.19f * scale, 0.13f * scale);
    drawEllipseCloud(x - 0.01f * scale, y + 0.08f * scale, 0.17f * scale, 0.13f * scale);
    drawEllipseCloud(x + 0.19f * scale, y - 0.02f * scale, 0.15f * scale, 0.10f * scale);
    drawEllipseCloud(x + 0.05f * scale, y - 0.07f * scale, 0.13f * scale, 0.09f * scale);

    // White cloud body (drawn on top, slightly smaller)
    glColor3f(1.0f, 1.0f, 1.0f);
    drawEllipseCloud(x - 0.13f * scale, y,         0.20f * scale, 0.13f * scale);
    drawEllipseCloud(x + 0.09f * scale, y + 0.03f * scale, 0.16f * scale, 0.11f * scale);
    drawEllipseCloud(x - 0.01f * scale, y + 0.08f * scale, 0.14f * scale, 0.11f * scale);
    drawEllipseCloud(x + 0.19f * scale, y - 0.02f * scale, 0.12f * scale, 0.08f * scale);
    drawEllipseCloud(x + 0.05f * scale, y - 0.07f * scale, 0.10f * scale, 0.07f * scale);
}

// Draw glowing, twinkling stars (NIGHT, many, 3D twinkle effect)
void drawStars(float time) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (const auto& s : stars) {
        float twinkle = 0.7f + 0.3f * sin(time * s.twinkleSpeed + s.twinklePhase);
        float twinkle3D = 0.7f + 0.3f * cos(time * (s.twinkleSpeed * 1.3f) + s.twinklePhase * 1.7f);
        float alpha = s.baseAlpha * twinkle;
        float size = 1.1f + 0.9f * twinkle3D;
        glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.18f);
        drawEllipse(s.x, s.y, size * 2.2f, size * 2.2f, 0, 10);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);
        drawEllipse(s.x, s.y, size, size, 0, 8);
    }
    glDisable(GL_BLEND);
}

// Draw crescent moon with glow (NIGHT, only crescent white, no blue leftover, animated glow)
void drawCrescentMoon(float cx, float cy, float r, float glowPhase) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    float glowStrength = 0.7f + 0.3f * sin(glowPhase * 0.8f);
    for (int i = 0; i < 18; ++i) {
        float rr = r * (1.0f + 1.5f * i / 18.0f);
        float alpha = 0.18f * (1.0f - float(i) / 18.0f) * glowStrength;
        glColor4f(1.0f, 1.0f, 0.92f, alpha);
        drawEllipse(cx, cy, rr, rr, 0, 32);
    }
    glDisable(GL_BLEND);

    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    drawEllipse(cx, cy, r, r, 0, 40);

    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
    drawEllipse(cx + r * 0.45f, cy - r * 0.08f, r * 0.92f, r * 0.92f, 0, 40);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glColor3f(1.0f, 1.0f, 0.92f);
    drawEllipse(cx, cy, r, r, 0, 40);

    glDisable(GL_STENCIL_TEST);
}

// Draw a simple grass patch at the bottom
void drawGround() {
    glBegin(GL_POLYGON);
    glColor3f(0.32f, 0.72f, 0.22f);
    glVertex2f(0, WINDOW_HEIGHT - 120);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT - 120);
    glColor3f(0.22f, 0.62f, 0.18f);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    for (int i = 0; i < 80; ++i) {
        float x = randf(0, WINDOW_WIDTH);
        float y = WINDOW_HEIGHT - 120 + randf(0, 18);
        glColor3f(0.18f, 0.55f + 0.25f * randf(0, 1), 0.13f);
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + randf(-3, 3), y - 18 - randf(0, 10));
        glEnd();
    }
}

// --- Dome, base, entry, and road drawing functions ---
const float DOME_CENTER_X = 400.0f;
const float DOME_CENTER_Y = 350.0f;
const float DOME_RADIUS_X = 240.0f;
const float DOME_RADIUS_Y = 220.0f;
const int DOME_LONGITUDE_LINES = 18;
const int DOME_LATITUDE_LINES = 13;
const float SQUARE_HEIGHT = 110.0f;
const float ENTRY_COL_WIDTH = 16.0f;
const float ENTRY_COL_HEIGHT = 120.0f;
const float ENTRY_COL_SPACING = 36.0f;
const float ENTRY_COL_MARGIN = 28.0f;
const float ENTRY_ROOF_HEIGHT = 18.0f;
const float ENTRY_ROOF_OVERHANG = 38.0f;

void fillDomePanelsStriped() {
    for (int j = 0; j < DOME_LATITUDE_LINES - 2; ++j) {
        float phi1 = (M_PI * j) / DOME_LATITUDE_LINES;
        float phi2 = (M_PI * (j + 1)) / DOME_LATITUDE_LINES;
        if (j % 2 == 0)
            glColor3f(0.75f, 0.87f, 0.97f); // Light blue
        else
            glColor3f(0.82f, 0.82f, 0.82f); // Light grey
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= DOME_LONGITUDE_LINES; ++i) {
            float theta = (2.0f * M_PI * i) / DOME_LONGITUDE_LINES;
            float x1 = DOME_CENTER_X + DOME_RADIUS_X * sin(phi1) * cos(theta);
            float y1 = DOME_CENTER_Y - DOME_RADIUS_Y * cos(phi1);
            float x2 = DOME_CENTER_X + DOME_RADIUS_X * sin(phi2) * cos(theta);
            float y2 = DOME_CENTER_Y - DOME_RADIUS_Y * cos(phi2);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
        glEnd();
    }
    float phi_flat = (M_PI * (DOME_LATITUDE_LINES - 2)) / DOME_LATITUDE_LINES;
    float y_flat = DOME_CENTER_Y - DOME_RADIUS_Y * cos(phi_flat);
    float x_left = DOME_CENTER_X + DOME_RADIUS_X * sin(phi_flat) * cos(M_PI);
    float x_right = DOME_CENTER_X + DOME_RADIUS_X * sin(phi_flat) * cos(0);
    if ((DOME_LATITUDE_LINES - 2) % 2 == 0)
        glColor3f(0.75f, 0.87f, 0.97f);
    else
        glColor3f(0.82f, 0.82f, 0.82f);
    glBegin(GL_POLYGON);
    glVertex2f(x_left, y_flat);
    glVertex2f(x_right, y_flat);
    glVertex2f(x_right, y_flat + 1.0f);
    glVertex2f(x_left, y_flat + 1.0f);
    glEnd();
}

void fillSquareBase(float left_x, float right_x, float top_y, float& base_bottom) {
    float width = right_x - left_x;
    float height = SQUARE_HEIGHT;
    float bottom_y = top_y + height;
    base_bottom = bottom_y;

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(left_x, top_y);
    glVertex2f(right_x, top_y);
    glVertex2f(right_x, bottom_y);
    glVertex2f(left_x, bottom_y);
    glEnd();

    float door_width = width * 0.28f;
    float door_height = height * 0.78f;
    float door_left = left_x + (width - door_width) / 2.0f;
    float door_right = door_left + door_width;
    float door_top = bottom_y - door_height;
    float door_bottom = bottom_y;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(door_left, door_top);
    glVertex2f(door_right, door_top);
    glVertex2f(door_right, door_bottom);
    glVertex2f(door_left, door_bottom);
    glEnd();

    float win_width = width * 0.22f;
    float win_height = height * 0.48f;
    float win_top = bottom_y - door_height + (door_height - win_height) / 2.0f;
    float win_bottom = win_top + win_height;

    float win1_left = left_x + width * 0.08f;
    float win1_right = win1_left + win_width;
    glColor3f(0.75f, 0.87f, 0.97f);
    glBegin(GL_POLYGON);
    glVertex2f(win1_left, win_top);
    glVertex2f(win1_right, win_top);
    glVertex2f(win1_right, win_bottom);
    glVertex2f(win1_left, win_bottom);
    glEnd();

    float win2_right = right_x - width * 0.08f;
    float win2_left = win2_right - win_width;
    glColor3f(0.75f, 0.87f, 0.97f);
    glBegin(GL_POLYGON);
    glVertex2f(win2_left, win_top);
    glVertex2f(win2_right, win_top);
    glVertex2f(win2_right, win_bottom);
    glVertex2f(win2_left, win_bottom);
    glEnd();
}

void drawEntryStructure(float left_x, float right_x, float top_y) {
    float width = right_x - left_x;
    float col_height = ENTRY_COL_HEIGHT;
    float col_width = ENTRY_COL_WIDTH;
    float col_y_top = top_y + SQUARE_HEIGHT * 0.18f;
    float col_y_bottom = col_y_top + col_height;

    float col1_x_left = left_x - ENTRY_ROOF_OVERHANG + ENTRY_COL_MARGIN;
    float col1_x_right = col1_x_left + col_width;
    float col2_x_left = col1_x_right + ENTRY_COL_SPACING;
    float col2_x_right = col2_x_left + col_width;
    float col4_x_right = right_x + ENTRY_ROOF_OVERHANG - ENTRY_COL_MARGIN;
    float col4_x_left = col4_x_right - col_width;
    float col3_x_right = col4_x_left - ENTRY_COL_SPACING;
    float col3_x_left = col3_x_right - col_width;

    glColor3f(0.85f, 0.07f, 0.07f);
    glBegin(GL_POLYGON); glVertex2f(col1_x_left, col_y_top); glVertex2f(col1_x_right, col_y_top); glVertex2f(col1_x_right, col_y_bottom); glVertex2f(col1_x_left, col_y_bottom); glEnd();
    glBegin(GL_POLYGON); glVertex2f(col2_x_left, col_y_top); glVertex2f(col2_x_right, col_y_top); glVertex2f(col2_x_right, col_y_bottom); glVertex2f(col2_x_left, col_y_bottom); glEnd();
    glBegin(GL_POLYGON); glVertex2f(col3_x_left, col_y_top); glVertex2f(col3_x_right, col_y_top); glVertex2f(col3_x_right, col_y_bottom); glVertex2f(col3_x_left, col_y_bottom); glEnd();
    glBegin(GL_POLYGON); glVertex2f(col4_x_left, col_y_top); glVertex2f(col4_x_right, col_y_top); glVertex2f(col4_x_right, col_y_bottom); glVertex2f(col4_x_left, col_y_bottom); glEnd();

    float roof_y = col_y_top;
    float roof_height = ENTRY_ROOF_HEIGHT;
    float roof_left = col1_x_left - ENTRY_COL_WIDTH * 0.3f;
    float roof_right = col4_x_right + ENTRY_COL_WIDTH * 0.3f;
    glColor3f(0.85f, 0.07f, 0.07f);
    glBegin(GL_POLYGON);
    glVertex2f(roof_left, roof_y - roof_height);
    glVertex2f(roof_right, roof_y - roof_height);
    glVertex2f(roof_right, roof_y);
    glVertex2f(roof_left, roof_y);
    glEnd();

    int stripes = 5;
    float stripe_gap = roof_height / (stripes + 1);
    glColor3f(0.6f, 0.0f, 0.0f);
    for (int i = 1; i <= stripes; ++i) {
        float y = roof_y - i * stripe_gap;
        glBegin(GL_LINES);
        glVertex2f(roof_left, y);
        glVertex2f(roof_right, y);
        glEnd();
    }
}

void draw3DRoad(float base_bottom) {
    float road_top = base_bottom;
    float road_bottom = WINDOW_HEIGHT;
    float road_top_width = 340.0f;
    float road_bottom_width = WINDOW_WIDTH;
    float road_top_center = WINDOW_WIDTH / 2.0f;

    glBegin(GL_QUADS);
    glColor3f(0.32f, 0.32f, 0.32f);
    glVertex2f(road_top_center - road_top_width / 2.0f, road_top);
    glVertex2f(road_top_center + road_top_width / 2.0f, road_top);
    glColor3f(0.55f, 0.55f, 0.55f);
    glVertex2f(WINDOW_WIDTH, road_bottom);
    glVertex2f(0, road_bottom);
    glEnd();

    float sidewalk_width_top = 24.0f;
    float sidewalk_width_bottom = 32.0f;
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(road_top_center - road_top_width / 2.0f - sidewalk_width_top, road_top);
    glVertex2f(road_top_center - road_top_width / 2.0f, road_top);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(0 + sidewalk_width_bottom, road_bottom);
    glVertex2f(0, road_bottom);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(road_top_center + road_top_width / 2.0f, road_top);
    glVertex2f(road_top_center + road_top_width / 2.0f + sidewalk_width_top, road_top);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(WINDOW_WIDTH, road_bottom);
    glVertex2f(WINDOW_WIDTH - sidewalk_width_bottom, road_bottom);
    glEnd();

    int stripes = 9;
    for (int i = 0; i < stripes; ++i) {
        float t1 = (float)i / stripes;
        float t2 = (float)(i + 0.5f) / stripes;
        float y1 = road_top + t1 * (road_bottom - road_top);
        float y2 = road_top + t2 * (road_bottom - road_top);

        float w1 = road_top_width + t1 * (road_bottom_width - road_top_width);
        float w2 = road_top_width + t2 * (road_bottom_width - road_top_width);

        float x1_left = road_top_center - w1 / 12.0f;
        float x1_right = road_top_center + w1 / 12.0f;
        float x2_left = road_top_center - w2 / 12.0f;
        float x2_right = road_top_center + w2 / 12.0f;

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(x1_left, y1);
        glVertex2f(x1_right, y1);
        glVertex2f(x2_right, y2);
        glVertex2f(x2_left, y2);
        glEnd();
    }
}

void drawDomeWithFlatBottomTwoLines(float& flat_left_x, float& flat_right_x, float& flat_y) {
    glColor3f(0.18f, 0.18f, 0.18f);
    for (int i = 0; i < DOME_LONGITUDE_LINES; ++i) {
        float theta = (2.0f * M_PI * i) / DOME_LONGITUDE_LINES;
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < DOME_LATITUDE_LINES - 1; ++j) {
            float phi = (M_PI * j) / DOME_LATITUDE_LINES;
            float x = DOME_CENTER_X + DOME_RADIUS_X * sin(phi) * cos(theta);
            float y = DOME_CENTER_Y - DOME_RADIUS_Y * cos(phi);
            glVertex2f(x, y);
        }
        float phi_flat = (M_PI * (DOME_LATITUDE_LINES - 2)) / DOME_LATITUDE_LINES;
        float x_flat = DOME_CENTER_X + DOME_RADIUS_X * sin(phi_flat) * cos(theta);
        float y_flat = DOME_CENTER_Y - DOME_RADIUS_Y * cos(phi_flat);
        glVertex2f(x_flat, y_flat);
        glEnd();
    }
    for (int j = 1; j < DOME_LATITUDE_LINES - 2; ++j) {
        float phi = (M_PI * j) / DOME_LATITUDE_LINES;
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= DOME_LONGITUDE_LINES; ++i) {
            float theta = (2.0f * M_PI * i) / DOME_LONGITUDE_LINES;
            float x = DOME_CENTER_X + DOME_RADIUS_X * sin(phi) * cos(theta);
            float y = DOME_CENTER_Y - DOME_RADIUS_Y * cos(phi);
            glVertex2f(x, y);
        }
        glEnd();
    }
    float phi_flat = (M_PI * (DOME_LATITUDE_LINES - 2)) / DOME_LATITUDE_LINES;
    flat_left_x = DOME_CENTER_X + DOME_RADIUS_X * sin(phi_flat) * cos(M_PI);
    flat_y = DOME_CENTER_Y - DOME_RADIUS_Y * cos(phi_flat);
    flat_right_x = DOME_CENTER_X + DOME_RADIUS_X * sin(phi_flat) * cos(0);
    glBegin(GL_LINES);
    glVertex2f(flat_left_x, flat_y);
    glVertex2f(flat_right_x, flat_y);
    glEnd();
}

void drawSquareBaseLines(float left_x, float right_x, float top_y) {
    float height = SQUARE_HEIGHT;
    float bottom_y = top_y + height;
    glColor3f(0.18f, 0.18f, 0.18f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(left_x, top_y);
    glVertex2f(right_x, top_y);
    glVertex2f(right_x, bottom_y);
    glVertex2f(left_x, bottom_y);
    glEnd();
}

// --- Royal Poinciana Bushy Tree Drawing Function (STATIC/WIND) ---
void drawPoincianaBushTreeStatic(const PoincianaTreeData& tree, float windPhase = 0.0f, bool wind = false) {
    glColor3f(0.38f, 0.25f, 0.13f);
    glBegin(GL_POLYGON);
    glVertex2f(tree.base_x - tree.trunk_width / 2.0f, tree.base_y);
    glVertex2f(tree.base_x + tree.trunk_width / 2.0f, tree.base_y);
    glVertex2f(tree.base_x + tree.trunk_width * 0.35f, tree.base_y - tree.trunk_height);
    glVertex2f(tree.base_x - tree.trunk_width * 0.35f, tree.base_y - tree.trunk_height);
    glEnd();

    for (const auto& cluster : tree.clusters) {
        for (const auto& leaf : cluster.leaflets) {
            float sway = 0.0f;
            if (wind) sway = 12.0f * sin(windPhase + leaf.swayPhase);
            glColor3f(leaf.r, leaf.g, leaf.b);
            drawEllipse(leaf.px + sway, leaf.py, leaf.rx, leaf.ry, leaf.angle + sway * 0.7f, 10);
        }
    }
    for (const auto& flower : tree.flowers) {
        for (const auto& petal : flower.petals) {
            glColor3f(petal.r, petal.g, petal.b);
            drawEllipse(petal.px, petal.py, petal.rx, petal.ry, petal.angle, 8);
        }
        glColor3f(flower.center.r, flower.center.g, flower.center.b);
        drawEllipse(flower.center.px, flower.center.py, flower.center.rx, flower.center.ry, 0, 8);
    }
}

// --- Realistic upright bush with leaf shapes and white flowers (STATIC/WIND) ---
void drawRealBushStatic(const BushData& bush, float windPhase = 0.0f, bool wind = false) {
    for (const auto& leaf : bush.leaves) {
        float sway = 0.0f;
        if (wind) sway = 8.0f * sin(windPhase + leaf.swayPhase);
        glColor3f(leaf.r, leaf.g, leaf.b);
        drawEllipse(leaf.cx + sway, leaf.cy, leaf.rx, leaf.ry, leaf.angle + sway * 0.7f, 20);
        glColor3f(0.13f, 0.38f, 0.13f);
        glBegin(GL_LINES);
        glVertex2f(leaf.cx + sway, leaf.cy);
        glVertex2f(leaf.cx + sway, leaf.cy - leaf.ry * 0.9f);
        glEnd();
    }
    for (const auto& flower : bush.flowers) {
        for (const auto& petal : flower.petals) {
            glColor3f(petal.r, petal.g, petal.b);
            drawEllipse(petal.px, petal.py, petal.rx, petal.ry, petal.angle, 10);
        }
        glColor3f(flower.center.r, flower.center.g, flower.center.b);
        drawEllipse(flower.center.px, flower.center.py, flower.center.rx, flower.center.ry, 0, 8);
    }
    glColor3f(0.13f, 0.28f, 0.13f);
    drawEllipse(bush.base_x, bush.base_y + bush.bush_radius * 0.13f, bush.bush_radius * 0.48f, bush.bush_radius * 0.13f, 0, 18);

    int nTufts = 4;
    for (int t = 0; t < nTufts; ++t) {
        float gx = bush.base_x + bush.bush_radius * (-0.48f + 0.32f * t);
        float gy = bush.base_y + bush.bush_radius * (0.09f + 0.01f * t);
        int nBlades = 2;
        for (int b = 0; b < nBlades; ++b) {
            float bladeLen = bush.bush_radius * (0.32f + 0.08f * b);
            float angle = -10 + 20 * b;
            glColor3f(0.13f + 0.04f * b, 0.55f + 0.06f * b, 0.13f);
            glBegin(GL_LINES);
            glVertex2f(gx, gy);
            glVertex2f(gx + bladeLen * sin(angle * M_PI / 180.0f), gy + bladeLen * -cos(angle * M_PI / 180.0f));
            glEnd();
        }
    }
}

// --- Generate all static tree and bush data at startup ---
void generateAllStaticTreesAndBushes(
    float flat_left_x, float flat_right_x, float flat_y, float base_bottom,
    float road_top_center, float road_top_width, float road_top, float road_bottom,
    float bush_x_left, float bush_x_right, float bush_y,
    float tree_trunk_height, float tree_trunk_width, float tree_canopy_radius,
    int nTreesPerSide, float offset, float front_tree_x_left, float front_tree_x_right, float front_tree_y
) {
    allTrees.clear();
    allBushes.clear();

    // --- Bushes ---
    for (int bushIdx = 0; bushIdx < 2; ++bushIdx) {
        BushData bush;
        bush.base_x = (bushIdx == 0) ? bush_x_left : bush_x_right;
        bush.base_y = bush_y;
        bush.bush_radius = 48.0f;
        int nLeaves = 66;
        for (int i = 0; i < nLeaves; ++i) {
            float angle = -80 + 160.0f * i / nLeaves;
            float r = randf(bush.bush_radius * 0.18f, bush.bush_radius * 1.18f);
            float spread = (1.0f - r / (bush.bush_radius * 1.22f));
            float cx = bush.base_x + randf(-bush.bush_radius * 0.42f, bush.bush_radius * 0.42f) * spread;
            float cy = bush.base_y - r;
            float leaf_angle = randf(-28, 28);
            float rx = randf(bush.bush_radius * 0.17f, bush.bush_radius * 0.25f);
            float ry = randf(bush.bush_radius * 0.38f, bush.bush_radius * 0.62f);
            float swayPhase = randf(0, 6.28f);
            bush.leaves.push_back({cx, cy, rx, ry, leaf_angle, 0.19f + randf(0, 0.11f), 0.62f + randf(0, 0.18f), 0.19f + randf(0, 0.11f), swayPhase});
        }
        int nFlowers = 15;
        for (int i = 0; i < nFlowers; ++i) {
            float theta = randf(-80, 80);
            float r = randf(bush.bush_radius * 0.22f, bush.bush_radius * 1.02f);
            float fx = bush.base_x + randf(-bush.bush_radius * 0.32f, bush.bush_radius * 0.32f) * (1.0f - r / (bush.bush_radius * 1.22f));
            float fy = bush.base_y - r - randf(0, bush.bush_radius * 0.13f);
            float base_angle = randf(0, 360);
            BushFlower flower;
            for (int p = 0; p < 5; ++p) {
                float petal_angle = base_angle + p * 72.0f;
                float petal_len = bush.bush_radius * ((p == 0) ? 0.23f : 0.17f);
                flower.petals.push_back({
                    fx + petal_len * cos(petal_angle * M_PI / 180.0f),
                    fy + petal_len * sin(petal_angle * M_PI / 180.0f),
                    bush.bush_radius * ((p == 0) ? 0.09f : 0.07f),
                    bush.bush_radius * ((p == 0) ? 0.15f : 0.09f),
                    petal_angle,
                    1.0f, 1.0f, 1.0f
                });
            }
            flower.center = {fx, fy, bush.bush_radius * 0.045f, bush.bush_radius * 0.06f, 0.98f, 0.98f, 0.7f};
            bush.flowers.push_back(flower);
        }
        allBushes.push_back(bush);
    }

    // --- Trees along both sides of the road ---
    for (int side = 0; side < 2; ++side) {
        for (int i = 0; i < nTreesPerSide; ++i) {
            float t = float(i) / (nTreesPerSide - 1);
            float left_x_top = road_top_center - road_top_width / 2.0f - 24.0f;
            float left_x_bottom = 0 + 32.0f;
            float left_x = left_x_top + t * (left_x_bottom - left_x_top);
            float left_y = road_top + t * (road_bottom - road_top);

            float right_x_top = road_top_center + road_top_width / 2.0f + 24.0f;
            float right_x_bottom = WINDOW_WIDTH - 32.0f;
            float right_x = right_x_top + t * (right_x_bottom - right_x_top);
            float right_y = road_top + t * (road_bottom - road_top);

            float base_x = (side == 0) ? left_x : right_x;
            float base_y = (side == 0) ? left_y : right_y;
            bool whiteFlowers = (side == 0) ? (i % 2 == 0) : (i % 2 == 1);

            PoincianaTreeData tree;
            tree.base_x = base_x;
            tree.base_y = base_y;
            tree.trunk_height = tree_trunk_height;
            tree.trunk_width = tree_trunk_width;
            tree.canopy_radius = tree_canopy_radius;
            tree.whiteFlowers = whiteFlowers;
            int nClusters = 80;
            float canopy_center_y = base_y - tree_trunk_height - tree.canopy_radius * 0.25f;
            for (int c = 0; c < nClusters; ++c) {
                float theta = randf(-110, 110);
                float r = randf(tree_canopy_radius * 0.45f, tree_canopy_radius);
                float cx = base_x + r * cos(theta * M_PI / 180.0f) * randf(0.85f, 1.0f);
                float cy = canopy_center_y + r * sin(theta * M_PI / 180.0f) * randf(0.5f, 1.0f);
                int nLeaflets = 10;
                TreeCluster cluster;
                for (int k = 0; k < nLeaflets; ++k) {
                    float angle = randf(0, 360);
                    float rx = randf(7, 11);
                    float ry = randf(2.5f, 4.5f);
                    float px = cx + randf(-8, 8);
                    float py = cy + randf(-8, 8);
                    float swayPhase = randf(0, 6.28f);
                    cluster.leaflets.push_back({px, py, rx, ry, angle, 0.22f + randf(0, 0.08f), 0.62f + randf(0, 0.15f), 0.18f + randf(0, 0.08f), swayPhase});
                }
                tree.clusters.push_back(cluster);
            }
            int nFlowers = 38;
            for (int f = 0; f < nFlowers; ++f) {
                float theta = randf(-110, 110);
                float r = randf(tree.canopy_radius * 0.45f, tree.canopy_radius * 0.98f);
                float fx = base_x + r * cos(theta * M_PI / 180.0f) * randf(0.85f, 1.0f);
                float fy = canopy_center_y + r * sin(theta * M_PI / 180.0f) * randf(0.5f, 1.0f);
                float base_angle = randf(0, 360);
                TreeFlower flower;
                for (int p = 0; p < 5; ++p) {
                    float petal_angle = base_angle + p * 72.0f;
                    float petal_len = (p == 0) ? 8.0f : 6.0f;
                    float rF = whiteFlowers ? 1.0f : 1.0f;
                    float gF = whiteFlowers ? 1.0f : randf(0.25f, 0.45f);
                    float bF = whiteFlowers ? 1.0f : 0.1f + randf(0, 0.1f);
                    flower.petals.push_back({
                        fx + petal_len * cos(petal_angle * M_PI / 180.0f),
                        fy + petal_len * sin(petal_angle * M_PI / 180.0f),
                        (p == 0) ? 3.5f : 2.5f, (p == 0) ? 5.0f : 3.5f,
                        petal_angle,
                        rF, gF, bF
                    });
                }
                float cr = whiteFlowers ? 0.95f : 1.0f;
                float cg = whiteFlowers ? 0.95f : 0.7f;
                float cb = whiteFlowers ? 0.7f : 0.2f;
                flower.center = {fx, fy, 1.5f, 2.0f, cr, cg, cb};
                tree.flowers.push_back(flower);
            }
            allTrees.push_back(tree);
        }
    }

    // --- Trees in front of the dome ---
    for (int i = 0; i < 2; ++i) {
        float base_x = (i == 0) ? front_tree_x_left : front_tree_x_right;
        float base_y = front_tree_y;
        bool whiteFlowers = (i == 1);
        PoincianaTreeData tree;
        tree.base_x = base_x;
        tree.base_y = base_y;
        tree.trunk_height = tree_trunk_height;
        tree.trunk_width = tree_trunk_width;
        tree.canopy_radius = tree_canopy_radius;
        tree.whiteFlowers = whiteFlowers;
        int nClusters = 80;
        float canopy_center_y = base_y - tree_trunk_height - tree.canopy_radius * 0.25f;
        for (int c = 0; c < nClusters; ++c) {
            float theta = randf(-110, 110);
            float r = randf(tree_canopy_radius * 0.45f, tree_canopy_radius);
            float cx = base_x + r * cos(theta * M_PI / 180.0f) * randf(0.85f, 1.0f);
            float cy = canopy_center_y + r * sin(theta * M_PI / 180.0f) * randf(0.5f, 1.0f);
            int nLeaflets = 10;
            TreeCluster cluster;
            for (int k = 0; k < nLeaflets; ++k) {
                float angle = randf(0, 360);
                float rx = randf(7, 11);
                float ry = randf(2.5f, 4.5f);
                float px = cx + randf(-8, 8);
                float py = cy + randf(-8, 8);
                float swayPhase = randf(0, 6.28f);
                cluster.leaflets.push_back({px, py, rx, ry, angle, 0.22f + randf(0, 0.08f), 0.62f + randf(0, 0.15f), 0.18f + randf(0, 0.08f), swayPhase});
            }
            tree.clusters.push_back(cluster);
        }
        int nFlowers = 38;
        for (int f = 0; f < nFlowers; ++f) {
            float theta = randf(-110, 110);
            float r = randf(tree.canopy_radius * 0.45f, tree.canopy_radius * 0.98f);
            float fx = base_x + r * cos(theta * M_PI / 180.0f) * randf(0.85f, 1.0f);
            float fy = canopy_center_y + r * sin(theta * M_PI / 180.0f) * randf(0.5f, 1.0f);
            float base_angle = randf(0, 360);
            TreeFlower flower;
            for (int p = 0; p < 5; ++p) {
                float petal_angle = base_angle + p * 72.0f;
                float petal_len = (p == 0) ? 8.0f : 6.0f;
                float rF = whiteFlowers ? 1.0f : 1.0f;
                float gF = whiteFlowers ? 1.0f : randf(0.25f, 0.45f);
                float bF = whiteFlowers ? 1.0f : 0.1f + randf(0, 0.1f);
                flower.petals.push_back({
                    fx + petal_len * cos(petal_angle * M_PI / 180.0f),
                    fy + petal_len * sin(petal_angle * M_PI / 180.0f),
                    (p == 0) ? 3.5f : 2.5f, (p == 0) ? 5.0f : 3.5f,
                    petal_angle,
                    rF, gF, bF
                });
            }
            float cr = whiteFlowers ? 0.95f : 1.0f;
            float cg = whiteFlowers ? 0.95f : 0.7f;
            float cb = whiteFlowers ? 0.7f : 0.2f;
            flower.center = {fx, fy, 1.5f, 2.0f, cr, cg, cb};
            tree.flowers.push_back(flower);
        }
        allTrees.push_back(tree);
    }
}

// Draw rain drops (RAINY)
void drawRainDome() {
    glColor3f(0.6f, 0.8f, 1.0f);
    glBegin(GL_LINES);
    for (const auto& drop : rainDrops) {
        glVertex2f(drop.x, drop.y);
        glVertex2f(drop.x, drop.y + drop.len);
    }
    glEnd();
}

// Draw all clouds (Rainy mode only)
void drawCloudsDome() {
    for (const auto& c : clouds) {
        drawCloud(c.x, c.y, c.scale);
    }
}

// Draw a vertical gradient sky background (DAY)
void drawSkyDay() {
    glBegin(GL_QUADS);
    glColor3f(0.18f, 0.38f, 0.68f); // Top (deep blue)
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glColor3f(0.65f, 0.85f, 0.98f); // Bottom (light blue)
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
}

// Draw a vertical gradient sky background (NIGHT)
void drawSkyNight() {
    glBegin(GL_QUADS);
    glColor3f(0.06f, 0.10f, 0.22f); // Top (very dark blue)
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glColor3f(0.13f, 0.18f, 0.32f); // Bottom (less dark blue)
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
}

// Draw a vertical gradient sky background (RAINY)
void drawSkyRainy() {
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.44f, 0.52f); // Top
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glColor3f(0.55f, 0.57f, 0.65f); // Bottom
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
}

// --- GLUT idle callback for sun/moon/star animation ---
void idle() {
    static float windPhase = 0.0f;
    if (sceneMode == MODE_DAY) {
        sunAngle += 0.0007f;
        if (sunAngle > M_PI) sunAngle = 0.0f;
    }
    moonGlowPhase += 0.01f;
    windPhase += (sceneMode == MODE_RAINY) ? 0.04f : 0.0f;

    // Rain update
    if (sceneMode == MODE_RAINY) {
        // Move clouds
        for (auto& c : clouds) {
            c.x += c.speed;
            if (c.x - c.scale * 0.25f > WINDOW_WIDTH) {
                c.x = -c.scale * 0.25f;
            }
        }
        for (auto& drop : rainDrops) {
            drop.y += drop.speed;
            if (drop.y > WINDOW_HEIGHT) {
                drop.x = randf(0, WINDOW_WIDTH);
                drop.y = randf(-60, 0);
                drop.len = randf(10, 15);
                drop.speed = randf(2.0f, 5.0f);
            }
        }
    }
    glutPostRedisplay();
}
// --- Initialize stars for night sky (many, random, 3D twinkle) ---
void initStars() {
    stars.clear();
    int count = (sceneMode == MODE_NIGHT) ? NIGHT_STAR_COUNT : DAY_STAR_COUNT;
    for (int i = 0; i < count; ++i) {
        float x = randf(30, WINDOW_WIDTH - 30);
        float y = randf(20, WINDOW_HEIGHT * 0.45f);
        float baseAlpha = randf(0.55f, 1.0f);
        float twinkleSpeed = randf(0.5f, 1.2f);
        float twinklePhase = randf(0, 6.28f);
        float twinkle3D = randf(0, 6.28f);
        stars.push_back({x, y, baseAlpha, twinkleSpeed, twinklePhase, twinkle3D});
    }
}

void init() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glEnable(GL_STENCIL_TEST);
    initStars();
}

void prepareStaticGeometry() {
    float flat_left_x, flat_right_x, flat_y;
    drawDomeWithFlatBottomTwoLines(flat_left_x, flat_right_x, flat_y);

    float base_bottom;
    fillSquareBase(flat_left_x, flat_right_x, flat_y, base_bottom);

    float road_top = base_bottom;
    float road_top_width = 340.0f;
    float road_top_center = WINDOW_WIDTH / 2.0f;
    float left_tree_x = road_top_center - road_top_width / 2.0f - 24.0f;
    float right_tree_x = road_top_center + road_top_width / 2.0f + 24.0f;
    float bush_y = base_bottom + 6;
    float col1_x_left = flat_left_x - ENTRY_ROOF_OVERHANG + ENTRY_COL_MARGIN;
    float col4_x_right = flat_right_x + ENTRY_ROOF_OVERHANG - ENTRY_COL_MARGIN;
    float bush_x_left = (col1_x_left + left_tree_x) / 2.0f;
    float bush_x_right = (col4_x_right + right_tree_x) / 2.0f;
    float tree_trunk_height = SQUARE_HEIGHT * 1.35f;
    float tree_trunk_width = 16.0f;
    float tree_canopy_radius = 70.0f;
    int nTreesPerSide = 5;
    float offset = 80.0f;
    float front_tree_y = road_top;
    float front_tree_x_left = flat_left_x - offset;
    float front_tree_x_right = flat_right_x + offset;

    generateAllStaticTreesAndBushes(
        flat_left_x, flat_right_x, flat_y, base_bottom,
        road_top_center, road_top_width, road_top, WINDOW_HEIGHT,
        bush_x_left, bush_x_right, bush_y,
        tree_trunk_height, tree_trunk_width, tree_canopy_radius,
        nTreesPerSide, offset, front_tree_x_left, front_tree_x_right, front_tree_y
    );
}

void displayDomeScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // --- Draw sky first ---
    if (sceneMode == MODE_NIGHT)
        drawSkyNight();
    else if (sceneMode == MODE_DAY)
        drawSkyDay();
    else
        drawSkyRainy();

    // --- Draw clouds in rainy mode ---
    if (sceneMode == MODE_RAINY) {
        drawClouds();
    }

    // --- Draw sun or moon or nothing (rainy) ---
    if (sceneMode == MODE_DAY) {
        float sunRadius = 54.0f;
        float sunPathCx = WINDOW_WIDTH / 2.0f;
        float sunPathCy = 340.0f;
        float sunPathRx = (WINDOW_WIDTH / 2.0f) - 60.0f;
        float sunPathRy = 210.0f;
        float sunX = sunPathCx + sunPathRx * cos(M_PI - sunAngle);
        float sunY = sunPathCy - sunPathRy * sin(sunAngle);
        drawSun(sunX, sunY, sunRadius);
    } else if (sceneMode == MODE_NIGHT) {
        drawStars(moonGlowPhase);
        float moonX = WINDOW_WIDTH * 0.78f;
        float moonY = 120.0f;
        float moonR = 48.0f;
        drawCrescentMoon(moonX, moonY, moonR, moonGlowPhase);
    }

    // --- Draw rain if rainy ---
    if (sceneMode == MODE_RAINY) {
        drawRain();
    }

    // --- Draw ground ---
    drawGround();

    fillDomePanelsStriped();

    float flat_left_x, flat_right_x, flat_y;
    drawDomeWithFlatBottomTwoLines(flat_left_x, flat_right_x, flat_y);

    float base_bottom;
    fillSquareBase(flat_left_x, flat_right_x, flat_y, base_bottom);

    drawSquareBaseLines(flat_left_x, flat_right_x, flat_y);

    draw3DRoad(base_bottom);

    // --- Place realistic bush between red columns and adjacent trees ---
    float windPhase = moonGlowPhase;
    bool wind = (sceneMode == MODE_RAINY);
    drawRealBushStatic(allBushes[0], windPhase, wind);
    drawRealBushStatic(allBushes[1], windPhase, wind);

    drawEntryStructure(flat_left_x, flat_right_x, flat_y);

    int nTreesPerSide = 5;
    for (int i = 0; i < nTreesPerSide * 2 + 2; ++i) {
        drawPoincianaBushTreeStatic(allTrees[i], windPhase, wind);
    }

    glFlush();
    // ...Paste the dome/road/trees display() code here...
}
void keyboardDomeScene(unsigned char key, int x, int y) {
    // ...Paste the dome/road/trees keyboard() code here...
    if (key == 'n' || key == 'N') {
        sceneMode = MODE_NIGHT;
        stars.clear();
        for (int i = 0; i < NIGHT_STAR_COUNT; ++i) {
            float x = randf(30, WINDOW_WIDTH - 30);
            float y = randf(20, WINDOW_HEIGHT * 0.45f);
            float baseAlpha = randf(0.55f, 1.0f);
            float twinkleSpeed = randf(0.5f, 1.2f);
            float twinklePhase = randf(0, 6.28f);
            float twinkle3D = randf(0, 6.28f);
            stars.push_back({x, y, baseAlpha, twinkleSpeed, twinklePhase, twinkle3D});
        }
        glutPostRedisplay();
    }
    if (key == 'd' || key == 'D') {
        sceneMode = MODE_DAY;
        stars.clear();
        for (int i = 0; i < DAY_STAR_COUNT; ++i) {
            float x = randf(30, WINDOW_WIDTH - 30);
            float y = randf(20, WINDOW_HEIGHT * 0.45f);
            float baseAlpha = randf(0.55f, 1.0f);
            float twinkleSpeed = randf(0.5f, 1.2f);
            float twinklePhase = randf(0, 6.28f);
            float twinkle3D = randf(0, 6.28f);
            stars.push_back({x, y, baseAlpha, twinkleSpeed, twinklePhase, twinkle3D});
        }
        glutPostRedisplay();
    }
    if (key == 'r' || key == 'R') {
        sceneMode = MODE_RAINY;
        // Rain drops
        rainDrops.clear();
        for (int i = 0; i < RAIN_DROP_COUNT; ++i) {
            float x = randf(0, WINDOW_WIDTH);
            float y = randf(-WINDOW_HEIGHT, WINDOW_HEIGHT);
            float len = randf(10, 15);
            float speed = randf(2.0f, 5.0f);
            rainDrops.push_back({x, y, len, speed});
        }
        // Clouds
        clouds.clear();
        int nClouds = 4;
        float yVals[4] = {90, 140, 190, 110};
        float scaleVals[4] = {110, 90, 70, 60};
        float speedVals[4] = {0.32f, 0.22f, 0.18f, 0.26f};
        for (int i = 0; i < nClouds; ++i) {
            float x = randf(-120, WINDOW_WIDTH - 120);
            clouds.push_back({x, yVals[i], scaleVals[i], speedVals[i]});
        }
        glutPostRedisplay();
    }

}

// --- Unified GLUT callbacks ---
void display() {
    if (currentScene == 0)
        displayAnnexScene();
    else if (currentScene == 1)
        displayDomeScene();
    else if (currentScene == 2)
        displayCampusScene();
}
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (currentScene == 0) {
        gluOrtho2D(0, width, height, 0);
    } else if (currentScene == 1) {
        gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    } else if (currentScene == 2) {
        gluOrtho2D(100, 900, 0, 700);
    }
}

// --- In your main keyboard() function, add: ---
void keyboard(unsigned char key, int x, int y) {
    if (key == '1') {
        currentScene = 0;
        glutIdleFunc(nullptr);
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glutPostRedisplay();
    } else if (key == '2') {
        currentScene = 1;
        if (!domeInitialized) {
            init();
            prepareStaticGeometry();
            domeInitialized = true;
            glutIdleFunc(idle);
        }
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glutPostRedisplay();
    } else if (key == '3') {
        currentScene = 2;
        if (!campusSceneInitialized) {
            // Set up projection for campus scene
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(100, 900, 0, 700);
            campusSceneInitialized = true;
        }
            glutIdleFunc(nullptr); // No GLUT idle for this scene, use timer
            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glutTimerFunc(0, updateCampusScene, 0);
        glutPostRedisplay();
    } else if (currentScene == 0) {
        keyboardAnnexScene(key, x, y);
    } else if (currentScene == 1) {
        keyboardDomeScene(key, x, y);
    } else if (currentScene == 2) {
        keyboardCampusScene(key, x, y);
    }
}
// --- Use pixel-based projection for both scenes ---
// --- In your reshape() function, add: ---

// --- Main function ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 768); // Or 900x700 if you prefer
    glutInitWindowPosition(50, 50);
    glutCreateWindow("AIUB Graphics Project: Scene Switcher");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
