/*
 * FFileManager.cpp
 *
 *  Created on: Dec 10, 2009
 *      Author:
 */

#include "FFileManager.h"
#include "FListPanel.h"
#include "FTextEditor.h"
#include "FImageViewer.h"
#include "FPermissionDialog.h"
#include "FSettings.h"
#include "FCopyDialog.h"
#include "FDeleteDialog.h"
#include "FMoveDialog.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QString>
#include <QKeyEvent>
#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>
#include <QSettings>
#include <QRegExp>

FFileManager::FFileManager() : QMainWindow(),
	action_refresh_all(new QAction(QIcon(":/icons/actions/view-refresh.png"), tr("refresh all"), this)),
//	action_file_filter(new QAction(QIcon(":/icons/filter.png"), tr("filter"), this)),
//	action_reverse_select(new QAction(QIcon(":/icons/actions/edit-redo.png"), tr("reverse select"), this)),
	action_settings(new QAction(QIcon(":/icons/actions/document-properties.png"), tr("settings"), this)),
	action_about(new QAction(QIcon(":/icons/actions/edit-find-replace.png"), tr("about FFileManager"), this)),
	action_qt(new QAction(QIcon(":/icons/qt.png"), tr("about qt"), this)),
	action_exit(new QAction(QIcon(":/icons/actions/process-stop.png"), tr("exit"), this)),

	panel_left(new FListPanel()),
	panel_right(new FListPanel()),

	list_focus(panel_left->listView()),
	list_notfocus(panel_right->listView()),

	shortcut_buttons(new FShortcutButtons())
{
	// TODO Auto-generated constructor stub
	createGui();

	panel_left->listView()->setFocus();

	connectSignals();
}

FFileManager::~FFileManager()
{
	// TODO Auto-generated destructor stub
}

void FFileManager::refreshAll()
{
	qDebug("refreshing All lists");
	panel_left->refresh();
	panel_right->refresh();
}

void FFileManager::about()
{
	FTextEditor editor(":/docs/about.txt", FMgr::VIEWMODE, this);
	editor.exec();
}

void FFileManager::help()
{
	FTextEditor editor(":/docs/help.txt", FMgr::VIEWMODE, this);
	editor.exec();
}

void FFileManager::view()
{
	QFileInfo curr_fileinfo = list_focus->currFileInfo();

	if (!curr_fileinfo.exists()) {
		qDebug("Nothing to View");
		return;
	}

	if (!curr_fileinfo.isFile()) {
		qDebug("Current selected index for view is not a file");
		return;
	}

	QSettings stx;
	QRegExp regexp;
	regexp.setCaseSensitivity(Qt::CaseInsensitive);

	regexp.setPattern(stx.value(FSettings::IMAGEVIEW_REGEXP, QVariant(FSettings::DEFAULT_IMAGEVIEW_REGEXP)).toString());
	if (regexp.exactMatch(curr_fileinfo.suffix())) {
		FImageViewer viewer(curr_fileinfo.filePath(), this); // ":docs/image.jpg"
		viewer.exec();
		return;
	}

	regexp.setPattern(stx.value(FSettings::TEXTVIEW_REGEXP, QVariant(FSettings::DEFAULT_TEXTVIEW_REGEXP)).toString());
	if (regexp.exactMatch(curr_fileinfo.suffix())) {
		FTextEditor editor(curr_fileinfo.filePath(), FMgr::VIEWMODE, this); // ":/docs/about.txt"
		editor.exec();
		return;
	}

	QMessageBox::critical(this, tr("No viewer available"),
			tr("No viewer available for extension: %1").arg(curr_fileinfo.suffix()),
			QMessageBox::Cancel, QMessageBox::Cancel
			);
}

void FFileManager::edit()
{
	QFileInfo curr_fileinfo = list_focus->currFileInfo();

	if (!curr_fileinfo.exists()) {
		qDebug("Nothing to Edit");
		return;
	}

	if (!curr_fileinfo.isFile()) {
		qDebug("Current selected index for edit is not a file");
		return;
	}

	QSettings stx;
	QRegExp regexp;
	regexp.setCaseSensitivity(Qt::CaseInsensitive);

	regexp.setPattern(stx.value(FSettings::TEXTEDIT_REGEXP, QVariant(FSettings::DEFAULT_TEXTEDIT_REGEXP)).toString());
	if (regexp.exactMatch(curr_fileinfo.suffix())) {
		FTextEditor editor(curr_fileinfo.filePath(), FMgr::EDITMODE, this); // ":/docs/about.txt"
		editor.exec();
		return;
	}

	QMessageBox::critical(this, tr("No editor available"),
			tr("No editor available for extension: %1").arg(curr_fileinfo.suffix()),
			QMessageBox::Cancel, QMessageBox::Cancel
			);
}

void FFileManager::copy()
{
	QList<QFileInfo> curr_finfos = list_focus->selectedFileInfos();

	if (curr_finfos.empty()) {
		qDebug("no files selected for deleting");
		return;
	}

	FCopyDialog cdialog(curr_finfos, list_notfocus->currPath(), this);
	cdialog.exec();
	refreshAll();
}

void FFileManager::renmov()
{
	QList<QFileInfo> curr_finfos = list_focus->selectedFileInfos();

	if (curr_finfos.empty()) {
		qDebug("no files selected for deleting");
		return;
	}

	FMoveDialog mdialog(curr_finfos, list_notfocus->currPath(), this);
	mdialog.exec();
	refreshAll();

//	qDebug("renaming file %s", qPrintable(curr_finfo.fileName()));
//	list_focus->rename();
}

void FFileManager::mkdir()
{
	QString new_dir = QInputDialog::getText(this, tr("Make a new directory"), tr("Enter new directory name"),
			QLineEdit::Normal, tr("newdir"));

	if (!list_focus->mkDir(new_dir)) {
		QMessageBox::critical(this, tr("Unable to create the directory"),
				tr("Directory was unable to create with name: %1").arg(new_dir),
				QMessageBox::Cancel, QMessageBox::Cancel
				);
	}
}

void FFileManager::del()
{
	QList<QFileInfo> curr_finfos = list_focus->selectedFileInfos();

	if (curr_finfos.empty()) {
		qDebug("no files selected for deleting");
		return;
	}

	FDeleteDialog ddialog(curr_finfos, this);
	ddialog.exec();

	refreshAll();

//	qDebug("deleting file: %s not visually", qPrintable(curr_finfo.filePath()));
//
//	int result = QMessageBox::warning(this, tr("Delete the file"),
//			tr("Do you want to really delete the selected files?"),
//			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes
//			);
//
//	if (result == QMessageBox::Yes) {
//		// TODO
//	}
}

void FFileManager::access()
{
	QFileInfo curr_fileinfo = list_focus->currFileInfo();

	if (!curr_fileinfo.exists()) {
		qDebug("Nothing to Change The Rights");
		return;
	}

	FPermissionDialog perms(curr_fileinfo.filePath());
	perms.exec();
}

void FFileManager::settings()
{
	FSettings stx;
	stx.exec();
}

void FFileManager::aboutQt()
{
	qApp->aboutQt();
}

void FFileManager::quit()
{
	int val = QMessageBox::question(this, tr("quit"),
			tr("Do you really want to quit?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes);

	if (val == QMessageBox::Yes) {
		qApp->closeAllWindows();
	}
}

void FFileManager::processShortcut(FMgr::Shortcuts shortcut)
{
	qDebug("Processing shortcut: %d", shortcut);

	switch (shortcut) {
		case FMgr::F1_KEY:
		case FMgr::HELP:
			help();
			break;
		case FMgr::F2_KEY:
			list_focus->rename();
			break;
		case FMgr::F3_KEY:
		case FMgr::VIEW:
			view();
			break;
		case FMgr::F4_KEY:
		case FMgr::EDIT:
			edit();
			break;
		case FMgr::F5_KEY:
		case FMgr::COPY:
			copy();
			break;
		case FMgr::F6_KEY:
		case FMgr::RENMOV:
			renmov();
			break;
		case FMgr::F7_KEY:
		case FMgr::MKDIR:
			mkdir();
			break;
		case FMgr::F8_KEY:
		case FMgr::DELETE:
			del();
			break;
		case FMgr::F9_KEY:
		case FMgr::RIGHTS:
			access();
			break;
		case FMgr::F10_KEY:
		case FMgr::QUIT:
			quit();
			break;
		case FMgr::REFRESH:
		case FMgr::FILTER:
		case FMgr::REVERSE_SELECT:
		default:
			Q_ASSERT_X(false, "processShortcut", "unknown type of shortcut");
			break;
	}
}

void FFileManager::processListFocus()
{
	FListView* list_sender = qobject_cast<FListView*>(sender());

	if (!list_sender) {
		qDebug("Bad type of list focus sender");
		return;
	}

	list_focus = list_sender;

	if (list_sender == panel_left->listView()) {
		qDebug("Left list focused");
		list_notfocus = panel_right->listView();
	} else if (list_sender == panel_right->listView()) {
		qDebug("Right list focused");
		list_notfocus = panel_left->listView();
	} else {
		qDebug("Unknown list focus sender");
	}
}

void FFileManager::createGui()
{
	setWindowTitle(tr("FileManager"));
	setWindowIcon(QIcon(":/icons/actions/edit-find-replace.png"));

	createToolBar();

	QWidget* central = new QWidget();

	QVBoxLayout* main_layout = new QVBoxLayout();

	QHBoxLayout* lists_layout = new QHBoxLayout();
	lists_layout->addWidget(panel_left);
	lists_layout->addWidget(panel_right);

	main_layout->addLayout(lists_layout);
	main_layout->addWidget(shortcut_buttons);
	central->setLayout(main_layout);

	setCentralWidget(central);
}

void FFileManager::createToolBar()
{
	QToolBar* tool_bar = new QToolBar();

	tool_bar->addAction(action_refresh_all);
//	tool_bar->addAction(action_file_filter);
//	tool_bar->addAction(action_reverse_select);

//	tool_bar->addSeparator();
	tool_bar->addAction(action_settings);
	tool_bar->addSeparator();

	tool_bar->addAction(action_about);
	tool_bar->addAction(action_qt);
	tool_bar->addSeparator();
	tool_bar->addAction(action_exit);

	addToolBar(tool_bar);
}

void FFileManager::connectSignals()
{
	connect(action_refresh_all, SIGNAL(triggered()), this, SLOT(refreshAll()));
	// TODO connect action_file_filter action_reverse_select

	connect(action_settings, SIGNAL(triggered()), this, SLOT(settings()));

	connect(action_about, SIGNAL(triggered()), this, SLOT(about()));
	connect(action_qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(action_exit, SIGNAL(triggered()), this, SLOT(quit()));

	connect(panel_left->listView(), SIGNAL(focusedIn()), this, SLOT(processListFocus()));
	connect(panel_right->listView(), SIGNAL(focusedIn()), this, SLOT(processListFocus()));

	connect(shortcut_buttons, SIGNAL(shortcutPressed(FMgr::Shortcuts)), this, SLOT(processShortcut(FMgr::Shortcuts )));
}

void FFileManager::keyPressEvent(QKeyEvent* key_event)
{
	if (key_event->modifiers() != Qt::NoModifier) {
		QMainWindow::keyPressEvent(key_event);
		return;
	}

	switch (key_event->key()) {
		case Qt::Key_F1:
			processShortcut(FMgr::F1_KEY);
			break;
		case Qt::Key_F2:
			processShortcut(FMgr::F2_KEY);
			break;
		case Qt::Key_F3:
			processShortcut(FMgr::F3_KEY);
			break;
		case Qt::Key_F4:
			processShortcut(FMgr::F4_KEY);
			break;
		case Qt::Key_F5:
			processShortcut(FMgr::F5_KEY);
			break;
		case Qt::Key_F6:
			processShortcut(FMgr::F6_KEY);
			break;
		case Qt::Key_F7:
			processShortcut(FMgr::F7_KEY);
			break;
		case Qt::Key_F8:
			processShortcut(FMgr::F8_KEY);
			break;
		case Qt::Key_F9:
			processShortcut(FMgr::F9_KEY);
			break;
		case Qt::Key_F10:
			processShortcut(FMgr::F10_KEY);
			break;
		default:
			QMainWindow::keyPressEvent(key_event);
			return;
			break;
	}

	key_event->accept();
}
