#ifndef RENDERLEGACYWINDOW_H
#define RENDERLEGACYWINDOW_H

#include "custom_window.h"

#include <QTemporaryFile>

#include "editing/settings.h"
#include "editing/project.h"


class RenderLegacyWindow : public CustomWindow
{
		Q_OBJECT

	public:
		explicit RenderLegacyWindow(Settings &settings, Project &project);

	private:
		Settings &settings;
		Project &project;

		QFile *rangesFile = nullptr;
};


#endif // RENDERLEGACYWINDOW_H
