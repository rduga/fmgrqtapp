/*
 * FTextEditor.cpp
 *
 *  Created on: Jan 6, 2010
 *      Author:
 */

#include "FTextEditor.h"

#include <QIcon>
#include <QToolBar>
#include <QMessageBox>
#include <QBoxLayout>
#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFile>
#include <QKeySequence>

FTextEditor::FTextEditor(const QString& file_path, FMgr::OpenMode open_mode /* = FMgr::VIEWMODE */,
		QWidget* parent /* = 0 */) : QDialog(parent),
	open_mode(open_mode),

	action_save(new QAction(QIcon(":/icons/actions/document-save.png"), tr("save"), this)),
	action_close(new QAction(QIcon(":/icons/actions/process-stop.png"), tr("close"), this)),

	editor(new QTextEdit(this)),

	file_path()
{
	createGui();
	openFile(file_path);
	connectSignals();
}

FTextEditor::~FTextEditor()
{
}

void FTextEditor::createGui()
{
	if (open_mode == FMgr::VIEWMODE) {
		setWindowTitle(tr("FTextViewer"));
	} else {
		setWindowTitle(tr("FTextEditor"));
	}
	setWindowIcon(QIcon(":/icons/actions/edit-find-replace.png"));

	QVBoxLayout* main_layout = new QVBoxLayout();

	// toolbar
	QToolBar* tool_bar = new QToolBar();

	action_save->setShortcut(QKeySequence(QKeySequence::Save));
	if (open_mode == FMgr::VIEWMODE) {
		action_save->setEnabled(false);
	}

	tool_bar->addAction(action_save);
	tool_bar->addAction(action_close);

	main_layout->addWidget(tool_bar);
	main_layout->addWidget(editor);

	setLayout(main_layout);
}

void FTextEditor::connectSignals()
{
	if (open_mode == FMgr::EDITMODE) {
		connect(action_save, SIGNAL(triggered()), this, SLOT(saveFile()));
	}
	connect(action_close, SIGNAL(triggered()), this, SLOT(close()));

	connect(editor->document(), SIGNAL(contentsChanged()), this, SLOT(textWasModified()));
}

bool FTextEditor::maybeSave()
{
	if (file_path.isEmpty()) {
		qDebug("No file opened");
		return true;
	}

	if (!editor->document()->isModified()) {
		qDebug("File has not been modified");
		return true;
	}

	int result = QMessageBox::warning(this, tr("Unsaved changes"),
			tr("File %1 has not been saved. "
					"Do you want to save changes or discard them?")
					.arg(file_path),
					QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
					QMessageBox::Save);

	if (result == QMessageBox::Save) {
		saveFile();
		return true;
	} else if (result == QMessageBox::Discard) {
		return true;
	} // else if (result == QMessageBox::Cancel)

	return false;
}

void FTextEditor::openFile(const QString& file_path)
{
	if (maybeSave()) {
		closeFile();
	} else {
		return;
	}

	this->file_path = file_path;

	QFile file(file_path);

	if (file.open(QIODevice::ReadOnly)) {
		editor->setReadOnly(open_mode == FMgr::VIEWMODE);

		QString text = editor->toPlainText();
		QTextStream istream(&file);

		editor->setPlainText(istream.readAll());
		file.close();

		if (open_mode == FMgr::VIEWMODE) {
			setWindowTitle(tr("FTextViewer - %1 [*]").arg(file_path));
		} else {
			setWindowTitle(tr("FTextEditor - %1 [*]").arg(file_path));
		}

		return;
	}

	QMessageBox::critical(this, tr("Unable to read"),
		tr("You do not have the read permission to save the file %1.")
				.arg(file_path),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
	closeFile();
}

void FTextEditor::textWasModified()
{
	qDebug("text modified");
	setWindowModified(editor->document()->isModified());
}

void FTextEditor::saveFile()
{
	qDebug("saving file");
	QFile file(file_path);

	if (file.open(QIODevice::WriteOnly)) {
		QString text = editor->toPlainText();
		QTextStream ostream(&file);

		ostream << text;
		file.close();

		editor->document()->setModified(false);
		setWindowModified(false);
		return;
	}

	QMessageBox::critical(this, tr("Unable to save"),
		tr("You do not have the write permission to save file %1.")
				.arg(file_path),
				QMessageBox::Cancel,
				QMessageBox::Cancel);
}

void FTextEditor::closeFile()
{
	editor->clear();
	editor->setReadOnly(true);
	file_path = QString();
	setWindowTitle(tr("FTextEditor [*]"));
}

void FTextEditor::closeEvent(QCloseEvent* event)
{
	if (maybeSave()) {
		event->accept();
	} else {
		event->ignore();
	}
}
