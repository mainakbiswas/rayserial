//
//  Framework for a raytracer
//  File: triangle.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Mainak Biswas
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef TRIANGLE_H_115209AE
#define TRIANGLE_H_115209AE

#include "object.h"

class Triangle : public Object
{
public:
    Triangle(Point p1, Point p2, Point p3, Vector n1, Vector n2, Vector n3) : 
    vertex1(p1), vertex2(p2), vertex3(p3),
    normal1(n1), normal2(n2), normal3(n3){ }

    virtual Hit intersect(const Ray &ray);
    
    virtual BoundBox getBoundBox();

    const Point vertex1;
    const Point vertex2;
    const Point vertex3;
    const Vector normal1;
    const Vector normal2;
    const Vector normal3;
};

#endif /* end of include guard: SPHERE_H_115209AE */
