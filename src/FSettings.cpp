/*
 * FSettings.cpp
 *
 *  Created on: Jan 12, 2010
 *      Author:
 */

#include "FSettings.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSettings>

QString FSettings::TEXTVIEW_REGEXP("TEXTVIEW_REGEXP");
QString FSettings::IMAGEVIEW_REGEXP("IMAGEVIEW_REGEXP");

QString FSettings::TEXTEDIT_REGEXP("TEXTEDIT_REGEXP");

QString FSettings::DEFAULT_TEXTVIEW_REGEXP("(txt|log|conf|ini|sh|xml||h|cpp|c|java|cs)");
QString FSettings::DEFAULT_IMAGEVIEW_REGEXP("(jpg|png|bmp|xpm|xbm|tiff|tga)");
QString FSettings::DEFAULT_TEXTEDIT_REGEXP("(txt|log|conf|ini|sh|xml||h|cpp|c|java|cs)");

FSettings::FSettings(QWidget* parent /* = 0 */) : QDialog(parent),
	view_text_input(new QLineEdit()),
	view_image_input(new QLineEdit()),

	edit_text_input(new QLineEdit()),

	buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel))
{
	createGui();
	setupGui();
	connectSignals();
}

FSettings::~FSettings()
{

}

void FSettings::createGui()
{
	setWindowTitle(tr("FSettings"));

	QVBoxLayout* main_layout = new QVBoxLayout(this);

	QGroupBox* view_box = new QGroupBox(tr("View Settings"));
	QGroupBox* edit_box = new QGroupBox(tr("Edit Settings"));
//	QGroupBox* exec_box = new QGroupBox(tr("Exec Settings"));

	setupViewBox(view_box);
	setupEditBox(edit_box);
//	setupExecBox(exec_box);

	main_layout->addWidget(view_box);
	main_layout->addWidget(edit_box);
//	main_layout->addWidget(exec_box);

	main_layout->addWidget(buttons);
}

void FSettings::setupGui()
{
	QSettings stx;

	view_text_input->setText(stx.value(FSettings::TEXTVIEW_REGEXP, DEFAULT_TEXTVIEW_REGEXP).toString());
	view_image_input->setText(stx.value(FSettings::IMAGEVIEW_REGEXP, DEFAULT_IMAGEVIEW_REGEXP).toString());
	edit_text_input->setText(stx.value(FSettings::TEXTEDIT_REGEXP, DEFAULT_TEXTEDIT_REGEXP).toString());
}

void FSettings::setupViewBox(QGroupBox* box)
{
	QGridLayout* box_layout = new QGridLayout();

	QLabel* view_image_label = new QLabel(tr("RegExp of File Extension for view with FImageViewer:"));
	QLabel* view_text_label = new QLabel(tr("RegExp of File Extension for view with FTextViewer:"));

	box_layout->addWidget(view_image_label, 0, 0);
	box_layout->addWidget(view_image_input, 0, 1);

	box_layout->addWidget(view_text_label, 1, 0);
	box_layout->addWidget(view_text_input, 1, 1);

	box->setLayout(box_layout);
}

void FSettings::setupEditBox(QGroupBox* box)
{
	QGridLayout* box_layout = new QGridLayout();

	QLabel* edit_text_label = new QLabel(tr("RegExp of File Extension for edit with FTextEditor:"));

	box_layout->addWidget(edit_text_label, 0, 0);
	box_layout->addWidget(edit_text_input, 0, 1);

	box->setLayout(box_layout);
}

void FSettings::setupExecBox(QGroupBox* box)
{
	QVBoxLayout* box_layout = new QVBoxLayout();

	box->setLayout(box_layout);
}

void FSettings::connectSignals()
{
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

void FSettings::accept()
{
	qDebug("accepted settings");

	QSettings stx;
	stx.setValue(FSettings::TEXTVIEW_REGEXP, view_text_input->text());
	stx.setValue(FSettings::IMAGEVIEW_REGEXP, view_image_input->text());
	stx.setValue(FSettings::TEXTEDIT_REGEXP, edit_text_input->text());

	QDialog::accept();
}
