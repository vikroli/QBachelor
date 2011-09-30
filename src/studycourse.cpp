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


#include <QDomText>
#include <QTextStream>

#include "studycourse.h"

#define _ROOT_ data->documentElement()

StudyCourse::StudyCourse()
{
	data = new QDomDocument();
	QDomProcessingInstruction header = data->createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"UTF-8\"" );
	QDomElement root = data->createElement ( "StudyCourse" );
	data->appendChild ( header );
	data->appendChild ( root );
}

StudyCourse::~StudyCourse()
{
	delete data;
}

QDomElement StudyCourse::textElement ( const QString& name, const QString& value )
{
	QDomElement e=data->createElement ( name );
	QDomText t=data->createTextNode ( value );
	e.appendChild ( t );
	return e;
}

QDomElement StudyCourse::elementFromSubject ( Subject s )
{
	QDomElement e=data->createElement ( "Subject" );
	e.appendChild ( textElement ( "Name", s.name ) );
	if ( s.grade > 0 )
		e.appendChild ( textElement ( "Grade", QString::number ( s.grade ) ) );
	e.appendChild ( textElement ( "Credit-Points", QString::number ( s.cps ) ) );
	e.appendChild ( textElement ( "SPPW", QString::number ( s.sppw ) ) );
	if ( s.trials > 0 )
		e.appendChild ( textElement ( "Trials", QString::number ( s.trials ) ) );
	return e;
}

bool StudyCourse::validate ( const QDomDocument& doc )
{
	QDomElement root=doc.documentElement();
	return root.tagName() == "StudyCourse";
}

QDomNode StudyCourse::getSubjectNode ( unsigned int pos )
{
	if ( pos >= countSubjects() )
		throw IndexOutOfBoundsException;

	QDomNode tmp= _ROOT_ .firstChild();
	QDomElement e=tmp.toElement();
	unsigned i = 0;
	while ( !tmp.isNull() && i < pos )
	{
		if ( e.tagName() != "Subject" )
			continue;
		tmp=tmp.nextSibling();
		e=tmp.toElement();
		i++;
	}
	return tmp;
}

void StudyCourse::addSubject ( Subject s )
{
	_ROOT_ .appendChild ( elementFromSubject ( s ) );
}

void StudyCourse::addSubject ( Subject s, unsigned int pos )
{
	QDomNode tmp=getSubjectNode ( pos );
	if ( tmp.isNull() )
		addSubject ( s );
	else
		_ROOT_ .insertBefore ( elementFromSubject ( s ), tmp );
}

void StudyCourse::setSubject ( unsigned int pos, Subject s )
{
	_ROOT_ .replaceChild ( elementFromSubject ( s ), getSubjectNode ( pos ) );
}

void StudyCourse::remSubject ( unsigned int pos )
{
	_ROOT_ .removeChild ( getSubjectNode ( pos ) );
}

Subject StudyCourse::getSubject ( unsigned int pos )
{
	QDomNode tmp=getSubjectNode ( pos );
	QDomNode tmp_1=tmp.firstChild();
	QDomElement e;
	Subject s;
	s.name="";
	s.grade=0;
	s.cps=0;
	s.sppw=0;
	s.trials=0;
	while ( !tmp_1.isNull() )
	{
		e=tmp_1.toElement();
		if ( e.tagName() == "Name" )
		{
			s.name=e.text();
		}
		else if ( e.tagName() == "Grade" )
		{
			s.grade=e.text().toInt();
		}
		else if ( e.tagName() == "Credit-Points" )
		{
			s.cps=e.text().toInt();
		}
		else if ( e.tagName() == "SPPW" )
		{
			s.sppw=e.text().toInt();
		}
		else if ( e.tagName() == "Trials" )
		{
			s.trials=e.text().toInt();
		}
		tmp_1=tmp_1.nextSibling();
	}
	return s;
}

unsigned int StudyCourse::countSubjects()
{
	QDomNode tmp= _ROOT_ .firstChild();
	QDomElement e=tmp.toElement();
	int len=0;
	while ( !tmp.isNull() )
	{
		if ( e.tagName() != "Subject" )
			continue;
		len++;
		tmp=tmp.nextSibling();
		e=tmp.toElement();
	}
	return len;
}

int StudyCourse::load ( QFile& file )
{
	if ( file.open ( QIODevice::ReadOnly ) )
	{
		QDomDocument* tmp=new QDomDocument();
		if ( !tmp->setContent ( &file ) )
		{
			file.close();
			delete tmp;
			return -1;
		}
		else
		{
			file.close();
			if ( !validate ( *tmp ) )
				return -1;
			delete data;
			data=tmp;
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

int StudyCourse::save ( QFile& file )
{
	if ( file.open ( QIODevice::WriteOnly ) )
	{
		QTextStream stream ( &file );
		stream << data->toString();
		file.close();
		return 0;
	}
	else
	{
		return -1;
	}
}
