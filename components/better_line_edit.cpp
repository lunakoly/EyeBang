#include "better_line_edit.h"

#include <QKeyEvent>
#include <QPainter>

#include <QDebug>


BetterLineEdit::BetterLineEdit(QWidget *parent) : QLineEdit(parent)
{
	// clean dark style
	setStyleSheet("QLineEdit { background-color: #585858; color: white; border-radius: 5px; font-size: 20px; padding: 0px 5px; } QLineEdit:focus { border: none; }");
}

void BetterLineEdit::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return && !text().isEmpty())
	{
		emit submit(false, text());
	}
	else if (event->key() == Qt::Key_Escape)
	{
		emit submit(true, "");
	}
	else
	{
		QLineEdit::keyPressEvent(event);
	}
}
