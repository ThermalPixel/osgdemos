#pragma once
#include <map>
#include <vector>
#include <osg/Matrix>
#include <osg/Node>
#include <osg/Image>
#include "Mesh.h"
#include "EmbreeTypedefs.h"
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

class Raytracer
{
public:
    Raytracer(osg::Node * scene, osg::Camera * camera);
    ~Raytracer();
    void updateScene();
    void setOutputResolution(int w, int h);
    void trace();
private:
    const int TILE_SIZE_X, TILE_SIZE_Y;
    osg::Node * _rootNode;
    osg::Camera * _camera;
    RTCScene _scene;
    std::vector<osg::Matrix> _transforms;
    std::vector<Mesh> _meshes;
    std::map<int,int> _geoToMesh;
    std::map<int,int> _meshToGeo;
    int _width,_height;
    osg::Image * _framebuffer;
    osg::Vec4f _backgroundColor;


    void buildScene();
    void renderTile(int x0, int x1, int y0, int y1, const osg::Vec3f& eye, const osg::Matrix& projToWorld);
    osg::Vec4f renderPixel(int x, int y, const osg::Vec3f& eye, const osg::Vec3f& dir);
    osg::Vec3f getNormal(int geoId, int primId, float u, float v) const;
    osg::Vec4f getSurfaceColor(int geoId, int primId, float u, float v) const;
};
