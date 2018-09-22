
#include <osgDB/ReadFile>
#include <osg/ArgumentParser>
#include <osg/ApplicationUsage>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <common/Utils.h>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include "event_handler.hpp"
#include "CubeShadowMapping.h"

class LogFileHandler : public osg::NotifyHandler
{
public:
	LogFileHandler( const std::string& file )
	{ _log.open( file.c_str() ); }
	virtual ~LogFileHandler() { _log.close(); }
	virtual void notify(osg::NotifySeverity severity, const char* msg)
	{ _log << msg; }
protected:
	std::ofstream _log;
};

int main(int argc, char **argv)
{
    //osg::setNotifyLevel( osg::DEBUG_INFO );
    //osg::setNotifyHandler( new LogFileHandler("output.txt") );
	osg::ArgumentParser arguments(&argc,argv);
	osgViewer::Viewer viewer;
	//viewer.addEventHandler(new osgViewer::StatsHandler());
    common::addColoredStatsHandlerCoreProfile(viewer,osg::Vec4(1,0,0,1));
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.addEventHandler(new osgViewer::ThreadingHandler());
	viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    //viewer.setUpViewInWindow(100,100,1024,768);
	common::enableGLDebug(&viewer);
    common::addCameraPrinter(viewer);
	std::string cameraFile = "";
	while (arguments.read("--camera", cameraFile))	{}
	if(cameraFile.size() > 0)
    common::loadCameraSettings(viewer,cameraFile);
	common::addCameraPrinter(viewer);
	osg::Node * scene = osgDB::readNodeFiles(arguments);
	if(!scene){
		scene = osgDB::readNodeFile("cow.osg");
	}


    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 0;
    traits->width = 1024;
    traits->height = 768;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
    traits->vsync = false;
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (!gc) {
        osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;

    }


    viewer.setSceneData(scene);
    CubeShadowMap * cm = new CubeShadowMap(2048,osg::Vec3f(-2, 1.0, 6.2859));
    cm->setGraphicsContext(gc);

    viewer.addSlave(cm);

    CubeShadowMapping * shadow_scene= new CubeShadowMapping(cm);
    shadow_scene->setGraphicsContext(gc);
    shadow_scene->setViewport(0,0,1024,768);
    viewer.addSlave(shadow_scene);

	//activate the osg_* attributes for shaders
    osgViewer::Viewer::Windows windows;
	viewer.getWindows(windows);
	for (auto it = windows.begin();it != windows.end();++it)
	{
		(*it)->getState()->setUseModelViewAndProjectionUniforms(true);
		(*it)->getState()->setUseVertexAttributeAliasing(true);
	}

	viewer.addEventHandler(new EventHandler());
	viewer.realize();

    float angle = 0;
	while(!viewer.done())
	{
  //     cm->setLightPos(osg::Vec3(std::cos(angle) -2.0,std::sin(angle) + 1.0,6));
    //    angle += 0.01;
		viewer.frame();
	}
}
