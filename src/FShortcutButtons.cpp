/*
 * FShortcutButtons.cpp
 *
 *  Created on: Dec 23, 2009
 *      Author:
 */

#include "FShortcutButtons.h"

#include <QHBoxLayout>


FShortcutButtons::FShortcutButtons(QWidget* parent /* = 0 */) : QWidget(parent),
		button_f1(new QPushButton("F1 Help", this)),
		button_f2(new QPushButton("F2 Rename", this)),
		button_f3(new QPushButton("F3 View", this)),
		button_f4(new QPushButton("F4 Edit", this)),
		button_f5(new QPushButton("F5 Copy", this)),
		button_f6(new QPushButton("F6 RenMov", this)),
		button_f7(new QPushButton("F7 Mkdir", this)),
		button_f8(new QPushButton("F8 Delete", this)),
		button_f9(new QPushButton("F9 Access", this)),
		button_f10(new QPushButton("F10 Quit", this)),

		mapper(new QSignalMapper(this))
{
	createGui();
	createMappings();
}

FShortcutButtons::~FShortcutButtons()
{
	// TODO Auto-generated destructor stub
}

void FShortcutButtons::shortcutPressedPriv(int int_shortcut)
{
	FMgr::Shortcuts shortcut = static_cast<FMgr::Shortcuts>(int_shortcut);

	qDebug("Shortcut Button pressed: %d", shortcut);
	emit shortcutPressed(shortcut);
}

void FShortcutButtons::createGui()
{
	QHBoxLayout* layout = new QHBoxLayout(this);

	layout->addWidget(button_f1);
	layout->addWidget(button_f2);
	layout->addWidget(button_f3);
	layout->addWidget(button_f4);
	layout->addWidget(button_f5);
	layout->addWidget(button_f6);
	layout->addWidget(button_f7);
	layout->addWidget(button_f8);
	layout->addWidget(button_f9);
	layout->addWidget(button_f10);

	setFocusPolicies();
}

void FShortcutButtons::createMappings()
{
	mapper->setMapping(button_f1, FMgr::F1_KEY);
	mapper->setMapping(button_f2, FMgr::F2_KEY);
	mapper->setMapping(button_f3, FMgr::F3_KEY);
	mapper->setMapping(button_f4, FMgr::F4_KEY);
	mapper->setMapping(button_f5, FMgr::F5_KEY);
	mapper->setMapping(button_f6, FMgr::F6_KEY);
	mapper->setMapping(button_f7, FMgr::F7_KEY);
	mapper->setMapping(button_f8, FMgr::F8_KEY);
	mapper->setMapping(button_f9, FMgr::F9_KEY);
	mapper->setMapping(button_f10, FMgr::F10_KEY);

	connect(button_f1, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f2, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f3, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f4, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f5, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f6, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f7, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f8, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f9, SIGNAL(clicked()), mapper, SLOT(map()));
	connect(button_f10, SIGNAL(clicked()), mapper, SLOT(map()));

	connect(mapper, SIGNAL(mapped(int)), this, SLOT(shortcutPressedPriv(int)));
}

void FShortcutButtons::setFocusPolicies()
{
	button_f1->setFocusPolicy(Qt::NoFocus);
	button_f2->setFocusPolicy(Qt::NoFocus);
	button_f3->setFocusPolicy(Qt::NoFocus);
	button_f4->setFocusPolicy(Qt::NoFocus);
	button_f5->setFocusPolicy(Qt::NoFocus);
	button_f6->setFocusPolicy(Qt::NoFocus);
	button_f7->setFocusPolicy(Qt::NoFocus);
	button_f8->setFocusPolicy(Qt::NoFocus);
	button_f9->setFocusPolicy(Qt::NoFocus);
	button_f10->setFocusPolicy(Qt::NoFocus);
}
