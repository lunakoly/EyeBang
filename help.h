#include <QDebug>
#include <QStyleFactory>


/*!
	Some helper things.
*/
namespace Help {
	/*!
		Shows the available styles for this platform.
	*/
	void printAvailableStyles()
	{
		qDebug() << "Possible styles are:";
		for (auto & it : QStyleFactory::keys()) {
			qDebug() << "=>" << it;
		}
	}
}
