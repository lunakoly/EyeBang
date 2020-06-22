#include "render_legacy_window.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QProcess>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDebug>


RenderLegacyWindow::RenderLegacyWindow(Settings &settings, Project &project)
	: CustomWindow("Render (Legacy CLI)")
	, settings(settings)
	, project(project)
{
	setWindowModality(Qt::ApplicationModal);
	resize(600, 350);

	QVBoxLayout *layout = new QVBoxLayout(this);

	QTextEdit *contents = new QTextEdit(this);
	contents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(contents);

	// check if cut.py has
	// been specified
	if (!QFile(settings.legacyCLIScriptPath).exists())
	{
		QMessageBox::critical(this, tr("Error"), tr("The path to the legacy Ranger CLI utility hasn't been specified!"));
		close();
		return;
	}

	// ask for the output folder
	QString saveDirectory = QFileDialog::getExistingDirectory(
		this,
		tr("Resulting Files Location"),
		""
	);

	if (saveDirectory.isNull())
	{
		close();
		return;
	}

	// setup the ranges file
	if (settings.legacyCLIRangesFileName.isEmpty())
	{
		// write a temporary ranges file
		auto temporary = new QTemporaryFile(this);
		rangesFile = temporary;

		if (!temporary->open())
		{
			QMessageBox::critical(this, tr("Error"), tr("Couln't create a temporary ranges file") + " (" + temporary->errorString() + ")");
			close();
			return;
		}
	}
	else
	{
		rangesFile = new QFile(saveDirectory + "/" + settings.legacyCLIRangesFileName);

		if (!rangesFile->open(QIODevice::ReadWrite))
		{
			QMessageBox::critical(this, tr("Error"), tr("Couln't create the ranges file") + " (" + rangesFile->errorString() + ")");
			close();
			return;
		}
	}

	project.exportRangesFile(*rangesFile);

	// setup the output files
	// template name
	QString outputFilesPattern = settings.legacyCLIOutputFilesPattern;

	if (outputFilesPattern.isEmpty())
	{
		outputFilesPattern = "{}.mp4";
	}

	// setup the python path
	QString command = settings.legacyCLIPythonCommand;

	if (command.isEmpty())
	{
		command = "python";
	}

	// construct the command
	QStringList arguments;
	arguments << "-u";
	arguments << settings.legacyCLIScriptPath;
	arguments << "-i" << project.getVideoFile();
	arguments << "-r" << rangesFile->fileName();
	arguments << "-o" << QDir(saveDirectory + "/" + outputFilesPattern).absolutePath();

	// fire
	QProcess *process = new QProcess(this);

	connect(
		process,
		QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
		[this, contents](int, QProcess::ExitStatus){
			contents->append("Finished!");
			this->rangesFile->deleteLater();
		}
	);

	connect(
		process,
		&QProcess::readyReadStandardOutput,
		[process, contents](){
			contents->append(process->readAllStandardOutput());
		}
	);

	connect(
		process,
		&QProcess::readyReadStandardError,
		[process, contents](){
			contents->append(process->readAllStandardError());
		}
	);

	// process->setProcessChannelMode(QProcess::ForwardedChannels);
	process->start(command, arguments, QIODevice::ReadWrite | QIODevice::Unbuffered);
}
