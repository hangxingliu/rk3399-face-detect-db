#include <stdio.h>
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

	printf("debugPingPong(4) == %d\n", debugPingPong(4));
	initDatabase(NULL);
	FLUSH_STDOUT();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::on_btnStart_clicked() {
	CaptureFrameAndFaceInfo info;
	printf("getFrameFromCapture: %d\n", getFrameFromCapture(NULL, &info));
	FLUSH_STDOUT();
}
