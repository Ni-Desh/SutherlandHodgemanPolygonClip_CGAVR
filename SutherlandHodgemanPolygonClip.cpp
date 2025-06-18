
//right click to clip
/*Enter clipping window coordinates (x_min, y_min, x_max, y_max): 100 100 300 300

Enter number of polygon vertices: 3
Enter polygon vertices (x y):
50 150
200 350
350 150


Enter number of polygon vertices: 4
Enter polygon vertices (x y):
50 150
200 350
350 250
150 50


Enter number of polygon vertices: 5
Enter polygon vertices (x y):
120 80
250 50
350 150
300 300
100 250

*/

#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
    float x, y;
};

vector<Point> polygon;
vector<Point> clippedPolygon;
float x_min, y_min, x_max, y_max;
bool showClipped = false;

Point intersect(Point p1, Point p2, int boundary) {
    Point inter;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    if (boundary == 0) { // Left Edge
        inter.x = x_min;
        inter.y = p1.y + dy * (x_min - p1.x) / dx;
    } else if (boundary == 1) { // Right Edge
        inter.x = x_max;
        inter.y = p1.y + dy * (x_max - p1.x) / dx;
    } else if (boundary == 2) { // Bottom Edge
        inter.y = y_min;
        inter.x = p1.x + dx * (y_min - p1.y) / dy;
    } else { // Top Edge
        inter.y = y_max;
        inter.x = p1.x + dx * (y_max - p1.y) / dy;
    }
    return inter;
}

bool inside(Point p, int boundary) {
    if (boundary == 0) return p.x >= x_min;
    if (boundary == 1) return p.x <= x_max;
    if (boundary == 2) return p.y >= y_min;
    return p.y <= y_max;
}

void clipPolygon(int boundary) {
    vector<Point> temp;
    int size = clippedPolygon.size();

    if (size == 0) return;

    Point prev = clippedPolygon[size - 1];
    for (int i = 0; i < size; i++) {
        Point curr = clippedPolygon[i];
        bool prevInside = inside(prev, boundary);
        bool currInside = inside(curr, boundary);

        if (currInside) {
            if (!prevInside) temp.push_back(intersect(prev, curr, boundary));
            temp.push_back(curr);
        } else if (prevInside) {
            temp.push_back(intersect(prev, curr, boundary));
        }
        prev = curr;
    }
    clippedPolygon = temp;
}

void sutherlandHodgman() {
    clippedPolygon = polygon;
    for (int i = 0; i < 4; i++) clipPolygon(i);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Clipping Window (Red)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_min, y_min);
    glVertex2f(x_max, y_min);
    glVertex2f(x_max, y_max);
    glVertex2f(x_min, y_max);
    glEnd();

    if (!showClipped) {
        // Draw Original Polygon (Blue)
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (auto p : polygon) glVertex2f(p.x, p.y);
        glEnd();
    } else {
        // Draw Clipped Polygon (Green)
        if (!clippedPolygon.empty()) {
            glColor3f(0.0, 1.0, 0.0);
            glBegin(GL_LINE_LOOP);
            for (auto p : clippedPolygon) glVertex2f(p.x, p.y);
            glEnd();
        }
    }

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        sutherlandHodgman();
        showClipped = true;
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    int n;
    cout << "Enter clipping window coordinates (x_min, y_min, x_max, y_max): ";
    cin >> x_min >> y_min >> x_max >> y_max;
    cout << "Enter number of polygon vertices: ";
    cin >> n;
    cout << "Enter polygon vertices (x y): \n";

    for (int i = 0; i < n; i++) {
        Point p;
        cin >> p.x >> p.y;
        polygon.push_back(p);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}


