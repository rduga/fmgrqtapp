/*
 * FMoveDialog.cpp
 *
 *  Created on: Jan 18, 2010
 *      Author:
 */

#include "FMoveDialog.h"

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
#include <QPushButton>

FMoveDialog::FMoveDialog(const QList<QFileInfo>& files, const QString& destination , QWidget* parent /* = 0 */) : QDialog(parent),
	files(files),
	destination(destination),

//	ow_rename(new QPushButton(tr("rename"), this)),
	ow_skip(new QPushButton(tr("skip"), this)),
	ow_skip_all(new QPushButton(tr("skip all"), this)),
	ow_overwrite(new QPushButton(tr("overwrite"), this)),
	ow_overwrite_all(new QPushButton(tr("overwrite all"), this)),

	overwrite_mode(FMgr::OW_UNKNOWN),
	overwrite_qbox(new QMessageBox(this)),

	progress_bar(new QProgressBar()),
	source_path(new QLineEdit()),
	destination_path(new QLineEdit()),
	buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel))
{
	createGui();
	connectSignals();
}

FMoveDialog::~FMoveDialog()
{
}

void FMoveDialog::createGui()
{
	overwrite_qbox->setWindowTitle(tr("Overwrite file?"));
	overwrite_qbox->setText(tr("File in destination already exists. Do you want to overwrite it?"));

//	overwrite_qbox->addButton(ow_rename, QMessageBox::ActionRole);
	overwrite_qbox->addButton(ow_skip, QMessageBox::RejectRole);
	overwrite_qbox->addButton(ow_skip_all, QMessageBox::NoRole);
	overwrite_qbox->addButton(ow_overwrite, QMessageBox::AcceptRole);
	overwrite_qbox->addButton(ow_overwrite_all, QMessageBox::YesRole);
	overwrite_qbox->setDefaultButton(ow_overwrite_all);

	setWindowTitle(tr("FMoveDialog"));

	QVBoxLayout* main_layout = new QVBoxLayout(this);

	QGroupBox* box_progress = new QGroupBox("Progress of operation");
	QGroupBox* box_srcdest = new QGroupBox("Source and Destination");

	setupBoxProgress(box_progress);
	setupBoxSrcDest(box_srcdest);

	main_layout->addWidget(box_progress);
	main_layout->addWidget(box_srcdest);
	main_layout->addWidget(buttons);
}

void FMoveDialog::setupBoxProgress(QGroupBox* box)
{
	QGridLayout* box_layout = new QGridLayout();

	progress_bar->setMinimum(0);
//	progress_bar->setMaximum(files.size());

	box_layout->addWidget(progress_bar);

	box->setLayout(box_layout);
}

void FMoveDialog::setupBoxSrcDest(QGroupBox* box)
{
	QGridLayout* box_layout = new QGridLayout();

	QLabel* src_label = new QLabel(tr("Source File:"));
	QLabel* dest_label = new QLabel(tr("Destination File:"));

	source_path->setReadOnly(true);
	source_path->setDisabled(true);
	destination_path->setReadOnly(true);
	destination_path->setDisabled(true);

	box_layout->addWidget(src_label, 0, 0);
	box_layout->addWidget(source_path, 0, 1);

	box_layout->addWidget(dest_label, 1, 0);
	box_layout->addWidget(destination_path, 1, 1);

	box->setLayout(box_layout);
}

void FMoveDialog::fileList(const QFileInfo& info, QFileInfoList& list)
{
	// file or symlink
	if (info.isFile()) {
		return;
	} else if (!info.isDir()) {
		qDebug("File %s is not a file nor a directory.", qPrintable(info.absoluteFilePath()));
		return;
	} // else is directory

	if (!info.isReadable()) {
		QMessageBox::critical(this, tr("Directory is not readable"),
				tr("Directory %1 is not readable.").arg(info.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
		return;
	}

	QDir srcDir(info.absoluteFilePath());

	srcDir.setFilter( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	srcDir.setSorting( QDir::Name );

	QFileInfoList srclist = srcDir.entryInfoList();

	foreach (QFileInfo src, srclist) {
		list.append(src);

		if (src.isDir()) {
			fileList(src, list);
		}
	}
}

void FMoveDialog::connectSignals()
{
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

void FMoveDialog::accept()
{
	qDebug("files to move:");

	QFileInfoList all_files;

	if (files.empty()) {
		QDialog::accept();
		return;
	}

	foreach (QFileInfo file, files) {
		all_files.append(file);
		fileList(file, all_files);
	}

	QDir curr_dir = files[0].dir();

	qDebug("CurrDir: %s", qPrintable(curr_dir.absolutePath()));

	progress_bar->setMaximum(all_files.count());

	int val = 0;

	QFileInfoList dirs_to_remove;

	qDebug("list and move of all files:");
	foreach (QFileInfo file, all_files) {
		QString dest_path(QString("%1/%2").arg(destination).arg(curr_dir.relativeFilePath(file.absoluteFilePath())));

		qDebug("File: %s move to: %s", qPrintable(file.absoluteFilePath()), qPrintable(dest_path));
		progress_bar->setValue(val++);
		source_path->setText(file.absoluteFilePath());
		destination_path->setText(dest_path);
		qApp->processEvents();

		if (file.isFile()) {
			QFile cfile(file.absoluteFilePath());
			QFile dfile(dest_path);

			// overwrite destination file?
			if (dfile.exists()) {
				if (overwrite_mode == FMgr::OW_UNKNOWN || overwrite_mode == FMgr::OW_SKIP ||
						overwrite_mode == FMgr::OW_OVERWRITE) {
					qDebug("Destionation File Exists: %s", qPrintable(dest_path));
					overwrite_qbox->exec();

					QAbstractButton* clicked_button = overwrite_qbox->clickedButton();

//					if ( clicked_button == ow_rename) {
//
//					} else
					if (clicked_button == ow_skip) {
						qDebug("Overwrite mode: OW_SKIP");
						overwrite_mode = FMgr::OW_SKIP;
						continue;
					} else if (clicked_button == ow_skip_all) {
						qDebug("Overwrite mode: OW_SKIP_ALL");
						overwrite_mode = FMgr::OW_SKIP_ALL;
						continue;
					} else if (clicked_button == ow_overwrite) {
						qDebug("Overwrite mode: OW_OVERWRITE");
						overwrite_mode = FMgr::OW_OVERWRITE;
					} else if (clicked_button == ow_overwrite_all) {
						qDebug("Overwrite mode: OW_OVERWRITE_ALL");
						overwrite_mode = FMgr::OW_OVERWRITE_ALL;
					} else {
						continue;
					}
				}

				if (overwrite_mode == FMgr::OW_SKIP_ALL) {
					continue;
				} else if (overwrite_mode == FMgr::OW_OVERWRITE_ALL || overwrite_mode == FMgr::OW_OVERWRITE) {
					if (overwrite_mode == FMgr::OW_OVERWRITE) {
						qDebug("Setting OW_UNKNOWN");
						overwrite_mode = FMgr::OW_UNKNOWN;
					}

					QFileInfo dfileinfo(dfile);
					if (file.absoluteFilePath() == dfileinfo.absoluteFilePath()) {
						qDebug("Nothing to rewrite, source and destination file are the same: %s", qPrintable(file.absoluteFilePath()));
						overwrite_mode = FMgr::OW_UNKNOWN;
						continue;
					}

					if (!dfile.remove()) {
						qDebug("Unable to overwrite file: %s", qPrintable(file.absoluteFilePath()));
						QMessageBox::critical(this, tr("Unable to overwrite file"),
								tr("Unable to overwrite file: %1").arg(file.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
					}
				}
			}

			if (!cfile.copy(dest_path)) {
				qDebug("Unable to move file: %s", qPrintable(file.absoluteFilePath()));
				QMessageBox::critical(this, tr("Unable to copy file"),
						tr("Unable to move file (copy part): %1").arg(dest_path), QMessageBox::Cancel, QMessageBox::Cancel);
				continue;
			}

			if (!cfile.remove()) {
				qDebug("Unable to move file: %s", qPrintable(file.absoluteFilePath()));
				QMessageBox::critical(this, tr("Unable to copy file"),
						tr("Unable to move file (delete part): %1").arg(dest_path), QMessageBox::Cancel, QMessageBox::Cancel);
				continue;
			}


		} else if (!file.isDir()) {
			qDebug("File %s is not a file nor a directory.", qPrintable(file.absoluteFilePath()));
			continue;
		} else {
			QDir destDir(dest_path);

			dirs_to_remove.prepend(file);

			if (destDir.absolutePath().indexOf(file.absoluteFilePath()) == 0) {
				qDebug("Cannot move directory (%s) into its subdirectory (%s)",
						qPrintable(file.absoluteFilePath()), qPrintable(destDir.absolutePath()));
				QMessageBox::critical(this, tr("Move Dir"),
						tr("Cannot move directory (%1) into its subdirectory (%2)").arg(file.absoluteFilePath()).arg(destDir.absolutePath()));
				continue;
			}

			if (!destDir.exists()) {
				qDebug("Creaging directory path: %s", qPrintable(destDir.absolutePath()));
				if (!destDir.mkpath(destDir.absolutePath())) {
					QMessageBox::information(this, tr("Copy Dir"),
							tr("Could not create destination directory %1").arg(destDir.canonicalPath()));
					continue;
				}

				QFile destFile(dest_path);
				if (!destFile.setPermissions(file.permissions())) {
					QMessageBox::information(this, tr("Copy Dir"),
							tr("Could not set permissions of destination directory %1").arg(destDir.canonicalPath()));
					continue;
				}
			}
		}
	}

	foreach (QFileInfo file, dirs_to_remove) {
		QDir file_dir(file.dir());

		file_dir.setFilter( QDir::Files);
		file_dir.setSorting( QDir::Name );

		qDebug("Directory %s, entries: %d", qPrintable(file.absoluteFilePath()) ,file_dir.entryList().count());

//		foreach (QString entry, file_dir.entryList()) {
//			qDebug(entry);
//		}

		// empty directory
		if (file_dir.entryList().empty()) {

			qDebug("Removing directory: %s from directory %s", qPrintable(file.fileName()), qPrintable(file_dir.absolutePath()));

			if (! (file_dir.rmdir(file.fileName()))) {
				QMessageBox::critical(this, tr("Unable to remove directory"),
						tr("Unable to remove directory: %1").arg(file.absoluteFilePath()), QMessageBox::Cancel, QMessageBox::Cancel);
			}
		}
	}

	source_path->clear();
	destination_path->clear();
	progress_bar->setValue(files.size());

	QMessageBox::information(this, tr("Moving finished"),
			tr("Moving has finished"));

	QDialog::accept();
}
