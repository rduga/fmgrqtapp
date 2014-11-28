/*
 * FDeleteDialog.h
 *
 *  Created on: Jan 13, 2010
 *      Author:
 */

#ifndef FDELETEDIALOG_H_
#define FDELETEDIALOG_H_

#include <QDialog>
#include <QList>
#include <QFileInfo>
#include <QThread>

class QLineEdit;
class QProgressBar;
class QDialogButtonBox;
class QGroupBox;

/**
 * Dialog for deleting files.
 *
 * @see http://lists.trolltech.com/qt-interest/2003-05/thread01274-0.html
 */
class FDeleteDialog : public QDialog
{
	Q_OBJECT
public:
	/**
	 * Creates a delete dialog for deleting files.
	 *
	 * @param files			list of files which are to be deleted
	 * @param parent		parent widget
	 */
	FDeleteDialog(const QList<QFileInfo>& files, QWidget* parent = 0);
	virtual ~FDeleteDialog();
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
	 * Clears the directory recursively.
	 *
	 * @param src_dir_path	directory which is to be cleared
	 */
	bool clearDir(const QString& src_dir_path);

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
public:
	/**
	 * Overriden method for accepting the dialog and starting to copy the files.
	 */
	virtual void accept();
private:
	QList<QFileInfo> files;

	QProgressBar* progress_bar;
	QLineEdit* source_path;
	QDialogButtonBox* buttons;
};

#endif /* FDELETEDIALOG_H_ */
