/***************************************************************************
 *   Copyright (C) 2011 by Viktoria Krolikowski                            *
 *   viktoria.krolikowski@yahoo.de                                         *
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


#ifndef _STUDYCOURSE_H_
#define _STUDYCOURSE_H_

#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

typedef struct subject
{
	QString name;
	unsigned int grade;
	unsigned int cps;
	unsigned int sppw;
	unsigned int trials;
} Subject;

class StudyCourse
{
	public:
		StudyCourse();
		~StudyCourse();

		void addSubject ( Subject );
		/* throws IndexOutOfBoundsException */
		void addSubject ( Subject, unsigned int );
		/* throws IndexOutOfBoundsException */
		void setSubject ( unsigned int, Subject );
		/* throws IndexOutOfBoundsException */
		void remSubject ( unsigned int );
		/* throws IndexOutOfBoundsException */
		Subject getSubject ( unsigned int );
		unsigned int countSubjects();

		int load ( QFile& );
		int save ( QFile& );

		static const int IndexOutOfBoundsException = 1;

	private:
		QDomElement textElement ( const QString&, const QString& );
		QDomElement elementFromSubject ( Subject );
		QDomNode getSubjectNode ( unsigned int );
		bool validate ( const QDomDocument& );

		QDomDocument* data;
};

#endif
