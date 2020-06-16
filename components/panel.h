#ifndef PANEL_H
#define PANEL_H

#include <QWidget>


class Panel : public QWidget
{
		Q_OBJECT

	public:
		explicit Panel(QWidget *parent = nullptr);

	protected:
		void paintEvent(QPaintEvent *event) override;
};


#endif // PANEL_H
