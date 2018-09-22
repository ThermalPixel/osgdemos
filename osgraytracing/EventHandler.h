#pragma once
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include "Raytracer.h"
class EventHandler : public osgGA::GUIEventHandler
{
public:
    EventHandler(Raytracer* viewer)
        :_raytracer(viewer)

	{
	}
    virtual ~EventHandler(){}
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);
private:
    Raytracer* _raytracer;
};
