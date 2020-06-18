#ifndef PLACEHOLDER_H
#define PLACEHOLDER_H

#include <QStackedWidget>


/**
 * Allows to replace some widget
 * with a placeholder and switch
 * between them back and forth.
 * Shows the placeholder widget
 * by default.
 */
class Placeholder : public QStackedWidget
{
		Q_OBJECT

	public:
		Placeholder(const QString &message, QWidget *parent = nullptr);

		/**
		 * Sets the specified widget as the
		 * main content widget for this window.
		 */
		void setContentWidget(QWidget *content);
		/**
		 * Shows the content widget.
		 */
		void showContent();
		/**
		 * Shows the placeholder widget.
		 */
		void showPlaceholder();
};


#endif // PLACEHOLDER_H
