#ifndef OVERLAYWINDOW_H
#define OVERLAYWINDOW_H

#include "custom_window.h"

#include "components/overlay.h"

#include <QMenuBar>


/**
 * A window with an overlay component that
 * can be used to ask the user. Use setWidget()
 * function to add the main content widget.
 */
class OverlayWindow : public CustomWindow
{
		Q_OBJECT

	public:
		explicit OverlayWindow(const QString &title);

		/**
		 * Sets the specified widget as the
		 * main content widget for this window.
		 */
		void setContentWidget(QWidget *widget);
		/**
		 * Sets the menubar to the inner layout.
		 */
		void setMenuBar(QMenuBar *menuBar);

	protected:
		Overlay *overlay;
};


#endif // OVERLAYWINDOW_H
