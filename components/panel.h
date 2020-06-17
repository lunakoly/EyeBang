#ifndef PANEL_H
#define PANEL_H

#include <QWidget>


/**
 * Just a widget with dark background
 * color and rounded corners.
 */
class Panel : public QWidget
{
		Q_OBJECT

	public:
		explicit Panel(QWidget *parent = nullptr);

	protected:
		void paintEvent(QPaintEvent *event) override;
};


#endif // PANEL_H
