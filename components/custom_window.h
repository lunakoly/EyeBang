#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QWidget>


class CustomWindow : public QWidget
{
		Q_OBJECT

	public:
		explicit CustomWindow(const QString &title, QWidget *parent = nullptr);
};


#endif // CUSTOMWINDOW_H
