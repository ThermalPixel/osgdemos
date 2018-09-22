#pragma once
#include <osg/NodeVisitor>
#include <osgSim/DOFTransform>

class DynamicNodeDetector: public osg::NodeVisitor
{
public:
    DynamicNodeDetector() :
        osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
    {
    }
    virtual ~DynamicNodeDetector()
    {
    }
protected:
    bool isDynamicNode( osg::Transform& transform)
    {
        osgSim::DOFTransform* dofNode = dynamic_cast<osgSim::DOFTransform*> (&transform);
        if (dofNode)
        {
            return true;
        }
        return transform.getEventCallback() != 0 || transform.getUpdateCallback() != 0;
    }
};

