#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

#include "opencv2/opencv.hpp"
#include "../src/api/api.hpp"

#define FLUSH_STDOUT() fflush(stdout)

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {

	ui->setupUi(this);

	timerCapture = new QTimer(this);
	timerCapture->setInterval(20); //50 fps
	connect(timerCapture, SIGNAL(timeout()), this, SLOT(on_timerCapture_ticked()));

	faces = new FaceManager();

	auto ok = initDatabase(NULL);
	FLUSH_STDOUT();
	if(ok != 0 ) {
		QMessageBox::critical(NULL, "Initialize failed!", "Details in stdout");
		timerCapture->start(); // start timer to close windows
	} else {
		initialized = true;
	}

}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::on_btnStart_clicked() {
	counter = 0;
	timerCapture->start();
}
void MainWindow::on_btnStop_clicked() {
	timerCapture->stop();
}

void MainWindow::on_timerCapture_ticked() {
	if(!initialized) {
		this->close();
		return;
	}

	counter++;
	sprintf(uiStringBuffer, "Count: %d", counter);

	ui->txtCounter->setText(uiStringBuffer);
	timerCapture->stop();

	CaptureRequestOptions opts;
	CaptureFrameAndFaceInfo info;

	int disableFaceDetection = counter & 7; // 0, 8, 16 ... detect face
	opts.disableFaceDetection = disableFaceDetection;

	auto ok = getFrameFromCapture(NULL, &info);

//	printf("getFrameFromCapture() return: %d { w: %d, h: %d, size: %d }\n", ok,
//		info.image.w, info.image.h, info.image.dataLength);

	if(ok == 0) {
//		printf("(after)  info.image.data: %p\n", info.image.data);

		if(!disableFaceDetection) {
			faces->clear();
			for(int i = 0 ; i < info.personCount; i ++ ) {
				auto p = &(info.persons[i]);
				faces->add(p->x0, p->y0, p->x1, p->y1);
			}

			if(info.personCount && info.persons)
				free(info.persons);
		}

		sprintf(uiStringBuffer, "FaceCount: %d", faces->count);
		ui->txtDebug->setText(uiStringBuffer);

		cv::Mat image(info.image.h, info.image.w, CV_8UC3, info.image.data);
		faces->drawToImage(image);

		QImage qImage = QImage(image.data, image.cols, image.rows, image.cols * 3,
			QImage::Format_RGB888);
		ui->imgDisplay->setPixmap(QPixmap::fromImage(qImage));

		free(info.image.data);

		//re-enable timer
		timerCapture->start();
	} else {
		printf("Fatal: getFrameFromCapture => %d\n", ok);
	}

	FLUSH_STDOUT();
}


void MainWindow::on_btnDump_clicked() {
}
