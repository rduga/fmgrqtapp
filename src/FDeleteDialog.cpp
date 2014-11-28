/*
 * FDeleteDialog.cpp
 *
 *  Created on: Jan 13, 2010
 *      Author:
 */

#include "FDeleteDialog.h"
#include "FCopyDialog.h"

#include <QBoxLayout>
#include <QLineEdit>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QTimer>
#include <QDir>
#include <QMessageBox>
#include <QFileInfo>
#include <QCoreApplication>
#include <QProgressDialog>

FDeleteDialog::FDeleteDialog(const QList<QFileInfo>& files, QWidget* parent /* = 0 */) : QDialog(parent),
	files(files),

	progress_bar(new QProgressBar()),
	source_path(new QLineEdit()),
	buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel))
{
	createGui();
	connectSignals();
}

FDeleteDialog::~FDeleteDialog()
{
}

void FDeleteDialog::createGui()
{
	setWindowTitle(tr("FDeleteDialog"));

	QVBoxLayout* main_layout = new QVBoxLayout(this);

	QGroupBox* box_progress = new QGroupBox("Progress of operation");
	QGroupBox* box_srcdest = new QGroupBox("Source and Destination");

	setupBoxProgress(box_progress);
	setupBoxSrcDest(box_srcdest);

	main_layout->addWidget(box_progress);
	main_layout->addWidget(box_srcdest);
	main_layout->addWidget(buttons);
}

void FDeleteDialog::setupBoxProgress(QGroupBox* box)
{
	QGridLayout* box_layout = new QGridLayout();

	progress_bar->setMinimum(0);
	progress_bar->setMaximum(files.size());

	box_layout->addWidget(progress_bar);

	box->setLayout(box_layout);
}

void FDeleteDialog::setupBoxSrcDest(QGroupBox* box)
{
	QGridLayout* box_layout = new QGridLayout();

	QLabel* src_label = new QLabel(tr("Source File:"));
//	QLabel* dest_label = new QLabel(tr("Destination File:"));

	source_path->setReadOnly(true);
//	destination_path->setReadOnly(true);

	box_layout->addWidget(src_label, 0, 0);
	box_layout->addWidget(source_path, 0, 1);

//	box_layout->addWidget(dest_label, 1, 0);
//	box_layout->addWidget(destination_path, 1, 1);

	box->setLayout(box_layout);
}

bool FDeleteDialog::clearDir(const QString& src_dir_path)
{
	qDebug("Clearing dir: %s", qPrintable(src_dir_path));
	QDir srcDir(src_dir_path);
	QString msg;

	srcDir.setFilter( QDir::Files | QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot );
	srcDir.setSorting( QDir::Name );

	QFileInfoList srclist = srcDir.entryInfoList();

	QProgressDialog progress( tr("Deleting files..."), tr("Abort Backup"), 0, srclist.count(), this);
	progress.setWindowTitle(tr("Deleting Dir"));

	qDebug("deleting files...");

	int val = 0;

	foreach (QFileInfo di, srclist) {
		msg.sprintf("Deleting file \"%s\"...", qPrintable(di.absoluteFilePath()));
		qDebug(qPrintable(msg));
		progress.setLabelText(msg);
		progress.setValue(val++);
		if ( progress.wasCanceled() ) {
			break;
		}
		qApp->processEvents();
		QString tmp = di.absoluteFilePath();
		QString tmp2 = srcDir.canonicalPath();
		if (!di.isDir()) {
			QFile cfile(di.absoluteFilePath());

			if (!cfile.remove()) {
				qDebug("Unable to remove file: %s", qPrintable(di.absoluteFilePath()));
				QMessageBox::critical(this, tr("Unable to remove file"),
						tr("Unable to remove file: %1").arg(di.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
			}
		} else {
			qDebug("Recursive delete directory: %s", qPrintable(di.absoluteFilePath()));

			if (!clearDir(di.absoluteFilePath())) {
				qDebug("Unable to clear directory: %s", qPrintable(di.absoluteFilePath()));
				continue;
			}

			if (! (di.dir().rmdir(di.fileName()))) {
				QMessageBox::critical(this, tr("Unable to remove directory"),
						tr("Unable to remove directory: %1").arg(di.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
				continue;
			}
		}
	}
	progress.setValue( srclist.count() );

	return true;
}

void FDeleteDialog::connectSignals()
{
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

void FDeleteDialog::accept()
{
	qDebug("files to copy:");

	int val = 0;

	foreach (QFileInfo info, files) {
		qDebug("FILE:%s", qPrintable(info.absoluteFilePath()));

		progress_bar->setValue(val++);
		source_path->setText(info.absoluteFilePath());
		qApp->processEvents();

		if (!info.isReadable()) {
			QMessageBox::critical(this, tr("Directory/File is not readable"),
					tr("Directory/File %1 is not readable.").arg(info.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
			continue;
		}

		if (info.isDir()) {
			if (!clearDir(info.absoluteFilePath())) {
				qDebug("Unable to clear directory: %s", qPrintable(info.absoluteFilePath()));
				QMessageBox::critical(this, tr("Unable to clear directory"),
						tr("Unable to clear directory: %1").arg(info.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
				continue;
			}

			if (! (info.dir().rmdir(info.fileName()))) {
				QMessageBox::critical(this, tr("Unable to remove directory"),
						tr("Unable to remove directory: %1").arg(info.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
				continue;
			}
		} else if (info.isFile()) {
			QFile cfile(info.absoluteFilePath());
			if (!cfile.remove()) {
				qDebug("Unable to remove file: %s", qPrintable(info.absoluteFilePath()));
				QMessageBox::critical(this, tr("Unable to remove file"),
						tr("Unable to remove file: %1").arg(info.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
			}
		}
	}

	progress_bar->setValue(files.size());

	source_path->clear();

	QDialog::accept();
}
