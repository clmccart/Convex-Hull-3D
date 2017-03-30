/*  geom.cpp
 *
 *  brute force algorithm for finding the convex hull on a set of points in 3D
 *
 *  created by bridget went and claire mccarthy 2/18/17
 *  last modified 2/24/17
 *
 */


#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;


/* returns 6 times the signed volume of the polyhedron formed by triangle abc
and point d. formula given in Computational Geometry textbook.
*/
int signed_volume(point3d a, point3d b, point3d c, point3d d) {

  return  (((-(a.z - d.z)*(b.y - d.y)*(c.x - d.x) + (a.y - d.y)*(b.z - d.z)*(c.x - d.x) +
  (a.z - d.z)*(b.x - d.x)*(c.y - d.y) - (a.x - d.x)*(b.z - d.z)*(c.y - d.y) -
  (a.y - d.y)*(b.x - d.x)*(c.z - d.z) + (a.x - d.x)*(b.y - d.y)*(c.z - d.z))) / 6);

}

/* return 1 if p,q,r, t on same plane, and 0 otherwise */
int coplanar(point3d p, point3d q, point3d r, point3d t) {

  return signed_volume(p,q,r,t) == 0;
}


/* return 1 if d is  strictly left of abc; 0 otherwise */
int left(point3d a, point3d b, point3d c, point3d d) {

  return signed_volume(a,b,c,d) < 0;
}

/* return 1 if the two points are equal; 0 otherwise */
int isEqual(point3d a, point3d b) {
  return (a.x == b.x && a.y == b.y && a.z == b.z);
}


/* compute and return the convex hull of the points */
vector<triangle3d> brute_force_hull(vector<point3d> &points) {

  vector<triangle3d> result;

  bool extreme = true;

  if (points.size() < 3) {
    return result;
  }

  if (points.size() == 3) {
    triangle3d face = {.a = &points[0], .b = &points[1],.c = &points[2]};
    result.push_back(face);
    return result;
  }

  // loop through all triplets
  for (int i = 0; i < points.size(); ++i) {
    for (int j = 0; j < points.size(); ++j) {
      for (int k = 0; k < points.size(); ++k) {
        if (isEqual(points[i], points[j]) || isEqual(points[i], points[k]) || isEqual(points[j], points[k])){
          continue;
        } else {
          // for each pair pi, pj, pk
          extreme = true;

          // now check if plane defined by pi, pj, pk is extreme
          for (int p = 0; p < points.size(); ++p) {

            if (!isEqual(points[p], points[i]) &&
                !isEqual(points[p], points[j]) &&
                !isEqual(points[p], points[k]) &&
                !left(points[i], points[j], points[k], points[p])) {
              extreme = false;
              break;
            }

          }
          if (extreme == true) {
            // create a face and add this face to the hull
            triangle3d face = {.a = &points[i], .b = &points[j],.c = &points[k]};
            result.push_back(face);
          }
        }

      }
    }
  }
  return result;
}
