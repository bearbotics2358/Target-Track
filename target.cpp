
/****************************************************************************
 **
 ** Copyright (C) 2016 The Qt Company Ltd.
 ** Contact: https://www.qt.io/licensing/
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and The Qt Company. For licensing terms
 ** and conditions see https://www.qt.io/terms-conditions. For further
 ** information use the contact form at https://www.qt.io/contact-us.
 **
 ** BSD License Usage
 ** Alternatively, you may use this file under the terms of the BSD license
 ** as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of The Qt Company Ltd nor the names of its
 **     contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

/*

	updated 7/30/18 BD
	- added rand() as replacement for QRandomGenerator::global->bounded(), which doesn't come in until 5.11



	// find distance to target
	double Vision::distance_calc_cube(int y)
	{
	distance = 480.0 - y;
	return distance;
	}

	// find angle to taret
	double Vision::angle_calc_cube(int xPos)
	{
	// match sign of gyro
	angle = atan((xPos-320.0)/530.47) * (180.0/M_PI);
	return angle; 
	}
		

*/


#include "target.h"

#include <QGraphicsScene>
#include <QPainter>
#include <stdlib.h> // for random()
#include <QStyleOption>
#include <qmath.h>
#include <stdio.h>
#include <qmqtt.h>

const qreal Pi = M_PI;
const qreal TwoPi = 2 * M_PI;

// replacement for QRandomGenerator::global->bounded()
double rand(double max)
{
	double ret;

	ret = (max * random())/((double) RAND_MAX);
	return ret;
}

Target::Target()
	: angle(0), 
		color(rand(256), rand(256), rand(256)), _qout(stdout)
{	
	setRotation(0);

}

QRectF Target::boundingRect() const
{
	qreal adjust = 0.5;
	// return QRectF(-18 - adjust, -22 - adjust, 36 + adjust, 60 + adjust);
	return QRectF(-18 - adjust, -22 - adjust, 50 + adjust, 60 + adjust);
}

QPainterPath Target::shape() const
{
	QPainterPath path;
	path.addRect(-10, -20, 40, 40);
	return path;
}

void Target::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setBrush(Qt::yellow);
	painter->drawEllipse(-10, -20, 40, 40);
}

void Target::onReceived(const QMQTT::Message& message)
{
	double x;
	double y;
	double angle;
	double dist;
	QString s1;
	QPoint camera(0,300); // camera is at (0, 300) on screen
	QPoint pos;
	
	s1 = QString::fromUtf8(message.payload());
	_qout << "target received: \""  << s1 	<< "\"" << endl;

	QStringList list;

	// get values from string 
	list = s1.simplified().split(QRegularExpression("\\s+"));
	dist = list[0].toDouble();
	angle = list[1].toDouble();
	_qout << "target received2: \""  << dist << "  " << angle	<< "\"" << endl;

	// convert distance and angle to x and y values for display
	// camera is at (0,300) on screen
	x = 1.0 * dist * sin(angle * M_PI / 180.0);
	y = -1.0 * dist * cos(angle * M_PI / 180.0);
	printf("x: %6.2lf y: %6.2lf\n", x, y);
	pos = QPoint(x,y);
	pos = camera + pos;
	_qout << "final pos: " << pos.x() << "  " << pos.y() << endl;
	setPos(pos);
	// setPos(-50, -90);

	emit updated();
}

void Target::advance(int step)
{
	// printf("in advance: %d\n", step);

	if (!step)
		return;

}
