/*
 * VecVisitor.h
 *
 *  Created on: Jan 19, 2010
 *      Author: token
 */

#ifndef VECVISITOR_H_
#define VECVISITOR_H_

#include <osg/Geometry>

/** writes all values of an array out to a stream, applies a matrix beforehand if necessary */
class Vec3Visitor: public osg::ValueVisitor {
public:
	Vec3Visitor() :
		osg::ValueVisitor() {
	}

	virtual void apply(float & inv) {
		_origin.set(inv, 0, 0);
	}

	virtual void apply(osg::Vec2 & inv) {
		_origin.set(inv[0], inv[1], 0);
	}

	virtual void apply(osg::Vec3f &inv) {
		float a = inv.x();
		float b = inv.y();
		float c = inv.z();
		_origin.set(a, b, c);
	}
	virtual void apply(osg::Vec3d & inv) {
		_origin.set(inv[0], inv[1], inv[2]);
	}
	virtual void apply(osg::Vec2b & inv) {
		_origin.set(inv[0], inv[1], 0);
	}

	virtual void apply(osg::Vec3b & inv) {
		_origin.set(inv[0], inv[1], inv[2]);
	}

	virtual void apply(osg::Vec2s & inv) {
		_origin.set(inv[0], inv[1], 0);
	}

	virtual void apply(osg::Vec3s & inv) {
		_origin.set(inv[0], inv[1], inv[2]);
	}

	osg::Vec3f getValue() {
		return _origin;
	}
private:
	osg::Vec3 _origin;
};

/** writes all values of an array out to a stream, applies a matrix beforehand if necessary */
class Vec2Visitor: public osg::ValueVisitor {
public:
	Vec2Visitor() :
		osg::ValueVisitor() {
	}

	virtual void apply(float & inv) {
		_origin.set(inv, 0);
	}

	virtual void apply(osg::Vec2f &inv) {
		float a = inv.x();
		float b = inv.y();
		_origin.set(a, b);
	}

	virtual void apply(osg::Vec2d & inv) {
		_origin.set(inv[0], inv[1]);
	}
	virtual void apply(osg::Vec2b & inv) {
		_origin.set(inv[0], inv[1]);
	}

	virtual void apply(osg::Vec2s & inv) {
		_origin.set(inv[0], inv[1]);
	}

	osg::Vec2f getValue() {
		return _origin;
	}
private:
	osg::Vec2 _origin;
};

#endif /* VECVISITOR_H_ */
