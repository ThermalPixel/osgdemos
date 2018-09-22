/*
 * TriangleIndexwriter.cpp
 *
 *  Created on: Jan 18, 2010
 *      Author: token
 */

#include <exception>

#include "TriangleIndexWriter.h"
#include <list>
#include <iostream>
#include <osg/io_utils>
#include <osg/Material>

using namespace std;

TriangleIndexWriter::TriangleIndexWriter(osg::Geometry* geo, Mesh* triList, osg::Matrix mat
                                         /*rt::TextureAtlas * atlas*/) :
    osg::PrimitiveIndexFunctor(), _geo(geo), _triangleList(triList), _localToWorld(mat), _hasGeodeMaterial(false)/*,
              textureAtlas(atlas)*/
{
}

/*TriangleIndexWriter::TriangleIndexWriter(osg::Geometry* geo, rt::TriangleMesh* triList, osg::Matrix mat,
        rt::Material sm, rt::TextureAtlas * atlas) :
    osg::PrimitiveIndexFunctor(), _geo(geo), _triangleList(triList), _localToWorld(mat), _materialCache(sm),
            _hasGeodeMaterial(true), textureAtlas(atlas)
{
}*/

TriangleIndexWriter::~TriangleIndexWriter()
{
}

void TriangleIndexWriter::drawArrays(GLenum mode, GLint first, GLsizei count)
{
    switch (mode)
    {
    case (GL_TRIANGLES):
    {
        unsigned int pos = first;
        for (GLsizei i = 2; i < count; i += 3, pos += 3)
        {
            writeTriangle(pos, pos + 1, pos + 2);
        }
        break;
    }
    case (GL_TRIANGLE_STRIP):
    {
        unsigned int pos = first;
        for (GLsizei i = 2; i < count; ++i, ++pos)
        {
            if ((i % 2))
                writeTriangle(pos, pos + 2, pos + 1);
            else
                writeTriangle(pos, pos + 1, pos + 2);
        }
        break;
    }
    case (GL_QUADS):
    {
        unsigned int pos = first;
        for (GLsizei i = 3; i < count; i += 4, pos += 4)
        {
            writeTriangle(pos, pos + 1, pos + 2);
            writeTriangle(pos, pos + 2, pos + 3);
        }
        break;
    }
    case (GL_QUAD_STRIP):
    {
        unsigned int pos = first;
        for (GLsizei i = 3; i < count; i += 2, pos += 2)
        {
            writeTriangle(pos, pos + 1, pos + 2);
            writeTriangle(pos + 1, pos + 3, pos + 2);
        }
        break;
    }
    case (GL_POLYGON): // treat polygons as GL_TRIANGLE_FAN
    case (GL_TRIANGLE_FAN):
    {
        unsigned int pos = first + 1;
        for (GLsizei i = 2; i < count; ++i, ++pos)
        {
            writeTriangle(first, pos, pos + 1);
        }
        break;
    }

    default:
        //osg::notify(osg::WARN) << "TriangleNodeVisitor :: can't handle mode " << mode << std::endl;
        break;
    }
}

void TriangleIndexWriter::writeTriangle(unsigned int i0, unsigned int i1, unsigned int i2)
{

    Vertex v0;
    Vertex v1;
    Vertex v2;

    Vec3Visitor visitor;
    _geo->getVertexArray()->accept(i0,visitor);
    v0.pos = visitor.getValue() * _localToWorld;
    _geo->getVertexArray()->accept(i1,visitor);
    v1.pos= visitor.getValue() * _localToWorld;
    _geo->getVertexArray()->accept(i2,visitor);
    v2.pos = visitor.getValue() * _localToWorld;

    bool calcNormals = !_geo->getNormalArray();
    if (!calcNormals)
    {
        _geo->getNormalArray()->accept(i0, visitor);
        v0.normal = visitor.getValue();
        _geo->getNormalArray()->accept(i1, visitor);
        v1.normal = visitor.getValue();
        _geo->getNormalArray()->accept(i2, visitor);
        v2.normal = visitor.getValue();
    }
    else
    {
        osg::Vec3f normal = (v2.pos - v0.pos) ^ (v1.pos - v0.pos);
        normal.normalize();
        v0.normal = normal;
        v1.normal = normal;
        v2.normal = normal;
    }

    if (_geo->getNumTexCoordArrays() > 0)
    {
        Vec2Visitor vec2Vis;

        _geo->getTexCoordArray(0)->accept(i0, vec2Vis);
        v0.uv = vec2Vis.getValue();
        _geo->getTexCoordArray(0)->accept(i1, vec2Vis);
        v1.uv = vec2Vis.getValue();
        _geo->getTexCoordArray(0)->accept(i2, vec2Vis);
        v2.uv = vec2Vis.getValue();
     }

    Triangle tri;

    if(_geo->getStateSet())
    {
        osg::StateAttribute* attr = _geo->getStateSet()->getTextureAttribute(0, osg::StateAttribute::TEXTURE);

        osg::Texture2D* texture2D = 0;
        osg::Material* material = dynamic_cast<osg::Material*> (_geo->getStateSet()->getAttribute(osg::StateAttribute::MATERIAL, 0));
        tri.diffuse = material->getDiffuse(osg::Material::FRONT);
        if (attr)
        {
            texture2D = dynamic_cast<osg::Texture2D*> (attr);

            if (texture2D)
            {
                tri.image = texture2D->getImage();
            }
        }
    }


    tri.v0 = _triangleList->addVertex(v0);
    tri.v1 = _triangleList->addVertex(v1);
    tri.v2 = _triangleList->addVertex(v2);
    _triangleList->addTriangle(tri);
    /*
    rt::Triangle tri;
    Vec3Visitor v0;
    Vec3Visitor n0;
    Vec2Visitor t0;
    _geo->getVertexArray()->accept(i0, v0);
    bool calcNormals = !_geo->getNormalArray();
    if (!calcNormals)
    {
        _geo->getNormalArray()->accept(i0, n0);
        tri.v0.normal = n0.getValue();
    }

    tri.v0.position = v0.getValue() * _localToWorld;

    Vec3Visitor v1;
    Vec3Visitor n1;
    Vec2Visitor t1;
    _geo->getVertexArray()->accept(i1, v1);
    if (!calcNormals)
    {
        _geo->getNormalArray()->accept(i1, n1);
        tri.v1.normal = n1.getValue();
    }

    tri.v1.position = v1.getValue() * _localToWorld;

    Vec3Visitor v2;
    Vec3Visitor n2;
    Vec2Visitor t2;
    _geo->getVertexArray()->accept(i2, v2);
    if (!calcNormals)
    {
        _geo->getNormalArray()->accept(i2, n2);
        tri.v2.normal = n2.getValue();
    }

    tri.v2.position = v2.getValue() * _localToWorld;
    if (calcNormals)
    {
        osg::Vec3f normal = (tri.v2.position - tri.v0.position) ^ (tri.v1.position - tri.v0.position);
        normal.normalize();
        tri.v0.normal = normal;
        tri.v1.normal = normal;
        tri.v2.normal = normal;
    }
    osg::StateSet* stset = _geo->getStateSet();

    if (_geo->getNumTexCoordArrays() > 0)
    {
        _geo->getTexCoordArray(0)->accept(i0, t0);
        _geo->getTexCoordArray(0)->accept(i1, t1);
        _geo->getTexCoordArray(0)->accept(i2, t2);
        tri.v0.texCoord = t0.getValue();
        tri.v1.texCoord = t1.getValue();
        tri.v2.texCoord = t2.getValue();
    }
    // Look if Geometry has its own StateSet
    if (stset)
    {

        if (_geo->getNumTexCoordArrays() > 0)
        {

            _geo->getTexCoordArray(0)->accept(i0, t0);
            _geo->getTexCoordArray(0)->accept(i1, t1);
            _geo->getTexCoordArray(0)->accept(i2, t2);
            tri.v0.texCoord = t0.getValue();
            tri.v1.texCoord = t1.getValue();
            tri.v2.texCoord = t2.getValue();
        } else
        {
            //cout << "No TexCoords set for Geometry[" << _geo->getName() << "]\n";
            tri.material.diffuse = SV_CONFIG::default_diffuse;
            tri.material.specular = SV_CONFIG::default_specular;

            tri.material.hasTexture = false;
        }

        osg::StateAttribute* attr = stset->getTextureAttribute(0, osg::StateAttribute::TEXTURE);

        osg::Texture2D* texture2D = 0;
        unsigned int mat_id = 0;

        rt::Material mat;

        osg::Material* material = dynamic_cast<osg::Material*> (stset->getAttribute(osg::StateAttribute::MATERIAL, 0));
        tri.material.diffuse = material->getDiffuse(osg::Material::FRONT);
        tri.material.specular = material->getSpecular(osg::Material::FRONT);

        //cout << "Material dif : " << tri.material.diffuse.x() << " , " << tri.material.diffuse.y() << " , " << tri.material.diffuse.z() << " , " << endl;

        if (attr)
        {
            texture2D = dynamic_cast<osg::Texture2D*> (attr);

            if (texture2D)
            {
                tri.material.hasTexture = true;
                tri.material.texName = storeTexture(texture2D);
                if (mat.texName == "")
                    mat.hasTexture = false;
            }
        }
    } else
    {
        // Test if we have a StateSet from the geode
        if (_hasGeodeMaterial)
        {
            tri.material.diffuse = _materialCache.diffuse;
            tri.material.specular = _materialCache.specular;
            tri.material.texName = _materialCache.texName;
            tri.material.hasTexture = true;
        } else
        {
            cout << "No StateSet for Geometry[" << _geo->getName() << "]\n";
            tri.material.diffuse = SV_CONFIG::default_diffuse;
            tri.material.specular = SV_CONFIG::default_specular;

            tri.material.hasTexture = false;
        }
    }

    _triangleList->AddTriangle(tri);*/
}

std::string TriangleIndexWriter::storeTexture(osg::Texture2D* tex)
{
    /*if (textureAtlas && tex && tex->getImage(0))
    {
        return textureAtlas->AddTexture(tex->getImage(0), osg::Vec2f(tex->getImage(0)->s(), tex->getImage(0)->t()));
    }*/
    return "";

}

