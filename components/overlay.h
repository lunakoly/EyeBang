#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>

#include "aliases.h"
#include "components/better_line_edit.h"


/**
 * Dark overlay that goes on top of
 * every other content (so use this widget
 * with QStackedLayout with setStackingMode
 * set to QStackedLayout::StackAll).
 */
class Overlay : public QWidget
{
		Q_OBJECT

	public:
		explicit Overlay(QWidget *parent = nullptr);

		/**
		 * Shows overlay and prompts for a line of text.
		 * Suitable for prompting a name for something.
		 */
		void askForText(
			const QString &question,
			const QObject *receiver,
			TextCallback callback,
			QString defaultText = ""
		);

	protected:
		// dark transparent background is drawn here
		void paintEvent(QPaintEvent *event) override;

	private:
		// gui for askForText
		QWidget *textInputPopup;
		QLabel *textInputPopupQuestion;
		BetterLineEdit *textInputPopupText;

		// hides the overlay after askForText
		void textInputPopupSubmitted(bool isCanceled, const QString &text);

		// just for configuration.
		// we will configure each popup
		// in it's own setup function
		void setupTextInputPopup();

		// called when canceling the input
		void cancel();
};


#endif // OVERLAY_H
