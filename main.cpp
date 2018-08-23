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


#include <QtWidgets>

#include <math.h>
#include <stdlib.h> // atoi
// #include <iostream>

#include "target.h"

#include "subscriber.h"

static const int MouseCount = 1;

QTextStream _qout;

// extern ostream cout;

void usage()
{
	printf("usage: target_track [-h <host address>] [-p <port number>]\n");
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QGraphicsScene scene;

		// setup my own scene class and override advance() to do painting
		
    scene.setSceneRect(-300, -300, 600, 600);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

		Target target;
		target.setPos(0, 0);
		scene.addItem(&target);

    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.setDragMode(QGraphicsView::ScrollHandDrag);
    view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Target Track"));
    // view.resize(400, 300);

		// camera
		scene.addRect(0, 300, 50, 50, QPen(Qt::blue), QBrush(Qt::blue));
		// scene.addRect(0, 0, 50, 50, QPen(Qt::blue), QBrush(Qt::blue));
		
    view.resize(1000, 800);
    view.show();

		int n;

		// set default host and port
		QHostAddress host = QHostAddress::LocalHost;
		quint16 port = 1183;

		for(n = 1; n < argc; n++) {
			if(argv[n][0] == '-') {
				switch(argv[n][1]) {
				case 'h': // host address
					if(n + 1 < argc) {
						host.setAddress(argv[n+1]);
						printf("host: *%s*\n", argv[n+1]);
					} else {
						printf("error: missing parameter\n");
						usage();
						exit(-1);
					}
					break;
					
				case 'p': // host port
					if(n + 1 < argc) {
						port = atoi(argv[n+1]);
					} else {
						printf("error: missing parameter\n");
						usage();
						exit(-1);
					}
					break;

				default:
					printf("unknown command\n");
					break;
				}
			}
		}

		QTextStream(stdout) << "attempting to connect to " << host.toString() << ": " << port << endl;
		
		// Subscriber subscriber;
		Subscriber subscriber(host, port);
    subscriber.connectToHost();

		QObject::connect(&subscriber, &Subscriber::received, &target, &Target::onReceived);
		
		/*
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    timer.start(1000 / 33);
		*/

		// QObject::connect(&target, SIGNAL(target.updated()), &scene, SLOT(advance()));

    return app.exec();
}
