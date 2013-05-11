#ifndef FACE_H
#define FACE_H

#include "triple.h"
#include "ray.h"
#include "hit.h"

//face can only be along x, y or z plane
class Face
{
public:         //eqn is axis = value
    char axis;
    double value; 
    
    Face(char a='i', double v=0.0) : axis(a), value(v) { }
    Face(const Face &f) : axis(f.axis), value(f.value) { }
    
    Hit intersect(const Ray &ray){
        double v1, v2, v3;
        
        v1 = value;
        switch(axis){
            case 'x':
                v2 = ray.O.x;
                v3 = ray.D.x;
                break;
            case 'y':
                v2 = ray.O.y;
                v3 = ray.D.y;
                break;
            case 'z':
                v2 = ray.O.z;
                v3 = ray.D.z;
                break;
        }        
        
        if(v3 == 0.0) return Hit::NO_HIT(); //parallel
        
        double dist = (v1 - v2) / v3;
        if(dist <= 0.0)
            return Hit::NO_HIT();
        else
            return Hit(dist, Vector(0,0,0));  //normal not used, and I want null objects in c++
    }
    
    bool operator==(const Face &f){
        return (axis == f.axis && value == f.value);
    }
};

#endif