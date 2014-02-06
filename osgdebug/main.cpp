#include <osgDB/ReadFile>
#include <osg/ArgumentParser>
#include <osg/ApplicationUsage>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include "EnableGLDebugOperation.h"

int main(int argc, char **argv)
{
    osg::setNotifyLevel(osg::WARN);
    osg::ArgumentParser arguments(&argc,argv);
    osgViewer::Viewer viewer;
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new osgViewer::ThreadingHandler());
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    viewer.setRealizeOperation(new EnableGLDebugOperation());
    viewer.setUpViewInWindow(0,0,512,512);

    osg::Node * scene = osgDB::readNodeFiles(arguments);
    if(!scene){
        scene = osgDB::readNodeFile("cow.osg");
    }

    osg::Texture2D * tex = new osg::Texture2D();
    tex->setTextureSize(512,512);
    tex->setSourceFormat(GL_RGBA);
    tex->setInternalFormat(GL_RGBA);
    //create some error
    tex->setSourceType(GL_CLAMP);
    scene->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex,osg::StateAttribute::ON);

    viewer.setSceneData(scene);
    viewer.realize();

    while(!viewer.done())
    {
        viewer.frame();
    }
    return 0;
}
