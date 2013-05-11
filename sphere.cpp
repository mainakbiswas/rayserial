//
//  Framework for a raytracer
//  File: sphere.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    double b = (2*(ray.O.x - position.x)*ray.D.x) 
                + (2*(ray.O.y - position.y)*ray.D.y) 
                + (2*(ray.O.z - position.z)*ray.D.z);
    double c = pow(ray.O.x - position.x, 2) 
                + pow(ray.O.y - position.y, 2) 
                + pow(ray.O.z - position.z, 2) 
                - (r*r);
    double discriminant = b*b - 4*c;
    double t;
    if(discriminant <= 0)
        return Hit::NO_HIT();
    else{
        /// the ray intersects the sphere
			
        // the first root
        double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.000001;

        if (root_1 > 0) {
                // the first root is the smallest positive root
                t = root_1;
        }
        else {
                // the second root is the smallest positive root
                double root_2 = ((sqrt(discriminant) - b)/2) - 0.000001;
                t = root_2;
        }
    }
    if(t<0)
        return Hit::NO_HIT();
//    // place holder for actual intersection calculation
//
//    Vector OC = (position - ray.O).normalized();
//    if (OC.dot(ray.D) < 0.999) {
//        return Hit::NO_HIT();
//    }
//    double t = ((position - ray.O).dot((position - ray.O)));

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    Point intersect = ray.at(t);//ray.O + ray.D * t;
    Vector N = intersect - position;/* = ... */;

    return Hit(t,N.normalized());
}

BoundBox Sphere::getBoundBox(){
    double maxx = position.x + r;
    double minx = position.x - r;
    double maxy = position.y + r;
    double miny = position.y - r;
    double maxz = position.z + r;
    double minz = position.z - r;
    
    return BoundBox(
            Face('x', minx),
            Face('x', maxx),
            Face('y', miny),
            Face('y', maxy),
            Face('z', minz),
            Face('z', maxz)
              );
}
