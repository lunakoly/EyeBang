#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>

#include "components/better_line_edit.h"


/**
 * Dark overlay that goes on top of
 * every other content (so use this widget
 * with QStackedLayout with setStackingMode
 * set to QStackedLayout::StackAll).
 */
class OverlayWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit OverlayWidget(QWidget *parent = nullptr);

		/**
		 * A type of a callback needed for accepting the
		 * text answer. The one may need to cast their
		 * funtion pointer to this type explicitly.
		 */
		using AnswerReceiver = void (QObject::*)(const QString &answer);

		/**
		 * Shows overlay and prompts for a line of text.
		 * Suitable for prompting a name for something.
		 */
		void askForText(const QString &question, const QObject *receiver, AnswerReceiver callback);

	protected:
		// dark transparent background is drawn here
		void paintEvent(QPaintEvent *event) override;

	private:
		// gui for askForText
		QWidget *textInputPopup;
		QLabel *textInputPopupQuestion;
		BetterLineEdit *textInputPopupText;

		// hides the overlay after askForText
		void textInputPopupSubmitted(const QString &text);

		// just for configuration.
		// we will configure each popup
		// in it's own setup function
		void setupTextInputPopup();

		// called when canceling the input
		void cancel();
};


#endif // OVERLAYWIDGET_H
