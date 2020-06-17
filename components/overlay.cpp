#include "overlay.h"

#include <QVBoxLayout>
#include <QStackedLayout>
#include <QMetaMethod>
#include <QDebug>
#include <QPainter>

#include "panel.h"


void Overlay::setupTextInputPopup()
{
	textInputPopup = new Panel(this);
	textInputPopup->setFixedSize(250, 75);

	QVBoxLayout *textInputPopupContents = new QVBoxLayout(textInputPopup);

	textInputPopupQuestion = new QLabel(textInputPopup);
	textInputPopupQuestion->setStyleSheet("color: white; font-size: 20px;");
	textInputPopupQuestion->setAlignment(Qt::AlignCenter);
	textInputPopupContents->addWidget(textInputPopupQuestion);

	textInputPopupText = new BetterLineEdit(textInputPopup);
	textInputPopupContents->addWidget(textInputPopupText);
}

Overlay::Overlay(QWidget *parent) : QWidget(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setHidden(true);

	setupTextInputPopup();

	// box layout is used here
	// because it gives us easy
	// center alignment
	QVBoxLayout *popups = new QVBoxLayout(this);
	popups->setAlignment(Qt::AlignCenter);
	popups->addWidget(textInputPopup);
}

void Overlay::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.save();

	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(0, 0, 0, 220));
	painter.drawRect(rect());

	painter.restore();
}

void Overlay::cancel()
{
	setHidden(true);
}

void Overlay::askForText(
	const QString &question,
	const QObject *receiver,
	TextCallback callback
)
{
	textInputPopupQuestion->setText(question);
	textInputPopupText->disconnect();
	textInputPopupText->setText("");
	textInputPopupText->setFocus();
	setHidden(false);

	// esc pressed
	connect(textInputPopupText, &BetterLineEdit::escape, this, &Overlay::cancel);

	// call user code and hide the overlay
	connect(textInputPopupText, &BetterLineEdit::submit, this, &Overlay::textInputPopupSubmitted);
	connect(textInputPopupText, &BetterLineEdit::submit, receiver, callback);
}

void Overlay::textInputPopupSubmitted(const QString &text)
{
	Q_UNUSED(text);
	setHidden(true);
}
