/*
 * FMoveDialog.h
 *
 *  Created on: Jan 18, 2010
 *      Author:
 */

#ifndef FMOVEDIALOG_H_
#define FMOVEDIALOG_H_

#include "FInclude.h"

#include <QDialog>
#include <QList>
#include <QFileInfo>
#include <QThread>

class QLineEdit;
class QProgressBar;
class QDialogButtonBox;
class QGroupBox;
class QMessageBox;

/**
 * Dialog for moving files.
 *
 * @see http://lists.trolltech.com/qt-interest/2003-05/thread01274-0.html
 */
class FMoveDialog : public QDialog
{
	Q_OBJECT
public:
	/**
	 * Creates a move dialog for deleting files.
	 *
	 * @param files			list of files which are to be moved
	 * @param destination	destination directory
	 * @param parent		parent widget
	 */
	FMoveDialog(const QList<QFileInfo>& files, const QString& destination, QWidget* parent = 0);
	virtual ~FMoveDialog();
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void createGui();

	/**
	 * Sets up a progress bar.
	 */
	void setupBoxProgress(QGroupBox* box);

	/**
	 * Sets up a source and destination descriptions in dialog.
	 */
	void setupBoxSrcDest(QGroupBox* box);

	/**
	 * Appends to a list all files from current directory.
	 *
	 * @param info current directory
	 * @param list list of files which is appended by files in specified directory
	 */
	void fileList(const QFileInfo& info, QFileInfoList& list);

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
public:
	/**
	 * Overriden method for accepting the dialog and starting to move the files.
	 */
	virtual void accept();
private:
	QList<QFileInfo> files;
	QString destination;

//	QPushButton* ow_rename;
	QPushButton* ow_skip;
	QPushButton* ow_skip_all;
	QPushButton* ow_overwrite;
	QPushButton* ow_overwrite_all;

	FMgr::OverwriteMode overwrite_mode;
	QMessageBox* overwrite_qbox;

	QProgressBar* progress_bar;
	QLineEdit* source_path;
	QLineEdit* destination_path;
	QDialogButtonBox* buttons;
};

#endif /* FMOVEDIALOG_H_ */
