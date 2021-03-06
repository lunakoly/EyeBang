#include <QApplication>

#include "help.h"
#include "windows/editor_window.h"


int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	Help::printAvailableStyles();

	// the common style for all the platforms
	application.setStyle("Fusion");

	// or we could try using something like this:
	// QStyle *platformStyle = QApplication::style(); // it's objectName = ""
	// QApplication::setStyle("Fusion");
	// menuBar = new QMenuBar(this);
	// menuBar->setStyle(platformStyle);

	EditorWindow window;
	window.show();

	return application.exec();
}
