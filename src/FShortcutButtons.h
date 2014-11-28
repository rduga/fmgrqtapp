/*
 * FShortcutButtons.h
 *
 *  Created on: Dec 23, 2009
 *      Author:
 */

#ifndef FSHORTCUTBUTTONS_H_
#define FSHORTCUTBUTTONS_H_

#include "FInclude.h"

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>

/**
 * Widget with buttons which are on the bottom of the FileManager.
 *
 * After clicking the specified button the signal with specified shortcut
 * is emited (FMgr::Shortcuts)
 */
class FShortcutButtons :public QWidget
{
	Q_OBJECT
public:
	/**
	 * Creates a new FShortcutButtons widget.
	 *
	 * @param parent		parent widget
	 */
	FShortcutButtons(QWidget* parent = 0);
	virtual ~FShortcutButtons();
private slots:
	/**
	 * This slot emits FShortcutButtons::shortcutPressed(FMgr::Shortcuts) signal.
	 *
	 * @param int_shortcut	FMgr::Shortcuts in integer value
	 */
	void shortcutPressedPriv(int int_shortcut);
signals:
	/**
	 * This signal is emitted if some kind of button is pressed.
	 */
	void shortcutPressed(FMgr::Shortcuts shortcut);
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void createGui();

	/**
	 * Create mappings of files and FMgr::Shortcuts int values.
	 */
	void createMappings();

	/**
	 * Sets up focus policies of buttons (for the right TAB focusing).
	 */
	void setFocusPolicies();
private:
	QPushButton* button_f1;
	QPushButton* button_f2;
	QPushButton* button_f3;
	QPushButton* button_f4;
	QPushButton* button_f5;
	QPushButton* button_f6;
	QPushButton* button_f7;
	QPushButton* button_f8;
	QPushButton* button_f9;
	QPushButton* button_f10;

	QSignalMapper* mapper;
};

#endif /* FSHORTCUTBUTTONS_H_ */
