#ifndef BOUND_BOX_H
#define BOUND_BOX_H

#include "face.h"

class BoundBox
{
public:
    Face f1, f2, f3, f4, f5, f6;
    
    BoundBox(Face f1, Face f2, Face f3, Face f4, Face f5, Face f6) : 
    f1(f1), f2(f2), f3(f3), f4(f4), f5(f5), f6(f6) { }
};

#endif
