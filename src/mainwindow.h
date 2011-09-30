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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QCloseEvent>
#include <QString>
#include <QFile>
#include <QTranslator>
#include <QAction>
#include <QList>

#include "ui_mainwindow.h"
#include "studycourse.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
		Q_OBJECT

	public:
		MainWindow ( QWidget* parent = 0, Qt::WFlags fl = 0 );
		~MainWindow();
		void loadFile ( QFile& );
		void saveFile ( QFile& );
		void setTranslator ( QTranslator* );
		QTranslator* getTranslator ();
		void setLanguage ( const QString& );

	protected:
		/* methods */
		void closeEvent ( QCloseEvent* );
		bool continueWithoutSaving();
		void reloadSubjects();
		QString getSaveFileName();
		void saveSettings();
		void loadSettings();
		void createLanguageMenu();
		void deleteLanguageMenu();

		/* attributes */
		bool changed;
		QFile* current_file;
		StudyCourse* studycourse;
		QList<QAction*> languageActions;
		QTranslator* translator;

	protected slots:
		/* Menu */
		void actionNew_clicked();
		void actionOpen_clicked();
		void actionSave_clicked();
		void actionSaveAs_clicked();
		void actionSaveTemplateAs_clicked();
		void actionAbout_clicked();
		void actionAboutQt_clicked();
		void selectLanguage();

		/* Pushbuttons */
		void buttonAddSubject_clicked();
		void buttonDeleteSubject_clicked();
		void buttonSubjectUp_clicked();
		void buttonSubjectDown_clicked();
		void buttonSubjectApply_clicked();

		/* List of subjects */
		void displaySubject ( int );
};

#endif
