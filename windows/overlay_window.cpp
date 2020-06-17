#include "overlay_window.h"

#include <QStackedLayout>


OverlayWindow::OverlayWindow(const QString &title)
	: CustomWindow(title)
{
	overlay = new Overlay(this);

	// we could've used absolute positioning
	// instead but int that case we'd need
	// to manually update the overlay size
	QStackedLayout *layout = new QStackedLayout(this);
	layout->setStackingMode(QStackedLayout::StackAll);
	layout->addWidget(overlay);

	layout->setCurrentWidget(overlay);
	overlay->setHidden(true);
}

void OverlayWindow::setContentWidget(QWidget *widget)
{
	layout()->addWidget(widget);
}

void OverlayWindow::setMenuBar(QMenuBar *menuBar)
{
	layout()->setMenuBar(menuBar);
}
