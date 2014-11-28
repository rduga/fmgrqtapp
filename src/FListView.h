/*
 * FListView.h
 *
 *  Created on: Dec 11, 2009
 *      Author:
 */

#ifndef FLISTVIEW_H_
#define FLISTVIEW_H_

#include <QTreeView>
#include <QWidget>
#include <QModelIndex>
#include <QFileInfo>
#include <QList>
#include <QFileInfo>

class QKeyEvent;
class QFocusEvent;
class QDirModel;

/**
 * List of files using QTreeView widget.
 *
 * This is the view of QDirModel using Model/View Programming of Qt framework.
 */
class FListView : public QTreeView
{
	Q_OBJECT
public:
	/**
	 * Creates a new list widget with current directory set to home directory.
	 *
	 * @param parent		parent widget
	 */
	FListView(QWidget* parent = 0);
	virtual ~FListView();

	/**
	 * @return	current directory/path set up in this list.
	 */
	QString currPath() const { return curr_path; }

	/**
	 * @return	current directory/path fileinfo set up in this list.
	 */
	QFileInfo currFileInfo() const;

	/**
	 * @return	selected list of QFileInfo items.
	 */
	QList<QFileInfo> selectedFileInfos();
signals:
	/**
	 * Emitted when this list gains the focus.
	 */
	void focusedIn();

	/**
	 * Emitted when current path has changed.
	 */
	void currPathChanged(const QString& path);
public slots:
	/**
	 * Changes the current path without emitting the FListView::currPathChanged(const QString&) signal.
	 */
	bool changeCurrPath(const QString& path);
private:
	/**
	 * Sets up the QDirModel model of this view.
	 */
	void setupModel();

	/**
	 * Creates the basic gui of the widget.
	 */
	void setupGui();

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
private slots:
	/**
	 * Executes the current index of list or specified index.
	 *
	 * @param index	specified index
	 */
	void execFile(const QModelIndex& index = QModelIndex());
public slots:
	/**
	 * Ups the directory.
	 */
	void upDir();

	/**
	 * Refreshes the file list.
	 */
	void refresh();

	/**
	 * Makes the new directory in current directory.
	 */
	bool mkDir(const QString& new_dir);

	/**
	 * Renames the current index file.
	 */
	void rename();
protected:
	/**
	 * Overriden method, emits the FListView::focusedIn() signal.
	 */
	virtual void focusInEvent(QFocusEvent * event);

	/**
	 * Handles ENTER (exec current index), BACKSPACE (cdups the current dir) and F2 (renames the current index).
	 */
	virtual void keyPressEvent(QKeyEvent * event);

	/**
	 * Handles double click as exec current index.
	 */
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
private:
	QDirModel* listmodel;
	QString curr_path;
};

#endif /* FLISTVIEW_H_ */
