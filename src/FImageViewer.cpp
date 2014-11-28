/*
 * FImageViewer.cpp
 *
 *  Created on: Jan 9, 2010
 *      Author:
 */

#include "FImageViewer.h"

#include <QVBoxLayout>
#include <QImage>
#include <QMessageBox>
#include <QScrollArea>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QAction>
#include <QIcon>
#include <QMatrix>
#include <QFile>

const double FImageViewer::zoom_step = 1.1;
const double FImageViewer::min_zoom_factor = 0.1;
const double FImageViewer::max_zoom_factor = 10;

FImageViewer::FImageViewer(const QString& image_path, QWidget* parent /* = 0 */) : QDialog(parent),
	action_zoom_in(new QAction(QIcon(":/icons/zoom-in.png"), tr("zoom in"), this)),
	action_zoom_out(new QAction(QIcon(":/icons/zoom-out.png"), tr("zoom out"), this)),
	action_zoom_original(new QAction(QIcon(":/icons/zoom-original.png"), tr("zoom original"), this)),
	action_close(new QAction(QIcon(":/icons/actions/process-stop.png"), tr("close"), this)),

	image_path(image_path),
	image(image_path),
//	zoom_info(new QLabel(tr("Zoom: 100%"))),
	zoom_factor(1),
//	area(new QScrollArea()),
	area(new QGraphicsView()),
	status(new QStatusBar())
{
	createGui();
	checkPermissions();
	setupGui();
	connectSignals();
}

FImageViewer::~FImageViewer()
{
}

void FImageViewer::processZoom()
{
	QAction* action_sender = qobject_cast<QAction*>(sender());

	if (!action_sender) {
		qDebug("Unknown type of zoom action sender");
		return;
	}

	double new_zoom_factor = zoom_factor;
	if (action_sender == action_zoom_in) {
		new_zoom_factor *= FImageViewer::zoom_step;
	} else if (action_sender == action_zoom_out) {
		new_zoom_factor /= FImageViewer::zoom_step;
	} else if (action_sender == action_zoom_original) {
		new_zoom_factor = 1;
	} else {
		qDebug("Unknown zoom action sender");
		return;
	}

	if (new_zoom_factor < FImageViewer::min_zoom_factor) {
		qDebug("zoom exceeded minimum zoom. setting up minimum zoom");
		new_zoom_factor = FImageViewer::min_zoom_factor;
	} else if (new_zoom_factor > FImageViewer::max_zoom_factor) {
		qDebug("zoom exceeded maximum zoom. setting up maximum zoom");
		new_zoom_factor = FImageViewer::max_zoom_factor;
	}

	// zoom has not changed (qFuzzyCompare for rounding errors)
	if (qFuzzyCompare(new_zoom_factor, zoom_factor)) {
		qDebug("zoom has not changed");
		return;
	}

	zoom_factor = new_zoom_factor;

	QMatrix oldMatrix = area->matrix();

	qDebug("setting up zoom");

	// setup zoom factor for scene area
	area->resetMatrix();
	area->translate(oldMatrix.dx(), oldMatrix.dy());
	area->scale(zoom_factor, zoom_factor);

	// setup zoom label
	int int_zoom_factor = zoom_factor * 100;

	qDebug("setting zoom: %d", int_zoom_factor);

	status->showMessage(tr("Zoom: %1%").arg(int_zoom_factor));
}

void FImageViewer::createGui()
{
	setWindowTitle(tr("FImage Viewer"));
	setWindowIcon(QIcon(":/icons/actions/edit-find-replace.png"));

	QVBoxLayout* main_layout = new QVBoxLayout();

	QToolBar* tool_bar = new QToolBar();

	tool_bar->addAction(action_zoom_in);
	tool_bar->addAction(action_zoom_out);
	tool_bar->addAction(action_zoom_original);
	tool_bar->addAction(action_close);

	main_layout->addWidget(tool_bar);
	main_layout->addWidget(area);
	main_layout->addWidget(status);

	setLayout(main_layout);
}

void FImageViewer::setupGui()
{
	QSize image_size = image.size();

//	if (!checkPermissions()) {
//		QMessageBox::critical(this, "Unable to read image",
//				"You have no permissions to read this image.", QMessageBox::Cancel, QMessageBox::Cancel);
//		return;
//	}

	if (image.isNull()) {
		QMessageBox::critical(this, "Unable to load image",
				"Loading of image failed.", QMessageBox::Cancel, QMessageBox::Cancel);
		return;
	}

	setWindowTitle(tr("FImage Viewer - %1").arg(image_path));

	status->addPermanentWidget(new QLabel(tr("Filename: %1").arg(image_path)));
	status->addPermanentWidget(new QLabel(tr("Image resolution:%1 x %2").arg(image_size.width()).arg(image_size.height())));
	status->showMessage(tr("Zoom: 100%"));

	QGraphicsScene* scene = new QGraphicsScene(this);

	QGraphicsPixmapItem* image_item = new QGraphicsPixmapItem(image, 0, scene);
	Q_UNUSED(image_item);

	area->setScene(scene);
}

bool FImageViewer::checkPermissions()
{
	return QFile(image_path).isReadable();
}

void FImageViewer::connectSignals()
{
	connect(action_zoom_in, SIGNAL(triggered()), this, SLOT(processZoom()));
	connect(action_zoom_out, SIGNAL(triggered()), this, SLOT(processZoom()));
	connect(action_zoom_original, SIGNAL(triggered()), this, SLOT(processZoom()));
	connect(action_close, SIGNAL(triggered()), this, SLOT(close()));
}
