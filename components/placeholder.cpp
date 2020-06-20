#include "placeholder.h"

#include <QVBoxLayout>
#include <QLabel>


Placeholder::Placeholder(const QString &message, QWidget *parent) : QStackedWidget(parent)
{
	QWidget *placeholder = new QWidget(this);
	addWidget(placeholder);

	QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholder);
	placeholderLayout->setAlignment(Qt::AlignCenter);

	QLabel *placeholderMessage = new QLabel(message, this);
	placeholderMessage->setStyleSheet("font-size: 25px;");
	placeholderLayout->addWidget(placeholderMessage);

	setCurrentIndex(0);
}

void Placeholder::setContentWidget(QWidget *content)
{
	addWidget(content);
}

void Placeholder::showContent()
{
	setCurrentIndex(1);
}

void Placeholder::showPlaceholder()
{
	setCurrentIndex(0);
}
