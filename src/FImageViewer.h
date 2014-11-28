/*
 * FImageViewer.h
 *
 *  Created on: Jan 9, 2010
 *      Author:
 */

#ifndef FIMAGEVIEWER_H_
#define FIMAGEVIEWER_H_

#include "FInclude.h"

#include <QDialog>
#include <QImage>

class QScrollArea;
class QStatusBar;
class QGraphicsView;
class QAction;
class QLabel;

/**
 * Dialog for viewing images.
 *
 * This dialog can zoom in, zoom out and zoom original the specified image
 * using The Graphics View Framework of Qt Toolkit.
 */
class FImageViewer : public QDialog
{
	Q_OBJECT
public:
	static const double zoom_step;

	static const double min_zoom_factor;
	static const double max_zoom_factor;

	/**
	 * Creates a new dialog with specified image source.
	 *
	 * @param image_path	source of image
	 * @param parent		parent widget
	 */
	FImageViewer(const QString& image_path, QWidget* parent = 0);
	virtual ~FImageViewer();
private slots:
	/**
	 * Slot for changing zoom accorging to object that sent the signal.
	 */
	void processZoom();
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void createGui();

	/**
	 * Setups gui, opens image, loads into scene and sets the basic info about it.
	 */
	void setupGui();

	/**
	 * Checks if the image with specified image path has the read access.
	 */
	bool checkPermissions();

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
private:
	QAction* action_zoom_in;
	QAction* action_zoom_out;
	QAction* action_zoom_original;
	QAction* action_close;

	QString image_path;
	QPixmap image;
//	QLabel* zoom_info;
	double zoom_factor;
	QGraphicsView* area;
//	QScrollArea* area;
	QStatusBar* status;
};

#endif /* FIMAGEVIEWER_H_ */
