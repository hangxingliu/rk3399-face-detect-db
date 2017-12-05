#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "opencv2/opencv.hpp"

#include "./face_manager.hpp"

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btnStart_clicked();
	void on_btnStop_clicked();
	void on_timerCapture_ticked();

	void on_btnDump_clicked();

private:	
	FaceManager* faces;

	char uiStringBuffer[128];
	int counter;

	bool initialized = false;

	QTimer *timerCapture;
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
