/*
 * FListView.cpp
 *
 *  Created on: Dec 11, 2009
 *      Author:
 */

#include "FListView.h"

#include <QDirModel>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>

FListView::FListView(QWidget* parent /* = 0 */) : QTreeView(parent),
	listmodel(new QDirModel(this)),
	curr_path(QDir::homePath())
{
	setupModel();
	setupGui();

	setRootIndex(listmodel->index(curr_path));
	setCurrentIndex(listmodel->index(0,0, rootIndex()));
}

FListView::~FListView()
{
}

QFileInfo FListView::currFileInfo() const
{
//	setCurrentIndex();
	return listmodel->fileInfo(selectionModel()->currentIndex());
}

QList<QFileInfo> FListView::selectedFileInfos()
{
	QModelIndexList selected_indexes = selectionModel()->selectedRows();

	QList<QFileInfo> fileinfo_list;

	foreach(QModelIndex sindex, selected_indexes) {
		fileinfo_list.append(listmodel->fileInfo(sindex));
	}

	return fileinfo_list;
}

bool FListView::changeCurrPath(const QString& path)
{
	QFileInfo file_info(path);

	if (!file_info.exists()) {
		QMessageBox::warning(this, tr("Path does not exist"),
				tr("The directory you have typed does not exist."),
				QMessageBox::Cancel, QMessageBox::Cancel);
		return false;
	}

	if (!file_info.isDir()) {
		QMessageBox::warning(this, tr("Path is not a directory"),
				tr("The path you have typed is not a directory."),
				QMessageBox::Cancel, QMessageBox::Cancel);
		return false;
	}

	curr_path = file_info.filePath();
	setRootIndex(listmodel->index(curr_path));
	setCurrentIndex(listmodel->index(0,0, rootIndex()));

	return true;
}

void FListView::setupModel()
{
	listmodel->setResolveSymlinks(false);

	QDir::Filters s = QDir::NoDotAndDotDot;
	s = ~s;
	s |= QDir::AllEntries;

	listmodel->setFilter(s);
	listmodel->setReadOnly(false);

	setModel(listmodel);
}

void FListView::setupGui()
{
	setHeaderHidden(false);
	setRootIsDecorated(false);
	setItemsExpandable(false);
	setSortingEnabled(true);
	sortByColumn(0, Qt::AscendingOrder);

	// selection
	setSelectionMode(QAbstractItemView::ExtendedSelection);
//	setSelectionMode(QAbstractItemView::SingleSelection);

//	listmodel->setRootPath(QDir::currentPath());
}

void FListView::connectSignals()
{
	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(execFile(const QModelIndex&)));
}

void FListView::execFile(const QModelIndex& index /* = QModelIndex() */)
{
	qDebug("exec file start");
	QFileInfo finfo;

	if (index.isValid()) {
		finfo = listmodel->fileInfo(index);
	} else {
		finfo = listmodel->fileInfo(selectionModel()->currentIndex());
	}

	if (finfo.isDir()) {
		if (!finfo.isReadable()) {

			QMessageBox::critical(this, tr("Unable to read the directory"),
							tr("The directory you are trying to read does not have the read right."),
							QMessageBox::Cancel, QMessageBox::Cancel);

			qDebug("unable to read the directory - missing read permission");
			return;
		}

		if (!finfo.isExecutable()) {

			QMessageBox::critical(this, tr("Unable to execute the directory"),
							tr("The directory you are trying to execute does not have the exec right."),
							QMessageBox::Cancel, QMessageBox::Cancel);

			qDebug("unable to exec the directory - missing exec permission");
			return;
		}

		qDebug("exec directory");
		setRootIndex(selectionModel()->currentIndex());
		setCurrentIndex(listmodel->index(0,0, rootIndex()));
//		selectionModel()->setCurrentIndex(listmodel->index(0, 0, rootIndex()), QItemSelectionModel::Current);

		qDebug("emiting path changed");
		curr_path = finfo.canonicalFilePath(); //filePath();
		emit currPathChanged(curr_path);

		return;
	}

	qDebug("exec file");
	// TODO: exec file
}

void FListView::upDir()
{
//	QFileInfo curr_info = currentIndex()
	QFileInfo finfo = listmodel->fileInfo(rootIndex());

	qDebug("Up Dir");

	if (finfo.isDir()) {
		QDir path(finfo.filePath());

		if (finfo.isRoot()) {
			qDebug("Unable to Up Dir - root path");
		} else if (!path.cdUp()) {
			qDebug("Unable to Up Dir - maybe permissions problem");
		}

		curr_path = finfo.absolutePath(); // canonicalFilePath();
		setRootIndex(listmodel->index(curr_path));

		qDebug("UpDir, setting curr index at: %s", qPrintable(finfo.absoluteFilePath()));



		qDebug("emiting path changed");
		emit currPathChanged(curr_path);

		// TODO: permissions

		if (finfo.isRoot()) {
			setCurrentIndex(listmodel->index(0,0, rootIndex()));
		} else {
			setCurrentIndex(listmodel->index(finfo.absoluteFilePath()));
		}

//		selectionModel()->setCurrentIndex(listmodel->index(0, 0, rootIndex()), QItemSelectionModel::Current);
	}
}

void FListView::refresh()
{
	qDebug("refreshing directory model");
//	QFileInfo currinfo = listmodel->fileInfo(currentIndex());

	listmodel->refresh(rootIndex());
	setCurrentIndex(listmodel->index(0, 0, rootIndex()));
}

bool FListView::mkDir(const QString& new_dir)
{
	qDebug("making new directory: %s", qPrintable(new_dir));

	QModelIndex dir_index = listmodel->mkdir(rootIndex(), new_dir);

	if (dir_index.isValid()) {
		setCurrentIndex(dir_index);
	}

	return dir_index.isValid();
}

void FListView::rename()
{
	if (!currentIndex().isValid()) {
		qDebug("No file selected for renaming");
	}

	QFileInfo curr = listmodel->fileInfo(currentIndex());

	bool ok;
    QString text = QInputDialog::getText(this, tr("Renaming file"),
    		tr("Renaming file: %1").arg(curr.fileName()), QLineEdit::Normal,
    		curr.fileName(), &ok);

    if (!ok || text.isEmpty()) {
    	return;
    }

    qDebug("Rename file");
    QFile curr_file(curr.absoluteFilePath());

    if (!curr.dir().rename(curr.fileName(), text)) {
		QMessageBox::critical(this, tr("Unable to rename the file"),
						tr("Unable to rename the file."),
						QMessageBox::Cancel, QMessageBox::Cancel);
		return;
    }

    refresh();

//	edit(currentIndex());
}

void FListView::focusInEvent(QFocusEvent * event)
{
	QTreeView::focusInEvent(event);

	emit focusedIn();
}

void FListView::keyPressEvent(QKeyEvent* event)
{
	if (event->modifiers() != Qt::NoModifier) {
		qDebug("Some modifiers were on");
		return;
	}

	switch (event->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
			qDebug("Enter pressed");
			execFile();
			return;
			break;
		case Qt::Key_Backspace:
			qDebug("Backspace pressed");
			upDir();
			return;
		case Qt::Key_F2:
			qDebug("F2 pressed");
			rename();
			return;
		case Qt::Key_Insert:
		default:
			break;
	}

	QTreeView::keyPressEvent(event);
}

void FListView::mouseDoubleClickEvent(QMouseEvent* event)
{
	qDebug("Double clicked - editing");

	QModelIndex index = indexAt(event->pos());
	execFile(index);
	event->accept();
}
