#ifndef TRIANGLEINDEXWRITER_H_
#define TRIANGLEINDEXWRITER_H_


#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/Texture2D>
#include <osg/Material>
#include "VecVisitor.h"
#include "Mesh.h"

class TriangleIndexWriter : public osg::PrimitiveIndexFunctor {
public:
    TriangleIndexWriter(osg::Geometry* geo, Mesh* triList,  osg::Matrix mat/*, rt::Material sm,rt::TextureAtlas* atlas*/);
    //TriangleIndexWriter(osg::Geometry* geo, /*rt::TriangleMesh* triList, */osg::Matrix mat/*, rt::TextureAtlas* atlas*/);
	virtual ~TriangleIndexWriter();

    virtual void setVertexArray(unsigned int,const osg::Vec2*) {}
    virtual void setVertexArray(unsigned int ,const osg::Vec3* ) {}
    virtual void setVertexArray(unsigned int,const osg::Vec4* ) {}
    virtual void setVertexArray(unsigned int,const osg::Vec2d*) {}
    virtual void setVertexArray(unsigned int ,const osg::Vec3d* ) {}
    virtual void setVertexArray(unsigned int,const osg::Vec4d* ) {}

    virtual void begin(GLenum mode){
        _modeCache = mode;
        _indexCache.clear();
    }

    virtual void vertex(unsigned int vert)    {
        _indexCache.push_back(vert);
    }

    virtual void end() {
        if (!_indexCache.empty()) {
            drawElements(_modeCache,_indexCache.size(),&_indexCache.front());
        }
    }

    virtual void drawArrays(GLenum mode,GLint first,GLsizei count);

    virtual void drawElements(GLenum mode,GLsizei count,const GLubyte* indices) {
        drawElementsImplementation<GLubyte>(mode, count, indices);
    }

    virtual void drawElements(GLenum mode,GLsizei count,const GLushort* indices) {
        drawElementsImplementation<GLushort>(mode, count, indices);
    }

    virtual void drawElements(GLenum mode,GLsizei count,const GLuint* indices) {
        drawElementsImplementation<GLuint>(mode, count, indices);
    }

    void writeTriangle(unsigned int i1, unsigned int i2, unsigned int i3);

protected:
	//unsigned int storeTexture(osg::Texture2D* tex);

    template<typename T>
    void drawElementsImplementation(GLenum mode, GLsizei count, const T* indices){

    	if (indices==0 || count==0)
    		return;

        typedef const T* IndexPointer;

        switch(mode)
        {
            case(GL_TRIANGLES):
            {
                IndexPointer ilast = &indices[count];
                for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                    writeTriangle(*iptr,*(iptr+1),*(iptr+2));

                break;
            }
            case(GL_TRIANGLE_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
                    if ((i%2)) writeTriangle(*(iptr),*(iptr+2),*(iptr+1));
                    else       writeTriangle(*(iptr),*(iptr+1),*(iptr+2));
                }
                break;
            }
            case(GL_QUADS):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=4,iptr+=4)
                {
                    writeTriangle(*(iptr),*(iptr+1),*(iptr+2));
                    writeTriangle(*(iptr),*(iptr+2),*(iptr+3));
                }
                break;
            }
            case(GL_QUAD_STRIP):
            {
                IndexPointer iptr = indices;
                for(GLsizei i=3;i<count;i+=2,iptr+=2)
                {
                    writeTriangle(*(iptr),*(iptr+1),*(iptr+2));
                    writeTriangle(*(iptr+1),*(iptr+3),*(iptr+2));
                }
                break;
            }
            case(GL_POLYGON): // treat polygons as GL_TRIANGLE_FAN
            case(GL_TRIANGLE_FAN):
            {
                IndexPointer iptr = indices;
                unsigned int first = *iptr;
                ++iptr;
                for(GLsizei i=2;i<count;++i,++iptr)
                {
                    writeTriangle(first,*(iptr),*(iptr+1));
                }
                break;
            }

            default:
                // uhm should never come to this point :)
            	//throw std::exception("mode failure");
                break;
        }
    }


private:
    std::string storeTexture(osg::Texture2D* tex);

    osg::Geometry* 		_geo;
    GLenum				_modeCache;
    std::vector<GLuint> _indexCache;
    osg::Matrix			_localToWorld;
 //   rt::Material 		_materialCache;
    bool				_hasGeodeMaterial;

    Mesh*			_triangleList;
  //  rt::TextureAtlas * textureAtlas;
};

#endif /* TRIANGLEINDEXWRITER_H_ */
