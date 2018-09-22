#pragma once
#include <osgGA/GUIEventHandler>

class EventHandler : public osgGA::GUIEventHandler
{
public:
	EventHandler()

	{
	}
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);
private:
	
};