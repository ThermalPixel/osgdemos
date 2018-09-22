/*
 * TransformReadVisitor.h
 *
 *  Created on: 12.02.2010
 *      Author: Andreas Klein
 */

#ifndef TRANSFORMREADVISITOR_H_
#define TRANSFORMREADVISITOR_H_
#include "DynamicNodeDetector.h"

	class TransformReadVisitor: public DynamicNodeDetector
	{
	public:
		TransformReadVisitor(std::vector<osg::Matrix>* transf) :
			transforms(transf)
		{
			//identity matrix for the static geomtery
			transforms->push_back(osg::Matrix::identity());
		}
		virtual ~TransformReadVisitor()
		{
		}
		virtual void apply(osg::Transform& node);

	private:
		std::vector<osg::Matrix>* transforms;
	};

#endif /* TRANSFORMREADVISITOR_H_ */
