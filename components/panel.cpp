#include "panel.h"

#include <QPainter>


Panel::Panel(QWidget *parent) : QWidget(parent)
{

}

void Panel::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.save();

	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor("#383838"));
	painter.drawRoundedRect(rect(), 5, 5);

	painter.restore();
}
