#include "cubegrid.h"
#include "boundbox.h"
#include "hit.h"
#include <vector>
#include <set>

//extern bool printTrue;

void CubeGrid::storeObjects(std::vector<Object *> objects){
    if(objects.size() == 0) return;
    double maxx, minx, maxy, miny, maxz, minz;
    minx = objects[0]->getBoundBox().f1.value;
    maxx = objects[0]->getBoundBox().f2.value;
    miny = objects[0]->getBoundBox().f3.value;
    maxy = objects[0]->getBoundBox().f4.value;
    minz = objects[0]->getBoundBox().f5.value;
    maxz = objects[0]->getBoundBox().f6.value;
    unsigned int i,j,k;
    for(i=1; i<objects.size(); i++){
        BoundBox bb = objects[i]->getBoundBox();
        double x1,x2,y1,y2,z1,z2;
        x1 = objects[i]->getBoundBox().f1.value;
        x2 = objects[i]->getBoundBox().f2.value;
        y1 = objects[i]->getBoundBox().f3.value;
        y2 = objects[i]->getBoundBox().f4.value;
        z1 = objects[i]->getBoundBox().f5.value;
        z2 = objects[i]->getBoundBox().f6.value;
        
        minx = x1 < minx? x1 : minx;
        maxx = x2 > maxx? x2 : maxx;
        miny = y1 < miny? y1 : miny;
        maxy = y2 > maxy? y2 : maxy;
        minz = z1 < minz? z1 : minz;
        maxz = z2 > maxz? z2 : maxz;
    }
    bound.f1.axis = 'x';
    bound.f1.value = minx;
    bound.f2.axis = 'x';
    bound.f2.value = maxx;
    bound.f3.axis = 'y';
    bound.f3.value = miny;
    bound.f4.axis = 'y';
    bound.f4.value = maxy;
    bound.f5.axis = 'z';
    bound.f5.value = minz;
    bound.f6.axis = 'z';
    bound.f6.value = maxz;
    
    double xstep = (maxx - minx) / NCUBES;
    double ystep = (maxy - miny) / NCUBES;
    double zstep = (maxz - minz) / NCUBES;
    
    for(i=0;i<NCUBES;i++)
        for(j=0;j<NCUBES;j++)
            for(k=0;k<NCUBES;k++){
                boxes[i][j][k].f1.axis = 'x';
                boxes[i][j][k].f1.value = minx + xstep * i;
                boxes[i][j][k].f2.axis = 'x';
                boxes[i][j][k].f2.value = minx + xstep * (i+1);
                boxes[i][j][k].f3.axis = 'y';
                boxes[i][j][k].f3.value = miny + ystep * j;
                boxes[i][j][k].f4.axis = 'y';
                boxes[i][j][k].f4.value = miny + ystep * (j+1);
                boxes[i][j][k].f5.axis = 'z';
                boxes[i][j][k].f5.value = minz + zstep * k;
                boxes[i][j][k].f6.axis = 'z';
                boxes[i][j][k].f6.value = minz + zstep * (k+1);
            }
    
    unsigned int xmin,xmax, ymin, ymax, zmin, zmax;
    for(i=0;i<objects.size();i++){
        BoundBox bb = objects[i]->getBoundBox();
        xmin = (int)((bb.f1.value - minx) / xstep);
        xmax = (int)((bb.f2.value - minx) / xstep);
        ymin = (int)((bb.f3.value - miny) / ystep);
        ymax = (int)((bb.f4.value - miny) / ystep);
        zmin = (int)((bb.f5.value - minz) / zstep);
        zmax = (int)((bb.f6.value - minz) / zstep);
        
        unsigned int l;
        for(j=xmin;j<=xmax && j<NCUBES;j++)
            for(k=ymin;k<=ymax && k<NCUBES;k++)
                for(l=zmin;l<=zmax && l<NCUBES;l++)
                    boxes[j][k][l].addObject(objects[i]);
                    
        //std::cout<<"box[0][0][0] contains: "<<boxes[0][0][0].objects.size()<<std::endl;
    }
}

Object* CubeGrid::getNearest(const Ray& ray){
    //if ray does not intersect boundbox return NULL
    int firstI, firstJ, firstK;
    double minx = bound.f1.value;
    double maxx = bound.f2.value;
    double miny = bound.f3.value;
    double maxy = bound.f4.value;
    double minz = bound.f5.value;
    double maxz = bound.f6.value;
    
    double xstep = (maxx - minx) / NCUBES;
    double ystep = (maxy - miny) / NCUBES;
    double zstep = (maxz - minz) / NCUBES;
    
    bool inside = false;
    
    Point p;
    Ray currentRay(p, ray.D);
    if(bound.contains(ray.O)){
        firstI = (int)((ray.O.x - minx) / xstep);
        firstJ = (int)((ray.O.y - miny) / ystep);
        firstK = (int)((ray.O.z - minz) / zstep);
        //std::cout<<"reflected "<<firstI<<" "<<firstJ<<" "<<firstK<<std::endl;
        currentRay.O = ray.O;
        inside = true;
    }
    else{
        double t = bound.intersect(ray);
        if(t == std::numeric_limits<double>::infinity())
            return NULL;             //ray does not even hot the big bounding box
        else{
            p = ray.at(t);
            firstI = (int)((p.x - minx) / xstep);
            firstJ = (int)((p.y - miny) / ystep);
            firstK = (int)((p.z - minz) / zstep);
            currentRay.O = p;
        }    
    }
    
    std::set<Object*> objs;
    std::vector<Box*> rayBoxes;
    //rayBoxes.push_back(&boxes[firstI][firstJ][firstK]);
    int i = firstI==NCUBES?NCUBES-1:firstI, j = firstJ==NCUBES?NCUBES-1:firstJ, k = firstK==NCUBES?NCUBES-1:firstK;
    
    //std::cout<<"i,j,k: "<<i<<" "<<j<<" "<<k<<std::endl;
    
    while( 
            i >= 0 && i <= NCUBES - 1 &&
            j >= 0 && j <= NCUBES - 1 &&
            k >= 0 && k <= NCUBES - 1
         ){
        //if(printTrue) std::cout<<"i,j,k: "<<i<<" "<<j<<" "<<k<<" ray.D: "<<ray.D<<std::endl;
        rayBoxes.push_back(&boxes[i][j][k]);
        for(unsigned int l = 0; l < boxes[i][j][k].objects.size(); l++){
            objs.insert(boxes[i][j][k].objects[l]);
        }
        double dist[3];
        if(ray.D.x < 0){
            dist[0] = (boxes[i][j][k].f1.intersect(ray)).t;
        }
        else{
            dist[0] = (boxes[i][j][k].f2.intersect(ray)).t;
        }
        if(ray.D.y < 0){
            dist[1] = (boxes[i][j][k].f3.intersect(ray)).t;
        }
        else{
            dist[1] = (boxes[i][j][k].f4.intersect(ray)).t;
        }
        if(ray.D.z < 0){
            dist[2] = (boxes[i][j][k].f5.intersect(ray)).t;
        }
        else{
            dist[2] = (boxes[i][j][k].f6.intersect(ray)).t;
        }
        
        //if(printTrue) std::cout<<"dist: "<<dist[0]<<" "<<dist[1]<<" "<<dist[2]<<std::endl;
        int l=4; double min = std::numeric_limits<double>::infinity();
        for(int m = 0; m<3;m++){
            if(!isnan(dist[m]) && dist[m] < min){
                min = dist[m];
                l = m;
            }
        }
        switch(l){
            case 0:
                if(ray.D.x < 0) i--; else i++; break;
            case 1:
                if(ray.D.y < 0) j--; else j++; break;
            case 2:
                if(ray.D.z < 0) k--; else k++; break;
        }
    }
    
//    while( 
//            i >= 0 && i <= NCUBES - 1 &&
//            j >= 0 && j <= NCUBES - 1 &&
//            k >= 0 && k <= NCUBES - 1
//         ){
//        rayBoxes.push_back(&boxes[i][j][k]);
//        for(unsigned int l = 0; l < boxes[i][j][k].objects.size(); l++){
//            objs.insert(boxes[i][j][k].objects[l]);
//        }
//        //std::cout<<"inserted into objs: "<<boxes[i][j][k].objects.size()<<" objects"<<std::endl;
//        
//        int nextFace = 8;
//        double dist = std::numeric_limits<double>::infinity();
//        Face* faces[] = {&boxes[i][j][k].f1, &boxes[i][j][k].f2, &boxes[i][j][k].f3,
//                        &boxes[i][j][k].f4, &boxes[i][j][k].f5, &boxes[i][j][k].f6};
//        for(int l = 0; l< 6 ; l++){
//            double d = boxes[i][j][k].computeDist(currentRay, faces[l]);
//            if(!isnan(d) && d>=0.00000001 && d<dist){
//                dist = d;
//                nextFace = l;
//            }
//        }
//        
//        currentRay.O = currentRay.at(dist);
//        switch(nextFace){
//            case 0: i-= 1; break;
//            case 1: i+= 1; break;
//            case 2: j-= 1; break;
//            case 3: j+= 1; break;
//            case 4: k-= 1; break;
//            case 5: k+= 1; break;
//        }
//        
//        if(nextFace>5){
//            
//            break;
//        }
//    }
    
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object* retObj = NULL;
    //std::cout << "size for this ray: "<<objs.size()<<std::endl;
    std::vector<Object*> objConsider (objs.begin(), objs.end());
    //std::cout<<"objects considered: "<<objConsider.size()<<std::endl;
    for (unsigned int i = 0; i < objConsider.size(); ++i) {
        Hit hit(objConsider[i]->intersect(ray));
        if (!isnan(hit.t) && hit.t<min_hit.t) {
            min_hit = hit;
            retObj = objConsider[i];
        }
    }
//    if(inside && retObj!=NULL)
//        std::cout<<"reflected ray found object"<<std::endl;
    return retObj;
}
