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


#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QFileDialog>
#include <QIcon>
#include <QStatusBar>
#include <QCoreApplication>
#include <QDir>
#include <QStringList>

#include "mainwindow.h"
#include "aboutdialog.h"

#define ERROR(a) QMessageBox::critical(this, tr("Error - QBachelor"), a, QMessageBox::Ok, QMessageBox::Ok)

#define _STATUSBAR_STD_TIMEOUT_ 3000
#define STATUS(a,b) this->statusBar()->showMessage(a,b)
#define STD_STATUS(a) STATUS(a, _STATUSBAR_STD_TIMEOUT_ )

MainWindow::MainWindow ( QWidget* parent, Qt::WFlags fl )
		: QMainWindow ( parent, fl ), Ui::MainWindow()
{
	setupUi ( this );

	/* Menu */
	connect ( this->actionNew, SIGNAL ( triggered() ), this, SLOT ( actionNew_clicked() ) );
	connect ( this->actionOpen, SIGNAL ( triggered() ), this, SLOT ( actionOpen_clicked() ) );
	connect ( this->actionSave, SIGNAL ( triggered() ), this, SLOT ( actionSave_clicked() ) );
	connect ( this->actionSaveAs, SIGNAL ( triggered() ), this, SLOT ( actionSaveAs_clicked() ) );
	connect ( this->actionSaveTemplateAs, SIGNAL ( triggered() ), this, SLOT ( actionSaveTemplateAs_clicked() ) );
	connect ( this->actionQuit, SIGNAL ( triggered() ), this, SLOT ( close() ) );
	connect ( this->actionAbout, SIGNAL ( triggered() ), this, SLOT ( actionAbout_clicked() ) );
	connect ( this->actionAboutQt, SIGNAL ( triggered() ), this, SLOT ( actionAboutQt_clicked() ) );

	/* Pushbuttons */
	connect ( this->buttonAddSubject, SIGNAL ( clicked() ), this, SLOT ( buttonAddSubject_clicked() ) );
	connect ( this->buttonDeleteSubject, SIGNAL ( clicked() ), this, SLOT ( buttonDeleteSubject_clicked() ) );
	connect ( this->buttonSubjectUp, SIGNAL ( clicked() ), this, SLOT ( buttonSubjectUp_clicked() ) );
	connect ( this->buttonSubjectDown, SIGNAL ( clicked() ), this, SLOT ( buttonSubjectDown_clicked() ) );
	connect ( this->buttonSubjectApply, SIGNAL ( clicked() ), this, SLOT ( buttonSubjectApply_clicked() ) );

	/* List of Subjects */
	connect ( this->subject_list, SIGNAL ( currentRowChanged ( int ) ), this , SLOT ( displaySubject ( int ) ) );

	/* Initialise variables */
	this->changed = false;
	this->current_file = 0;
	this->studycourse = new StudyCourse();

	/* create Language Menu */
	createLanguageMenu();

	loadSettings();
	reloadSubjects();
}

MainWindow::~MainWindow()
{
	if ( this->current_file )
		delete this->current_file;
	delete this->studycourse;
	deleteLanguageMenu();
}

void MainWindow::closeEvent ( QCloseEvent* e )
{
	if ( continueWithoutSaving() )
	{
		saveSettings();
		e->accept();
	}
	else
	{
		e->ignore();
	}
}

bool MainWindow::continueWithoutSaving()
{
	if ( changed )
	{
		/*
		 * if something has changed, ask the user
		 * if he or she want's to save the changes
		 */
		QMessageBox::StandardButton answer = QMessageBox::question ( this, tr ( "Close File - QBachelor" ), tr ( "Do you want to save your changes?" ), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel );
		/*
		 * save changes if wanted by the user
		 */
		if ( answer == QMessageBox::Yes )
		{
			actionSave_clicked();
			return true;
		}
		else if ( answer == QMessageBox::No )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

void MainWindow::reloadSubjects()
{
	subject_list->clear();
	unsigned int count = studycourse->countSubjects();
	Subject s;
	QIcon green ( ":green.png" );
	QIcon yellow ( ":yellow.png" );
	QIcon red ( ":red.png" );
	QIcon* icon;

	int cps = 0;
	int cps_total = 0;

	double grade = 0.0;
	double allowed_grades[]= {0.0, 1.0, 1.3, 1.7, 2.0, 2.3, 2.7, 3.0, 3.3, 3.7, 4.0, 5.0, 6.0};
	int cps_total_with_grade = 0;

	for ( unsigned int i=0; i<count; i++ )
	{
		s = studycourse->getSubject ( i );
		/* not compeeded */
		if ( s.grade == 0 )
		{
			icon = &yellow;
		}
		/* failed */
		else if ( s.grade == 12 )
		{
			icon = &red;
		}
		else
		{
			icon = &green;
			cps += s.cps;
			/* passed with a grade */
			if ( s.grade != 11 )
			{
				cps_total_with_grade += s.cps;
				grade += allowed_grades[s.grade] * s.cps;
			}
		}
		cps_total += s.cps;
		subject_list->addItem ( new QListWidgetItem ( *icon, s.name ) );
	}
	overall_cps_label->setText ( tr ( "Credit-Points" ) + ": " + QString::number ( cps ) + "/" + QString::number ( cps_total ) );
	overall_cps_bar->setMaximum ( ( cps_total ? cps_total : 1 ) );
	overall_cps_bar->setValue ( cps );

	if ( cps_total_with_grade )
	{
		grade /= cps_total_with_grade;
	}
	else
	{
		grade = 0.0;
	}
	overall_grade_label->setText ( tr ( "Grade" ) + ": " + QString::number ( grade ) );
	overall_grade_bar->setMaximum ( 3000 );
	overall_grade_bar->setValue ( 4000- ( int ) ( grade*1000.0 ) );
}

void MainWindow::loadFile ( QFile& file )
{
	if ( !file.exists() )
		return;

	if ( studycourse->load ( file ) )
	{
		ERROR ( tr ( "Error loading File!" ) );
		return;
	}
	if ( current_file )
		delete current_file;
	current_file = new QFile ( file.fileName() );
	changed = false;
	reloadSubjects();
	STD_STATUS ( tr ( "File loaded" ) );
}

void MainWindow::saveFile ( QFile& file )
{
	if ( studycourse->save ( file ) )
	{
		ERROR ( tr ( "Error saving file!" ) );
		return;
	}
	if ( current_file )
		delete current_file;
	current_file = new QFile ( file.fileName() );
	changed = false;
	reloadSubjects();
	STD_STATUS ( tr ( "File saved" ) );
}

QString MainWindow::getSaveFileName()
{
	QString fileName = QFileDialog::getSaveFileName ( this, tr ( "Save Bachelor File" ), 0, tr ( "Bachelor File (*.bac)" ) );
	if ( fileName == QString::null )
		return QString::null;
	if ( !fileName.endsWith ( ".bac", Qt::CaseInsensitive ) )
		fileName.append ( ".bac" );
	return fileName;
}

void MainWindow::actionNew_clicked()
{
	if ( !continueWithoutSaving() )
		return;

	delete this->studycourse;
	this->studycourse = new StudyCourse();
	this->changed = false;
	if ( this->current_file )
	{
		delete this->current_file;
		this->current_file = 0;
	}
	reloadSubjects();
}

void MainWindow::actionOpen_clicked()
{
	if ( !continueWithoutSaving() )
		return;

	QString fileName = QFileDialog::getOpenFileName ( this, tr ( "Open Bachelor File" ), 0, tr ( "Bachelor Files (*.bac);;All Files (*)" ) );
	if ( fileName == QString::null )
		return;
	QFile file ( fileName );
	loadFile ( file );
}

void MainWindow::actionSave_clicked()
{
	if ( current_file )
	{
		QFile file ( current_file->fileName() );
		saveFile ( file );
	}
	else
	{
		actionSaveAs_clicked();
	}
}

void MainWindow::actionSaveAs_clicked()
{
	QString fileName = getSaveFileName();
	if ( fileName == QString::null )
		return;
	QFile file ( fileName );
	saveFile ( file );
}

void MainWindow::actionSaveTemplateAs_clicked()
{
	StudyCourse* sc = new StudyCourse();
	Subject s;
	QString fileName = getSaveFileName();
	if ( fileName == QString::null )
		return;
	for ( unsigned int i=0; i<studycourse->countSubjects(); i++ )
	{
		s = studycourse->getSubject ( i );
		s.grade = 0;
		s.trials = 0;
		sc->addSubject ( s );
	}
	QFile file ( fileName );
	sc->save ( file );
	delete sc;
}

void MainWindow::actionAbout_clicked()
{
	AboutDialog about ( this );
	about.exec();
}

void MainWindow::actionAboutQt_clicked()
{
	QMessageBox::aboutQt ( this );
}

void MainWindow::buttonAddSubject_clicked()
{
	Subject s;
	s.name = tr ( "New subject" );
	s.grade = 0;
	s.cps = 0;
	s.sppw = 0;
	s.trials = 0;
	studycourse->addSubject ( s );
	reloadSubjects();
	changed = true;
	subject_list->setCurrentRow ( subject_list->count()-1 );
}

void MainWindow::buttonDeleteSubject_clicked()
{
	try
	{
		studycourse->remSubject ( subject_list->currentRow() );
		changed = true;
		reloadSubjects();
	}
	catch ( int e )
	{
		ERROR ( tr ( "Select a subject from the list first!" ) );
	}
}

void MainWindow::buttonSubjectUp_clicked()
{
	try
	{
		int selection = subject_list->currentRow();
		Subject s1 = studycourse->getSubject ( selection );
		Subject s2 = studycourse->getSubject ( selection-1 );
		studycourse->setSubject ( selection-1, s1 );
		studycourse->setSubject ( selection, s2 );
		reloadSubjects();
		subject_list->setCurrentRow ( selection-1 );
		changed = true;
	}
	catch ( int e )
		{}
}

void MainWindow::buttonSubjectDown_clicked()
{
	try
	{
		int selection = subject_list->currentRow();
		Subject s1 = studycourse->getSubject ( selection );
		Subject s2 = studycourse->getSubject ( selection+1 );
		studycourse->setSubject ( selection+1, s1 );
		studycourse->setSubject ( selection, s2 );
		reloadSubjects();
		subject_list->setCurrentRow ( selection+1 );
		changed = true;
	}
	catch ( int e )
		{}
}

void MainWindow::buttonSubjectApply_clicked()
{
	int selected = subject_list->currentRow();
	Subject s;
	s.name = subject_name->text();
	s.grade = subject_grade->currentIndex();
	s.cps = subject_cps->value();
	s.sppw = subject_sppw->value();
	s.trials = subject_trials->value();

	if ( s.grade != 0 && s.trials == 0 )
	{
		s.trials = 1;
	}
	else if ( s.grade == 0 && s.trials != 0 )
	{
		s.trials = 0;
	}

	try
	{
		studycourse->setSubject ( selected, s );
		changed = true;
	}
	catch ( int e )
	{
		ERROR ( tr ( "Select a subject from the list first!" ) );
	}
	reloadSubjects();
	subject_list->setCurrentRow ( selected );
}

void MainWindow::displaySubject ( int index )
{
	try
	{
		Subject s = studycourse->getSubject ( index );
		subject_name->setText ( s.name );
		if ( s.grade< ( unsigned int ) ( subject_grade->count() ) )
			subject_grade->setCurrentIndex ( s.grade );
		subject_cps->setValue ( s.cps );
		subject_sppw->setValue ( s.sppw );
		subject_trials->setValue ( s.trials );
	}
	catch ( int e )
		{}
}

void MainWindow::loadSettings()
{
	QSettings settings ( "Viktoria Krolikowski", "QBachelor" );
	settings.beginGroup ( "Main" );
	QString fileName = settings.value ( "LastUsedFile", "" ).toString();
	if ( fileName != "" )
	{
		QFile file ( fileName );
		if ( file.exists() )
		{
			loadFile ( file );
		}
	}
	settings.endGroup();

	settings.beginGroup ( "MainWindow" );
	this->resize ( settings.value ( "Size", this->sizeHint() ).toSize() );
	this->move ( settings.value ( "Position", this->pos() ).toPoint() );
	if ( settings.value ( "Maximized", false ).toBool() )
		this->setWindowState ( Qt::WindowMaximized );
	settings.endGroup();
}

void MainWindow::saveSettings()
{
	QSettings settings ( "Viktoria Krolikowski", "QBachelor" );
	settings.beginGroup ( "Main" );
	QString fileName = "";
	if ( current_file )
	{
		QFileInfo fi ( current_file->fileName() );
		fileName = fi.absoluteFilePath();
	}
	settings.setValue ( "LastUsedFile", fileName );
	settings.endGroup();

	settings.beginGroup ( "MainWindow" );
	settings.setValue ( "Size", this->size() );
	settings.setValue ( "Position", this->pos() );
	settings.setValue ( "Maximized", this->isMaximized() );
	settings.endGroup();
}

void MainWindow::setTranslator ( QTranslator* t )
{
	translator = t;
}

QTranslator* MainWindow::getTranslator()
{
	return translator;
}

void MainWindow::createLanguageMenu()
{
	QDir appDir ( QCoreApplication::applicationDirPath() );
	QStringList entries = appDir.entryList ( QStringList ( "qbachelor_*.qm" ) );
	entries.prepend ( QString::null );
	QAction* action;

	for ( int i=0; i<entries.count(); i++ )
	{
		action = new QAction ( this );
		action->setObjectName ( entries.at ( i ) );
		if ( i == 0 )
		{
			action->setText ( tr ( "Default Language" ) );
		}
		else
		{
			action->setText ( entries.at ( i ).section ( '_', 1, 1 ).section ( '.', 0, 0 ).toUpper() );
		}
		connect ( action, SIGNAL ( triggered() ), this, SLOT ( selectLanguage() ) );
		menuLanguages->addAction ( action );
		languageActions.append ( action );
	}
}

void MainWindow::deleteLanguageMenu()
{
	for ( int i=0; i<languageActions.count(); i++ )
	{
		delete languageActions.at ( i );
	}
	languageActions.clear();
}

void MainWindow::setLanguage ( const QString& lang )
{
	QString currentDir=QDir::currentPath();
	QDir::setCurrent ( QCoreApplication::applicationDirPath() );
	translator->load ( lang );
	QDir::setCurrent ( currentDir );
	retranslateUi ( this );
	reloadSubjects();
}

void MainWindow::selectLanguage()
{
	QAction* action = qobject_cast<QAction*> ( sender() );
	setLanguage ( action->objectName() );
}
