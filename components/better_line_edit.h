#ifndef BETTERLINEEDIT_H
#define BETTERLINEEDIT_H

#include <QLineEdit>


/**
 * QLineEdit with additional signal
 * for detecting returnPressed and passing
 * the text as a parameter at once.
 */
class BetterLineEdit : public QLineEdit
{
		Q_OBJECT

	public:
		BetterLineEdit(QWidget *parent = nullptr);

	signals:
		/**
		 * Emitted when the return key is pressed
		 * and if text is not empty.
		 */
		void submit(const QString &text);
		/**
		 * Emitted when the escape key is pressed.
		 */
		void escape();

	protected:
		// handles Qt::Key_Return
		void keyPressEvent(QKeyEvent *event) override;
};


#endif // BETTERLINEEDIT_H
