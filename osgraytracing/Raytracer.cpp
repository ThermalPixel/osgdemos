#include "Raytracer.h"
#include <ctime>
#include <random>
#include <osgDB/WriteFile>
#include <osg/Vec3i>
#include "TransformReadVisitor.h"
#include "SceneVisitor.h"
#include <common/Utils.h>

Raytracer::Raytracer(osg::Node *scene, osg::Camera *camera)
    :_rootNode(scene), _camera(camera), TILE_SIZE_X(32),TILE_SIZE_Y(32), _backgroundColor(0.3f,0.4f,0.5f,1.0f)
{
    rtcInit();
    _scene = rtcNewScene(RTC_SCENE_STATIC, RTC_INTERSECT1);
    buildScene();
    setOutputResolution(512,512);
}
Raytracer::~Raytracer()
{
    rtcDeleteScene(_scene);
    rtcExit();
}

void Raytracer::updateScene()
{
    _transforms.clear();
    TransformReadVisitor visitor(&_transforms);
    _rootNode->accept(visitor);

    for (int i =0; i < _transforms.size();++i)
    {
        osg::Matrixf mat = _transforms[i];
        rtcSetTransform(_scene, _meshToGeo.at(i), RTC_MATRIX_ROW_MAJOR, mat.ptr());
    }
}

void Raytracer::setOutputResolution(int w, int h)
{
    _width = w;
    _height = h;
    _framebuffer = new osg::Image;
    _framebuffer->allocateImage(w,h, 1, GL_RGB, GL_UNSIGNED_BYTE);
}

namespace constants
{
    const float pi =3.141592653589f;
};
void sample_unit_sphere(int32_t num_samples, osg::Vec3f * samples)
        {
            for (int32_t i = 0; i < num_samples;++i)
            {
                float u1 = common::random_float();
                float u2 = common::random_float();
                float z = 1.0f - 2.0f * u1;
                float r = std::sqrt(std::max(0.f, 1.f - z*z));
                float phi = 2 * constants::pi * u2;
                float x = r * std::cos(phi);
                float y = r *std::sin(phi);
                samples[i] = osg::Vec3(x, y, z);
            }
        }


osg::Vec4f Raytracer::renderPixel(int x, int y, const osg::Vec3f& eye, const osg::Vec3f& dir)
{
    osg::Vec3f lightPos(-338.572, 0, 400);
    RTCRay ray;
    ray.primID = RTC_INVALID_GEOMETRY_ID;
    ray.geomID = RTC_INVALID_GEOMETRY_ID;
    ray.instID = RTC_INVALID_GEOMETRY_ID;
    ray.tnear = 0.0f;
    ray.tfar = 100000.0f;
    ray.org[0] = eye.x();
    ray.org[1] = eye.y();
    ray.org[2] = eye.z();
    ray.dir[0]=  dir.x();
    ray.dir[1] = dir.y();
    ray.dir[2] = dir.z();
    rtcIntersect(_scene, ray);
    if(ray.primID != -1)
    {
        osg::Vec3f pos = eye + dir*ray.tfar;
        osg::Vec4f diffuse = getSurfaceColor(ray.geomID, ray.primID,ray.u,ray.v);
        osg::Vec3 normal = getNormal(ray.geomID,ray.primID,ray.u,ray.v);
        osg::Vec3f lightDir = lightPos - pos;
        lightDir.normalize();

        float NdotL = std::max(normal * lightDir,0.0f);
        return diffuse * NdotL;

    }
    return _backgroundColor;
}

void Raytracer::renderTile(int x0, int x1, int y0, int y1, const osg::Vec3f& eye, const osg::Matrix& projToWorld)
{
    for (int y = y0; y < y1;++y)
    {
        for (int x = x0; x < x1;++x)
        {
            osg::Vec3f p(2.0f * x /(float)_width - 1.0f, 2 * y  / (float)_height - 1.0f, 1);
            osg::Vec3f dir = (p*projToWorld) - eye;
            dir.normalize();

            osg::Vec4f color = renderPixel(x,y, eye, dir);
            common::setColor(_framebuffer,osg::Vec2f(x,y),color);
        }
    }
}

void Raytracer::trace()
{
    osg::Vec3f eye,center,up;
    _camera->getViewMatrixAsLookAt(eye,center,up);
    osg::Matrix projToWorld = osg::Matrix::inverse(_camera->getViewMatrix() *_camera->getProjectionMatrix());

    const int numTilesX = (_width +TILE_SIZE_X-1)/TILE_SIZE_X;
    const int numTilesY = (_height+TILE_SIZE_Y-1)/TILE_SIZE_Y;

    for (int i =0; i < numTilesX*numTilesY;++i)
    {
        int tileY = i / numTilesX;
        int tileX= i - tileY*numTilesX;
        int x0 = tileX * TILE_SIZE_X;
        int x1 = std::min(x0+TILE_SIZE_X,_width);
        int y0 = tileY * TILE_SIZE_Y;
        int y1 = std::min(y0+TILE_SIZE_Y,_height);
        renderTile(x0,x1,y0,y1,eye,projToWorld);
    }

    osgDB::writeImageFile(*_framebuffer,"result.bmp");
}

void Raytracer::buildScene()
{
    SceneVisitor visitor(_meshes);
    _rootNode->accept(visitor);
    for (int  meshId = 0; meshId < _meshes.size();++meshId)
    {
        unsigned int geoId = rtcNewTriangleMesh (_scene, RTC_GEOMETRY_STATIC, _meshes[meshId].getNumTriangles(), _meshes[meshId].getNumVertices());
        osg::Vec4f* vertices = (  osg::Vec4f*) rtcMapBuffer(_scene,geoId,RTC_VERTEX_BUFFER);
        for (int i =0; i < _meshes[meshId].getNumVertices();++i)
        {
           Vertex tri = _meshes[meshId].getVertices()[i];
           vertices[i].x() =tri.pos.x();
           vertices[i].y() =tri.pos.y();
           vertices[i].z() =tri.pos.z();
        }
        rtcUnmapBuffer(_scene,geoId,RTC_VERTEX_BUFFER);

        osg::Vec3i* triangles = (osg::Vec3i*) rtcMapBuffer(_scene,geoId,RTC_INDEX_BUFFER);
        for (int i =0; i < _meshes[meshId].getNumTriangles();++i)
        {
            Triangle tri = _meshes[meshId].getTriangles()[i];
            triangles[i].x() =tri.v0;
            triangles[i].y() =tri.v1;
            triangles[i].z() =tri.v2;
        }
        rtcUnmapBuffer(_scene,geoId,RTC_INDEX_BUFFER);
        _geoToMesh[geoId] = meshId;
        _meshToGeo[meshId] = geoId;
    }
    rtcCommit(_scene);
}

osg::Vec3f Raytracer::getNormal(int meshId, int primId, float u, float v) const
{
   Triangle tri = _meshes[meshId].getTriangles()[primId];
   osg::Vec3 n1 = _meshes[meshId].getVertices()[tri.v0].normal;
   osg::Vec3 n2 = _meshes[meshId].getVertices()[tri.v1].normal;
   osg::Vec3 n3 = _meshes[meshId].getVertices()[tri.v2].normal;
   osg::Vec3f normal = n1*(1-u-v) + n2*u+n3*v;
   normal.normalize();
   return normal;
}

osg::Vec4f Raytracer::getSurfaceColor(int meshId, int primId, float u, float v) const
{
    Triangle tri = _meshes[meshId].getTriangles()[primId];
    osg::Vec2 uv = _meshes[meshId].getVertices()[tri.v0].uv *(1-u-v) + _meshes[meshId].getVertices()[tri.v1].uv*u + _meshes[meshId].getVertices()[tri.v2].uv*v;
    uv.x() = uv.x() - std::floor(uv.x());
    uv.y() = uv.y() - std::floor(uv.y());
    if(tri.image)
    {
        return tri.image->getColor(uv);
    }
    else
    {
        return osg::Vec4f(tri.diffuse.x(),tri.diffuse.y(),tri.diffuse.z(),1);
    }
}
