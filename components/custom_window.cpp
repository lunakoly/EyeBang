#include "custom_window.h"


CustomWindow::CustomWindow(const QString &title, QWidget *parent) : QWidget(parent)
{
	setWindowTitle(title);
	resize(1000, 600);
//	setWindowFlags(Qt::FramelessWindowHint);
//	setWindowFlags(Qt::CustomizeWindowHint);
}
