/*
 * FInclude.h
 *
 *  Created on: Dec 23, 2009
 *      Author:
 */

#ifndef FINCLUDE_H_
#define FINCLUDE_H_

namespace FMgr {

/**
 * Application shortcuts.
 */
enum Shortcuts
{
	F1_KEY = 1,
	F2_KEY,
	F3_KEY,
	F4_KEY,
	F5_KEY,
	F6_KEY,
	F7_KEY,
	F8_KEY,
	F9_KEY,
	F10_KEY = 10,

	HELP = 11,
	RIGHTS,
	VIEW,
	EDIT,
	COPY,
	RENMOV,
	MKDIR,
	DELETE,
	NONE,
	QUIT = 20,

	REFRESH,
	FILTER,
	REVERSE_SELECT
};

/**
 * Open mode of FTextEditor.
 */
enum OpenMode
{
	VIEWMODE,
	EDITMODE
};

/**
 * Zoom mode of FImageViewer.
 */
enum ZoomMode
{
	ZOOM_IN,
	ZOOM_OUT
};

/**
 * Overwrite mode of FCopyDialog and FMoveDialog when copying or moving files.
 */
enum OverwriteMode
{
	OW_UNKNOWN,
	OW_SKIP,
	OW_SKIP_ALL,
	OW_OVERWRITE,
	OW_OVERWRITE_ALL
};

} // End of namespace FMgr

#endif /* FINCLUDE_H_ */
