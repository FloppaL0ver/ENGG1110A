/**
 * ENGG1110 Problem Solving by Programming 
 *
 * I declare that the assignment here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 *   http://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name : Paisal, Pavat Zhang
 * Student ID   : 1155280121
 * Class/Section: A
 * Date         : 19/11/2026
 */

/* include header file */
#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y;      // 2D rectilinear coordinates of a point    
} Point;

double computeDistance(Point p1, Point p2) {
  // Write your code here
  return sqrt(pow(p1.x-p2.x ,2) + pow(p1.y-p2.y, 2) );
}

double computeTriangleArea(Point T[3]) {
  // Write your code here
  double a = computeDistance(T[0], T[1]);
  double b = computeDistance(T[1], T[2]);
  double c = computeDistance(T[2], T[3]);
  double s = (a+b+c)/2;
  return sqrt(s* (s-a) * (s-b) * (s-c));
  
  
}

int main(void) {
  // Don't modify the following line
  Point triangleA[3]; // 3 points in the triangle

  // Write your code here
  printf("p1? ");
  scanf("%lf %lf", &triangleA[0].x, &triangleA[0].y);
  printf("p2? ");
  scanf("%lf %lf", &triangleA[1].x, &triangleA[1].y);
  printf("p3? ");
  scanf("%lf %lf", &triangleA[2].x, &triangleA[2].y);

  printf("p1:(%.1lf, %.lf), p2:(%.1lf, %.lf), p3:(%.1lf, %.lf), area=%.1lf", triangleA[0].x,triangleA[0].y, triangleA[1].x, triangleA[1].y, triangleA[2].x, triangleA[2].y, computeTriangleArea(triangleA));
  


  return 0;
}