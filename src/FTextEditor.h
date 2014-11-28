/*
 * FTextEditor.h
 *
 *  Created on: Jan 6, 2010
 *      Author:
 */

#ifndef FTEXTEDITOR_H_
#define FTEXTEDITOR_H_

#include "FInclude.h"

#include <QDialog>
#include <QAction>
#include <QString>

class QTextEdit;

/**
 * Dialog for viewing/editing text files.
 *
 * This dialog uses QTextEdit widget for viewing/editing.
 */
class FTextEditor : public QDialog
{
	Q_OBJECT
public:
	/**
	 * Creates a new dialog with specified text source.
	 *
	 * @param file_path		source of text file
	 * @param open_mode		FMgr::VIEWMODE or FMgr::EDITMODE
	 * @param parent		parent widget
	 */
	FTextEditor(const QString& file_path, FMgr::OpenMode open_mode = FMgr::VIEWMODE, QWidget* parent = 0);
	virtual ~FTextEditor();
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void createGui();

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();

	bool maybeSave();
private slots:
	void textWasModified();
	void openFile(const QString& file_path);
	void saveFile();
	void closeFile();
protected:
	virtual void closeEvent(QCloseEvent* event );
private:
	FMgr::OpenMode open_mode;

	QAction* action_save;
	QAction* action_close;

	QTextEdit* editor;

	QString file_path;
};

#endif /* FTEXTEDITOR_H_ */
