#ifndef CUBE_GRID_H
#define CUBE_GRID_H

#include "accstructure.h"
#include "box.h"
#define NCUBES 100

class CubeGrid: public AccStructure
{
public:
    Box boxes[NCUBES][NCUBES][NCUBES];
    Box bound;
    
    virtual void storeObjects(std::vector<Object *> objects);
    
    virtual Object* getNearest(const Ray &ray);
};

#endif
