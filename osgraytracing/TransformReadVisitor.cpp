/*
 * TransformReadVisitor.cpp
 *
 *  Created on: 12.02.2010
 *      Author: Andreas Klein
 */

#include "TransformReadVisitor.h"
#include <osg/Geometry>
#include <osg/Geode>

	void TransformReadVisitor::apply(osg::Transform& node)
	{
		if (isDynamicNode(node))
		{
			osg::Matrix m = osg::computeLocalToWorld(getNodePath());

			transforms->push_back(m);
		}
	}

