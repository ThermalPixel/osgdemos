#include "event_handler.hpp"

bool EventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*)
{
	if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE) 
	{
		
		return true;
	}
	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
	{
		switch (ea.getKey())
		{
		default:
			return false;
		};
	}
	return false;
}