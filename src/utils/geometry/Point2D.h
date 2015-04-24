/*
 * Point2D.h
 *
 *  Created on: Oct 23, 2013
 *      Author: PCX
 */


#ifndef POINT2D_H_
#define POINT2D_H_

namespace geometry {

class Point2D {
private:
	int x,y;
public:
	Point2D();
	Point2D(int x, int y);
	bool operator==(const Point2D &p) const;
	bool operator<(const Point2D &rhs) const;
	Point2D& operator=(const Point2D &rhs);
	int euclidDist(const Point2D &p) const;
	int relativeAngleTo(const Point2D &p) const;
	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);
};

} /* namespace geometry */
#endif /* POINT2D_H_ */
