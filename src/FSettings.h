/*
 * FSettings.h
 *
 *  Created on: Jan 12, 2010
 *      Author:
 */

#ifndef FSETTINGS_H_
#define FSETTINGS_H_

#include <QDialog>
#include <QAbstractButton>

class QGroupBox;
class QLineEdit;
class QDialogButtonBox;

/**
 * Creates a dialog for setting up the accepted extensions for viewing/editing in FTextEditor and viewing in FImageViewer.
 *
 * This dialog uses QSettings of the current application
 * and loads/stores file extension regexps for matching at occured view or edit action.
 */
class FSettings : public QDialog
{
	Q_OBJECT
public:
	static QString TEXTVIEW_REGEXP;
	static QString IMAGEVIEW_REGEXP;

	static QString TEXTEDIT_REGEXP;

	static QString DEFAULT_TEXTVIEW_REGEXP;
	static QString DEFAULT_IMAGEVIEW_REGEXP;

	static QString DEFAULT_TEXTEDIT_REGEXP;

	/**
	 * Creates a new settings dialog.
	 *
	 * @param parent		parent widget
	 */
	FSettings(QWidget* parent = 0);
	virtual ~FSettings();
private:
	/**
	 * Creates the basic gui of the widget.
	 */
	void createGui();

	/**
	 * Setups input edits and loads regexps from settings values.
	 */
	void setupGui();

	/**
	 * Sets up the view box of gui.
	 */
	void setupViewBox(QGroupBox* box);

	/**
	 * Sets up the edit box of gui.
	 */
	void setupEditBox(QGroupBox* box);

	/**
	 * Sets up the exec box of gui.
	 *
	 * This box is empty box.
	 */
	void setupExecBox(QGroupBox* box);

	/**
	 * Connects slots and signals of this widget.
	 */
	void connectSignals();
public:
	/**
	 * Overloaded method, when accepting, saves the regexp values into settings of current application.
	 */
	virtual void accept();
private:
	QLineEdit* view_text_input;
	QLineEdit* view_image_input;

	QLineEdit* edit_text_input;

	QDialogButtonBox* buttons;
};

#endif /* FSETTINGS_H_ */
