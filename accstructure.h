#ifndef ACC_STRUCTURE_H
#define ACC_STRUCTURE_H

#include <vector>
#include "object.h"
#include "ray.h"

class AccStructure
{
public:
    virtual ~AccStructure(){ }
    
    virtual void storeObjects(std::vector<Object*> objects) = 0;
    
    virtual Object* getNearest(const Ray &ray) = 0;
};

#endif