#ifndef BOX_H
#define BOX_H

#include "face.h"
#include "object.h"
#include <vector>
#include "triple.h"
#include "hit.h"
#include <stdlib.h>

class Box
{
public:
    Face f1, f2, f3, f4, f5, f6;
    std::vector<Object*> objects;
    
    Box(Face f1, Face f2, Face f3, Face f4, Face f5, Face f6) : 
    f1(f1), f2(f2), f3(f3), f4(f4), f5(f5), f6(f6) { }
    
    Box() { }
    
    void addObject(Object *obj){
        objects.push_back(obj);
    }
    
    //returns closest face where ray intersects this box
    double intersect(const Ray &ray){
        Face* f[]={&f1, &f2, &f3, &f4, &f5, &f6};
        double dist = std::numeric_limits<double>::infinity();
        Face *res = NULL;
        for(int i=0;i<6;i++){
            double d = computeDist(ray, f[i]);
            if(!isnan(d) && d < dist){
                dist = d;
                res = f[i];
            }
        }
        return dist;
    }
    
    bool contains(Point p){
        if(p.x < f1.value - 0.00001)
            return false;
        if(p.x > f2.value + 0.00001)
            return false;
        if(p.y < f3.value - 0.00001)
            return false;
        if(p.y > f4.value + 0.00001)
            return false;
        if(p.z < f5.value - 0.00001)
            return false;
        if(p.z > f6.value + 0.00001)
            return false;
        return true;
    }
    
    double computeDist(const Ray &ray, Face *f){
        Hit h = f->intersect(ray);
        if(!isnan(h.t)){
            Point p = ray.at(h.t);
            if(contains(p))
                return h.t;
            else
                return std::numeric_limits<double>::quiet_NaN();
        }
        return h.t;
    }
};

#endif
