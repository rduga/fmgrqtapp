#include "FFileManager.h"

#include <QtGui>
#include <QApplication>

/**
 * @mainpage FileManager
 *
 * @section about About this FileManager
 * FileManager is a simple file manager similar to TotalCommander
 * which is written using Qt toolkit library. This library is platform
 * independent so you can use this manager on windows, linux and other
 * platforms which Qt supports.
 *
 * @section requirements Compilation Requirements
 * Program was written and tested using <a href="http://qt.nokia.com/">Qt toolkit library 4.5.3</a> so
 * this version is recommended also for user to compile.
 * Usually the newer or older versions of this library may work
 * but they were not tested.
 *
 * @section features Main Features
 * The main features of this file manager are the following:
 * - Show help (F1)
 * - Rename the file (F2)
 * - View the file (using FTextEditor or FImageViewer, extensions configurable in settings) (F3)
 * - Edit the file (using FTextEditor, extensions configurable in settings) (F4)
 * - Copy files (F5)
 * - Move files (F6)
 * - Make a new directory (F7)
 * - Delete files (F8)
 * - Show/Change access of file (F9)
 * - Quit the program (F10)
 *
 * Control by keyboard:
 * - Up, Down, Home, End, PgUp, PgDn - movement over focued list of files
 * - Backspace - up current directory
 * - Enter - execute and change current directory
 *
 * Next features:
 * - changing the current directory of list by typing into line edit and pressing enter
 * - changing drives in current list by selecting one of drives in combobox
 * - selecting mupltiple files by mouse (you can use SHIFT and CTRL combinations)
 * - Ctrl+S shortcut in FTextEditor for saving text file
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	a.setOrganizationName("FFileManagerOrg");
	a.setOrganizationDomain("FFileManagerDomain");

	a.setApplicationName("FFileManager");
	a.setApplicationVersion("1.0");

    FFileManager w;
    w.resize(500, 600);
    w.show();

    return a.exec();
}
