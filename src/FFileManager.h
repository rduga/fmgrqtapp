/*
 * FFileManager.h
 *
 *  Created on: Dec 10, 2009
 *      Author:
 */

#ifndef FFILEMANAGER_H_
#define FFILEMANAGER_H_

#include "FInclude.h"
#include "FShortcutButtons.h"
#include "FListPanel.h"

#include <QMainWindow>
#include <QStringList>

class QAction;

/**
 * Application Main Window.
 *
 * This window handles the main actions of FileManager.
 */
class FFileManager : public QMainWindow
{
	Q_OBJECT
public:
	FFileManager();
	virtual ~FFileManager();
private slots:
	/**
	 * Refreshes both file lists.
	 */
	void refreshAll();

	/**
	 * Shows short info about this appication.
	 */
	void about();

	/**
	 * Shows short help and how to control the file manager.
	 */
	void help();

	/**
	 * Shows the current file in FTextEditor or FImageViewer.
	 */
	void view();

	/**
	 * Edits the current file inf FTextEditor.
	 */
	void edit();

	/**
	 * Copies the current selected files.
	 */
	void copy();

	/**
	 * Moves the current selected files.
	 */
	void renmov();

	/**
	 * Makes a new directory.
	 */
	void mkdir();

	/**
	 * Deletes the current selected files.
	 */
	void del();

	/**
	 * Views/changes the access of the current file.
	 */
	void access();

	/**
	 * Shows the settings dialog for FTextEditor and FImageViewer.
	 */
	void settings();

	/**
	 * Shows short info about Qt toolkit.
	 */
	void aboutQt();

	/**
	 * Quits the application.
	 */
	void quit();

	/**
	 * Processes the application main shortcuts (F1-F10)
	 */
	void processShortcut(FMgr::Shortcuts shortcut);

	/**
	 * Sets the current FFileManager::list_focus and FFileManager::list_notfocus.
	 */
	void processListFocus();
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void createGui();

	/**
	 * Creates toolbar with actions.
	 */
	void createToolBar();

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
protected:
	/**
	 * Overloaded method for catching the shortcuts of application.
	 */
	virtual void keyPressEvent(QKeyEvent* key_event);
private:
	QAction* action_refresh_all;
//	QAction* action_file_filter;
//	QAction* action_reverse_select;
	QAction* action_settings;
	QAction* action_about;
	QAction* action_qt;
	QAction* action_exit;

	FListPanel* panel_left;
	FListPanel* panel_right;

	FListView* list_focus;
	FListView* list_notfocus;

	FShortcutButtons* shortcut_buttons;
};

#endif /* FFILEMANAGER_H_ */
