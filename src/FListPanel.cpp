/*
 * FListPanel.cpp
 *
 *  Created on: Dec 25, 2009
 *      Author:
 */

#include "FListPanel.h"

#include <QBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>
#include <QAction>
#include <QIcon>
#include <QDir>

FListPanel::FListPanel(QWidget * parent /* = 0 */) : QWidget(parent),
	action_cdup(new QAction(QIcon(":/icons/actions/go-up.png"), tr("Open the parent folder"), this)),
	action_refresh(new QAction(QIcon(":/icons/actions/view-refresh.png"), tr("Refresh the filelist"), this)),
	curr_path(new QLineEdit()),
	devices(new QComboBox()),
	list_view(new FListView())
{
	creteGui();
	refreshDevices();
	curr_path->setText(list_view->currPath());
	connectSignals();
}

FListPanel::~FListPanel()
{
}

void FListPanel::refresh()
{
	list_view->refresh();
//	refreshDevices();
}

void FListPanel::processEditingFinished()
{
	QString curr_path_str = list_view->currPath();

	if (list_view->changeCurrPath(curr_path->text())) {
		return;
	}

	curr_path->setText(curr_path_str);
}

void FListPanel::changeDevice(const QString& device)
{
	curr_path->setText(device);
	processEditingFinished();
//	list_view->changeCurrPath(device);
}

void FListPanel::creteGui()
{
	QVBoxLayout* mainLayout = new QVBoxLayout();

	QHBoxLayout* topLayout = new QHBoxLayout();

	QToolButton* button_cdup = new QToolButton();
	button_cdup->setFocusPolicy(Qt::ClickFocus);
	button_cdup->setDefaultAction(action_cdup);

	QToolButton* button_refresh = new QToolButton();
	button_refresh->setDefaultAction(action_refresh);
	button_refresh->setFocusPolicy(Qt::ClickFocus);

	topLayout->addWidget(button_cdup);
	topLayout->addWidget(button_refresh);
	topLayout->addWidget(curr_path);
	topLayout->addWidget(devices);

	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(list_view);

	setLayout(mainLayout);

	setFocusPolicies();
}

void FListPanel::refreshDevices()
{
	devices->clear();

	QFileInfoList drives = QDir::drives();

	foreach(QFileInfo drive, drives) {
		devices->addItem(drive.absoluteFilePath(), drive.absoluteFilePath());
	}
}

void FListPanel::setFocusPolicies()
{
	curr_path->setFocusPolicy(Qt::ClickFocus);
	devices->setFocusPolicy(Qt::ClickFocus);
}

void FListPanel::connectSignals()
{
	connect(action_cdup, SIGNAL(triggered()), list_view, SLOT(upDir()));
	connect(action_refresh, SIGNAL(triggered()), this, SLOT(refresh()));

	connect(curr_path, SIGNAL(editingFinished()), this, SLOT(processEditingFinished()));
	connect(list_view, SIGNAL(currPathChanged(const QString&)), curr_path, SLOT(setText(const QString&)));

	connect(devices, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(changeDevice(const QString&)));
}
