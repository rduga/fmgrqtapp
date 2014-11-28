/*
 * FPermissionDialog.h
 *
 *  Created on: Jan 6, 2010
 *      Author:
 */

#ifndef FPERMISSIONDIALOG_H_
#define FPERMISSIONDIALOG_H_

#include <QDialog>
#include <QFile>

class QCheckBox;

/**
 * Dialog for viewing/setting access rights for specified file.
 */
class FPermissionDialog : public QDialog
{
	Q_OBJECT
public:
	/**
	 * Creates a dialog with permissions of file of specified file path.
	 *
	 * @param file_path		path of file which are to change access rights
	 * @param parent		parent widget
	 */
	FPermissionDialog(const QString& file_path, QWidget* parent = 0);
	virtual ~FPermissionDialog();

	QFile::Permissions filePermissions();
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void createGui();

	/**
	 * Inits permission checkboxes according to the access rights of current file path.
	 */
	void initPermissionBoxes();

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
private slots:
	/**
	 * Change permission according to state of sender checkbox and specified signal sender.
	 */
	void changePermissionState(int state);
private:
	QString file_path;

//	QCheckBox* owner_read;
//	QCheckBox* owner_write;
//	QCheckBox* owner_exec;

	QCheckBox* user_read;
	QCheckBox* user_write;
	QCheckBox* user_exec;

	QCheckBox* group_read;
	QCheckBox* group_write;
	QCheckBox* group_exec;

	QCheckBox* other_read;
	QCheckBox* other_write;
	QCheckBox* other_exec;
};

#endif /* FPERMISSIONDIALOG_H_ */
