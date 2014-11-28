/*
 * FPermissionDialog.cpp
 *
 *  Created on: Jan 6, 2010
 *      Author:
 */

#include "FPermissionDialog.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QFile>
#include <QLabel>
#include <QFrame>
#include <QMessageBox>

FPermissionDialog::FPermissionDialog(const QString& file_path, QWidget* parent /* = 0 */) : QDialog(parent),
	file_path(file_path),

//	owner_read(new QCheckBox(tr("read"))),
//	owner_write(new QCheckBox(tr("write"))),
//	owner_exec(new QCheckBox(tr("exec"))),

	user_read(new QCheckBox(tr("read"))),
	user_write(new QCheckBox(tr("write"))),
	user_exec(new QCheckBox(tr("exec"))),

	group_read(new QCheckBox(tr("read"))),
	group_write(new QCheckBox(tr("write"))),
	group_exec(new QCheckBox(tr("exec"))),

	other_read(new QCheckBox(tr("read"))),
	other_write(new QCheckBox(tr("write"))),
	other_exec(new QCheckBox(tr("exec")))
{
	createGui();
	initPermissionBoxes();
	connectSignals();
}

FPermissionDialog::~FPermissionDialog()
{
}

QFile::Permissions FPermissionDialog::filePermissions()
{
	QFile file(file_path);
	return file.permissions();
}

void FPermissionDialog::createGui()
{
	setWindowTitle(tr("FPermissions for file %1").arg(file_path));

	QVBoxLayout* main_layout = new QVBoxLayout();

	QHBoxLayout* permissions_layout = new QHBoxLayout();

//	QFrame* owner_frame = new QFrame();
//	owner_frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
//	QVBoxLayout* owner_layout = new QVBoxLayout();
//	owner_frame->setLayout(owner_layout);
//
//	owner_layout->addWidget(new QLabel(tr("Owner rights")));
//	owner_layout->addWidget(owner_read);
//	owner_layout->addWidget(owner_write);
//	owner_layout->addWidget(owner_exec);

	QFrame* user_frame = new QFrame();
	user_frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	QVBoxLayout* user_layout = new QVBoxLayout();
	user_frame->setLayout(user_layout);

	user_layout->addWidget(new QLabel(tr("User rights")));
	user_layout->addWidget(user_read);
	user_layout->addWidget(user_write);
	user_layout->addWidget(user_exec);

	QFrame* group_frame = new QFrame();
	group_frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	QVBoxLayout* group_layout = new QVBoxLayout();
	group_frame->setLayout(group_layout);

	group_layout->addWidget(new QLabel(tr("Group rights")));
	group_layout->addWidget(group_read);
	group_layout->addWidget(group_write);
	group_layout->addWidget(group_exec);

	QFrame* other_frame = new QFrame();
	other_frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	QVBoxLayout* other_layout = new QVBoxLayout();
	other_frame->setLayout(other_layout);

	other_layout->addWidget(new QLabel(tr("Other rights")));
	other_layout->addWidget(other_read);
	other_layout->addWidget(other_write);
	other_layout->addWidget(other_exec);

//	permissions_layout->addWidget(owner_frame);
	permissions_layout->addWidget(user_frame);
	permissions_layout->addWidget(group_frame);
	permissions_layout->addWidget(other_frame);

	QDialogButtonBox* button_box = new QDialogButtonBox();

	main_layout->addLayout(permissions_layout);
	main_layout->addWidget(button_box);
	setLayout(main_layout);
}

void FPermissionDialog::initPermissionBoxes()
{
	QFile::Permissions ps_file_flags = filePermissions();

	// owner
//	if (ps_file_flags.testFlag(QFile::ReadOwner)) {
//		owner_read->setChecked(true);
//	}
//
//	if (ps_file_flags.testFlag(QFile::WriteOwner)) {
//		owner_write->setChecked(true);
//	}
//
//	if (ps_file_flags.testFlag(QFile::ExeOwner)) {
//		owner_exec->setChecked(true);
//	}

	// user
	if (ps_file_flags.testFlag(QFile::ReadUser)) {
		user_read->setChecked(true);
	}

	if (ps_file_flags.testFlag(QFile::WriteUser)) {
		user_write->setChecked(true);
	}

	if (ps_file_flags.testFlag(QFile::ExeUser)) {
		user_exec->setChecked(true);
	}

	// group
	if (ps_file_flags.testFlag(QFile::ReadGroup)) {
		group_read->setChecked(true);
	}

	if (ps_file_flags.testFlag(QFile::WriteGroup)) {
		group_write->setChecked(true);
	}

	if (ps_file_flags.testFlag(QFile::ExeGroup)) {
		group_exec->setChecked(true);
	}

	// others
	if (ps_file_flags.testFlag(QFile::ReadOther)) {
		other_read->setChecked(true);
	}

	if (ps_file_flags.testFlag(QFile::WriteOther)) {
		other_write->setChecked(true);
	}

	if (ps_file_flags.testFlag(QFile::ExeOther)) {
		other_exec->setChecked(true);
	}
}

void FPermissionDialog::connectSignals()
{
//	connect(owner_read, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
//	connect(owner_write, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
//	connect(owner_exec, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );

	connect(user_read, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
	connect(user_write, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
	connect(user_exec, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );

	connect(group_read, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
	connect(group_write, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
	connect(group_exec, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );

	connect(other_read, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
	connect(other_write, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
	connect(other_exec, SIGNAL(stateChanged(int)), this, SLOT(changePermissionState(int)) );
}

void FPermissionDialog::changePermissionState(int state)
{
	QCheckBox* sig_sender = qobject_cast<QCheckBox*>(sender());

	if (!sig_sender) {
		qDebug("sender is not checkbox");
		return;
	}

	QFile::Permissions ps_flags;

//	if (sig_sender == owner_read) {
//		ps_flags = QFile::ReadOwner;
//	} else if (sig_sender == owner_write) {
//		ps_flags = QFile::WriteOwner;
//	} else if (sig_sender == owner_exec) {
//		ps_flags = QFile::ExeOwner;
//	} else
	if (sig_sender == user_read) {
		ps_flags = QFile::ReadUser | QFile::ReadOwner;
	} else if (sig_sender == user_write) {
		ps_flags = QFile::WriteUser | QFile::WriteOwner;
	} else if (sig_sender == user_exec) {
		ps_flags = QFile::ExeUser | QFile::ExeOwner;
	} else if (sig_sender == group_read) {
		ps_flags = QFile::ReadGroup;
	} else if (sig_sender == group_write) {
		ps_flags = QFile::WriteGroup;
	} else if (sig_sender == group_exec) {
		ps_flags = QFile::ExeGroup;
	} else if (sig_sender == other_read) {
		ps_flags = QFile::ReadOther;
	} else if (sig_sender == other_write) {
		ps_flags = QFile::WriteOther;
	} else if (sig_sender == other_exec) {
		ps_flags = QFile::ExeOther;
	} else {
		qDebug("Unkown file permission signal sender");
		return;
	}

	QFile file(file_path);
	QFile::Permissions ps_file_flags = file.permissions();

	qDebug("Before permissions: 0x%X", (int)ps_file_flags);



	if (state == Qt::Unchecked) {
		ps_file_flags &= ~ps_flags;
	} else if (state == Qt::Checked) {
		ps_file_flags |= ps_flags;
	} else {
		qDebug("Bad state number of permission Checkbox");
		return;
	}

	qDebug("Trying to set permissions: 0x%X", (int)ps_file_flags);

	if (!file.setPermissions(ps_file_flags)) {
		QString failperm;

		if (state == Qt::Unchecked) {
			failperm = tr("Unable to unset the %1 permission of file %2");
		} else if (state == Qt::Checked) {
			failperm = tr("Unable to set the %1 permission of file %2");
		}

		QMessageBox::critical(this, tr("Unable to set/unset permission"),
				failperm.arg(sig_sender->text()).arg(file_path));

		sig_sender->setChecked(state == Qt::Unchecked);

	}

	qDebug("Permissions set: 0x%X", (int)ps_file_flags);

}
