/***************************************************************************
 *   Copyright (C) 2011 by Viktoria Krolikowski                            *
 *   viktoria.krolikowsi@yahoo.de                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, version 2 of the License.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include <QString>

#include "mainwindow.h"

int main ( int argc, char *argv[] )
{
	Q_INIT_RESOURCE ( application );
	QApplication app ( argc, argv );

	QTranslator* translator = new QTranslator ();
	QString locale = QLocale::system().name();
	QString currentdir=QDir::currentPath();
	QDir::setCurrent ( QApplication::applicationDirPath() );
	translator->load ( "qbachelor_"+locale );
	QDir::setCurrent ( currentdir );
	app.installTranslator ( translator );

	MainWindow* mw = new MainWindow();
	mw->setTranslator ( translator );

	if ( argc > 1 )
	{
		QFile file ( argv[argc-1] );
		if ( file.exists() )
		{
			mw->loadFile ( file );
		}
	}

	mw->show();
	int result = app.exec();
	delete translator;
	return result;
}
