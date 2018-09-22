#include "EventHandler.h"
#include <osgDB/WriteFile>
#include <osg/Timer>

bool EventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE)
    {
        //   _renderer.resize(ea.getWindowWidth(), ea.getWindowHeight());
        return true;
    }
    if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
    {
        switch (ea.getKey())
        {
        default:
            return false;
        case 'r':
        {
            _raytracer->updateScene();
             osg::Timer timer;
             timer.setStartTick();
            _raytracer->trace();
            std::cout << "Time: " <<  timer.time_m() << "\n";
        } break;

        };
    }
    return false;
}
