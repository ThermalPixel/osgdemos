#include "EmbreeTypedefs.h"
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/ArgumentParser>
#include <osg/ApplicationUsage>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

#include <common/Utils.h>
#include "SceneVisitor.h"
#include "EventHandler.h"

int main(int argc, char **argv)
{
    osg::setNotifyLevel( osg::WARN );
    osg::ArgumentParser arguments(&argc,argv);
    osgViewer::Viewer viewer;
    common::addColoredStatsHandlerCoreProfile(viewer,osg::Vec4(1,0,0,1));
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new osgViewer::ThreadingHandler());
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    viewer.setUpViewInWindow(10,10,1280,720);
    common::enableGLDebug(&viewer);
    common::addCameraPrinter(viewer);

    osg::Node * scene = osgDB::readNodeFiles(arguments);
    if(!scene){
        scene = osgDB::readNodeFile("cow.osg");
    }
    viewer.setSceneData(scene);

    Raytracer rt(scene, viewer.getCamera());
    rt.setOutputResolution(1280,720);
    viewer.addEventHandler(new EventHandler(&rt));
    common::loadCameraSettings(viewer, "scene.camera");
    viewer.realize();
    while (!viewer.done())
    {
        viewer.frame();

    }
    return 0;
}
