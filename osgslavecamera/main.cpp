#include <osgDB/ReadFile>
#include <osg/ArgumentParser>
#include <osg/ApplicationUsage>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include "RenderColorToTexture.h"
#include "DisplayTexture.h"

int main(int argc, char **argv)
{
    osg::setNotifyLevel(osg::WARN);
    osg::ArgumentParser arguments(&argc,argv);
    osgViewer::Viewer viewer;
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new osgViewer::ThreadingHandler());
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    viewer.addEventHandler(new osgViewer::StatsHandler());

    osg::Node * scene = osgDB::readNodeFiles(arguments);
    if(!scene){
        scene = osgDB::readNodeFile("cow.osg");
    }
    viewer.setSceneData(scene);

    int width = 1024;
    int height = 768;

    //create the graphics context
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 0;
    traits->width = width;
    traits->height = height;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
    traits->vsync = false;
    osg::ref_ptr<osg::GraphicsContext>  graphicsContext =
                         osg::GraphicsContext::createGraphicsContext(traits.get());

    RenderColorToTexture *rtt = new RenderColorToTexture();
    //Important: Assign viewport and graphics context!
    rtt->setViewport(0,0,width,height);
    rtt->setGraphicsContext(graphicsContext);
    //add it as a slave to the viewer
    viewer.addSlave(rtt);

    //set up the display camera.
    DisplayTexture *display = new DisplayTexture;
    display->setGraphicsContext(graphicsContext);
    display->setViewport(0,0,width,height);
    display->setTexture(rtt->getTexture());
    //The display camera provides it's own scene graph
    viewer.addSlave(display, osg::Matrix(), osg::Matrix(), false);

    viewer.realize();

    while(!viewer.done())
    {
        viewer.frame();
    }
    return 0;
}
