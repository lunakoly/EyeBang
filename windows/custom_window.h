#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QWidget>


/**
 * All our windows should extend this class
 * for the ease of further customization.
 */
class CustomWindow : public QWidget
{
		Q_OBJECT

	public:
		explicit CustomWindow(const QString &title, QWidget *parent = nullptr);
};


#endif // CUSTOMWINDOW_H
