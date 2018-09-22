#pragma once
#include <iostream>
#include <vector>
#include <osg/Vec3>
#include <osg/io_utils>

struct Triangle
{
    int v0,v1,v2;
    osg::Image * image;
    osg::Vec4f diffuse;
    Triangle()
        :v0(0), v1(0),v2(0), image(nullptr)
    {

    }
};

struct Vertex
{
    osg::Vec3f pos;
    osg::Vec3f normal;
    osg::Vec2f uv;

};

class Mesh
{
public:

    int addVertex(const Vertex& vertex)
    {
        _vertices.push_back(vertex);
        return _vertices.size()-1;
    }

    void addTriangle(const Triangle& triangle)
    {
        _triangleIndices.push_back(triangle);
    }

    std::size_t getNumTriangles() const
    {
        return _triangleIndices.size() ;
    }

    std::size_t getNumVertices() const
    {
        return _vertices.size();
    }

    const std::vector<Vertex>& getVertices() const
    {
        return _vertices;
    }

    const std::vector<Triangle>& getTriangles() const
    {
        return _triangleIndices;
    }
private:
    std::vector<Vertex> _vertices;
    std::vector<Triangle> _triangleIndices;
};
