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

#include "licensedialog.h"

#include <QFile>
#include <QTextStream>
#include <QTextEdit>

LicenseDialog::LicenseDialog ( QWidget* parent, Qt::WFlags fl )
		: QDialog ( parent, fl ), Ui::LicenseDialog()
{
	setupUi ( this );
	connect ( buttonClose, SIGNAL ( clicked() ), this, SLOT ( accept() ) );

	QFile file ( ":license.txt" );
	file.open ( QIODevice::ReadOnly );
	QTextStream textStream ( &file );
	license->setText ( textStream.readAll() );
	file.close ();
}

LicenseDialog::~LicenseDialog()
{
}
