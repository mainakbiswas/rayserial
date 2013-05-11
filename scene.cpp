//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"
#include <stdlib.h>

//bool printTrue = false;

Color Scene::trace(const Ray &ray, int numRefls)
{
    if(numRefls == -1) return Color(0.0, 0.0, 0.0);
    // Find hit object and distance
//    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
//    Object *obj = NULL;
//    for (unsigned int i = 0; i < objects.size(); ++i) {
//        Hit hit(objects[i]->intersect(ray));
//        if (!isnan(hit.t) && hit.t<min_hit.t) {
//            min_hit = hit;
//            obj = objects[i];
//            //std::cout<< "object hit" << endl;
//        }
//    }
    
    Object *obj = cubeDS.getNearest(ray);
    

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Hit min_hit = obj->intersect(ray);
    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    Color color, reflColor;
    double NdotView = N.dot(ray.D);
    Vector reflVector = -(N*2.0*(NdotView) - ray.D); // reflected vector from hit point away from object
    Ray reflRay(hit, reflVector);
    if(material->ks > 0.0)
        reflColor = trace(reflRay, numRefls - 1);

    for(unsigned int i = 0; i<lights.size(); i++){
        color += computeShading(lights.at(i), hit, N, V, material, false);
    }
    Light reflLight(reflRay.at(1.0), reflColor);
    color += computeShading(&reflLight, hit, N, V, material, true);
    return color;
}

Color Scene::computeShading(Light *l, Point p, Vector n, Vector view, Material *m, bool isReflectedLight)
{
    Color color;
    Vector lightVector = -(l->position - p).normalized(); // vector from light source to point
    Ray shadowRay(p, -lightVector); //ray from point to light source
    double NdotL = n.dot(-lightVector); //dot product of surface normal and vector from point to light source
    Vector R = (n*2.0*(NdotL) + lightVector).normalized();
    double VdotR = view.dot(R);

//    Hit obs_hit(std::numeric_limits<double>::infinity(),Vector());
//    unsigned int j;
//    for (j = 0; j < objects.size(); ++j) {
//        obs_hit = objects[j]->intersect(shadowRay);
//        if(!isnan(obs_hit.t) && obs_hit.t > 0.0) break;
//    }

    Object *ob = cubeDS.getNearest(shadowRay);
    if(!shadows || isReflectedLight || ob==NULL/*j == objects.size()*/) {   // no obstruction on the way to light source
        //diffuse colour
        NdotL = max(NdotL, 0.0);
        Color dmc = m->color * m->kd;
        if(!isReflectedLight)
            color += (l->color * dmc * NdotL);

        //specular colour
        VdotR = VdotR>0?VdotR:0;
        VdotR = pow(VdotR, m->n);
        //Color smc = material->color * material->ks;
        color += l->color * m->ks * VdotR;
    }
    if(!isReflectedLight)
        color += l->color * m->color * m->ka; //ambient light
    return color;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    std::cout << MaxRecursionDepth << std::endl;
    
    cubeDS.storeObjects(objects);
    #pragma omp parallel for
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            //if(x==200 && y==200) printTrue = true; else printTrue = false;
            Point pixel(x, h-1-y, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray, MaxRecursionDepth);
            col.clamp();
            img(x,y) = col;
            //std::cout<< "traced for i,j: "<<y<<" "<<x<<std::endl;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setShadows(bool s)
{
    shadows = s;
}

void Scene::setMaxRecursionDepth(int d)
{
    MaxRecursionDepth = d;
}
