/*
 * FListPanel.h
 *
 *  Created on: Dec 25, 2009
 *      Author:
 */

#ifndef FLISTPANEL_H_
#define FLISTPANEL_H_

#include "FListView.h"

#include <QWidget>

class QLineEdit;
class QComboBox;
class QAction;

/**
 * Widget which acts as container which groups the widgets associated to one list.
 *
 * It contains line edit with current path, refresh button, action for cd-uping,
 * combobox with drives/devices and list of files.
 */
class FListPanel : public QWidget
{
	Q_OBJECT
public:
	FListPanel(QWidget * parent = 0);
	virtual ~FListPanel();

	FListView* listView() { return list_view; }
public slots:
	/**
	 * Refreshes the FListView widget.
	 */
	void refresh();
private slots:
	/**
	 * Called after finished editing line edit to set the current path of FListPanel::list_view.
	 */
	void processEditingFinished();

	/**
	 * Changes the drive/device accoring to the string value (emitted from FListPanel::curr_path)
	 */
	void changeDevice(const QString& device);
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void creteGui();

	/**
	 * Refreshes drives/devices in FListPanel::devices.
	 */
	void refreshDevices();

	/**
	 * Sets up the focus policy for the right TAB focus functionality.
	 */
	void setFocusPolicies();

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
private:
	// TODO
	QAction* action_cdup;
	QAction* action_refresh;
	QLineEdit* curr_path;
	QComboBox* devices;
	FListView* list_view;
};

#endif /* FLISTPANEL_H_ */
