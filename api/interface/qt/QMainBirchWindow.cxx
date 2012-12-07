/*=========================================================================

  Program:  Birch (CLSA Ultrasound Image Viewer)
  Module:   QMainBirchWindow.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/
#include "QMainBirchWindow.h"
#include "ui_QMainBirchWindow.h"

#include "Application.h"
#include "Image.h"
#include "Rating.h"
#include "Study.h"
#include "User.h"

#include "vtkMedicalImageViewer.h"

#include "QAboutDialog.h"
#include "QLoginDialog.h"
#include "QProgressDialog.h"
#include "QSelectStudyDialog.h"
#include "QUserListDialog.h"

#include <QCloseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTreeWidgetItem>

#include <stdexcept>

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QMainBirchWindow::QMainBirchWindow( QWidget* parent )
  : QMainWindow( parent )
{
  Birch::Application *app = Birch::Application::GetInstance();
  QMenu *menu;
  
  this->ui = new Ui_QMainBirchWindow;
  this->ui->setupUi( this );
  
  // set up child widgets
  this->ui->studyTreeWidget->header()->hide();

  // connect the menu items
  QObject::connect(
    this->ui->actionOpenStudy, SIGNAL( triggered() ),
    this, SLOT( slotOpenStudy() ) );
  QObject::connect(
    this->ui->actionPreviousStudy, SIGNAL( triggered() ),
    this, SLOT( slotPreviousStudy() ) );
  QObject::connect(
    this->ui->actionNextStudy, SIGNAL( triggered() ),
    this, SLOT( slotNextStudy() ) );
  QObject::connect(
    this->ui->actionLogin, SIGNAL( triggered() ),
    this, SLOT( slotLogin() ) );
  QObject::connect(
    this->ui->actionUserManagement, SIGNAL( triggered() ),
    this, SLOT( slotUserManagement() ) );
  QObject::connect(
    this->ui->actionUpdateStudyDatabase, SIGNAL( triggered() ),
    this, SLOT( slotUpdateStudyDatabase() ) );
  QObject::connect(
    this->ui->actionExit, SIGNAL( triggered() ),
    qApp, SLOT( closeAllWindows() ) );
  
  // connect the help menu items
  QObject::connect(
    this->ui->actionAbout, SIGNAL( triggered() ),
    this, SLOT( slotAbout() ) );
  QObject::connect(
    this->ui->actionManual, SIGNAL( triggered() ),
    this, SLOT( slotManual() ) );

  QObject::connect(
    this->ui->previousStudyPushButton, SIGNAL( clicked() ),
    this, SLOT( slotPreviousStudy() ) );
  QObject::connect(
    this->ui->nextStudyPushButton, SIGNAL( clicked() ),
    this, SLOT( slotNextStudy() ) );
  QObject::connect(
    this->ui->studyTreeWidget, SIGNAL( itemSelectionChanged() ),
    this, SLOT( slotTreeSelectionChanged() ) );
  QObject::connect(
    this->ui->ratingSlider, SIGNAL( valueChanged( int ) ),
    this, SLOT( slotRatingSliderChanged( int ) ) );

  this->readSettings();
  this->updateInterface();
};

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QMainBirchWindow::~QMainBirchWindow()
{
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::closeEvent( QCloseEvent *event )
{
  this->writeSettings();
  event->accept();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotOpenStudy()
{
  bool loggedIn = NULL != Birch::Application::GetInstance()->GetActiveUser();

  if( loggedIn )
  {
    QSelectStudyDialog dialog( this );
    dialog.setModal( true );
    dialog.setWindowTitle( tr( "Select Study" ) );
    dialog.exec();

    // active study may have changed so update the interface
    this->updateInterface();
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotPreviousStudy()
{
  bool found = false;
  Birch::Application *app = Birch::Application::GetInstance();
  Birch::User *user = app->GetActiveUser();
  Birch::Study *activeStudy = app->GetActiveStudy();
  vtkSmartPointer< Birch::Study > study;
  if( user && activeStudy )
  {
    // check if unrated checkbox is pressed, keep searching for an unrated study
    if( this->ui->unratedCheckBox->isChecked() )
    {
      int currentStudyId = activeStudy->Get( "id" ).ToInt();

      // keep getting the previous study until we find one that has images which are not rated
      study = activeStudy->GetPrevious();
      while( study->Get( "id" ).ToInt() != currentStudyId )
      {
        if( 0 < study->GetCount( "Image" ) && !study->IsRatedBy( user ) )
        {
          found = true;
          break;
        }
        study = study->GetPrevious();
      }

      // warn user if no unrated studies left
      if( study->Get( "id" ).ToInt() == currentStudyId )
      {
        QMessageBox errorMessage( this );
        errorMessage.setWindowModality( Qt::WindowModal );
        errorMessage.setIcon( QMessageBox::Warning );
        errorMessage.setText( tr( "There are no remaining unrated studies available at this time." ) );
        errorMessage.exec();
      }
    }
    else
    {
      study = activeStudy->GetPrevious();
      found = true;
    }
  }

  if( found )
  {
    app->SetActiveStudy( study );
    this->updateInterface();
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotNextStudy()
{
  bool found = false;
  Birch::Application *app = Birch::Application::GetInstance();
  Birch::User *user = app->GetActiveUser();
  Birch::Study *activeStudy = app->GetActiveStudy();
  vtkSmartPointer< Birch::Study > study;
  if( user && activeStudy )
  {
    // check if unrated checkbox is pressed, keep searching for an unrated study
    if( this->ui->unratedCheckBox->isChecked() )
    {
      int currentStudyId = activeStudy->Get( "id" ).ToInt();

      // keep getting the previous study until we find one that has images which are not rated
      study = activeStudy->GetNext();
      while( study->Get( "id" ).ToInt() != currentStudyId )
      {
        if( 0 < study->GetCount( "Image" ) && !study->IsRatedBy( user ) )
        {
          found = true;
          break;
        }
        study = study->GetNext();
      }

      // warn user if no unrated studies left
      if( study->Get( "id" ).ToInt() == currentStudyId )
      {
        QMessageBox errorMessage( this );
        errorMessage.setWindowModality( Qt::WindowModal );
        errorMessage.setIcon( QMessageBox::Warning );
        errorMessage.setText( tr( "There are no remaining unrated studies available at this time." ) );
        errorMessage.exec();
      }
    }
    else
    {
      study = activeStudy->GetNext();
      found = true;
    }
  }

  if( found )
  {
    app->SetActiveStudy( study );
    this->updateInterface();
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotLogin()
{
  bool loggedIn = NULL != Birch::Application::GetInstance()->GetActiveUser();

  if( loggedIn )
  {
    Birch::Application::GetInstance()->ResetApplication();
  }
  else
  {
    QLoginDialog dialog( this );
    dialog.setModal( true );
    dialog.setWindowTitle( tr( "Login" ) );
    dialog.exec();
  }

  // active user may have changed so update the interface
  this->updateInterface();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotUserManagement()
{
  int attempt = 1;

  while( attempt < 4 )
  {
    // check for admin password
    QString text = QInputDialog::getText(
      this,
      QObject::tr( "User Management" ),
      QObject::tr( attempt > 1 ? "Wrong password, try again:" : "Administrator password:" ),
      QLineEdit::Password );
    
    // do nothing if the user hit the cancel button
    if( text.isEmpty() ) break;

    vtkSmartPointer< Birch::User > user = vtkSmartPointer< Birch::User >::New();
    user->Load( "name", "administrator" );
    if( user->IsPassword( text.toStdString().c_str() ) )
    {
      // load the users dialog
      QUserListDialog usersDialog( this );
      usersDialog.setModal( true );
      usersDialog.setWindowTitle( tr( "User Management" ) );
      usersDialog.exec();
      break;
    }
    attempt++;
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotUpdateStudyDatabase()
{
/* TODO: implement once Opal is ready
  int attempt = 1;

  while( attempt < 4 )
  {
    // check for admin password
    QString text = QInputDialog::getText(
      this,
      QObject::tr( "User Management" ),
      QObject::tr( attempt > 1 ? "Wrong password, try again:" : "Administrator password:" ),
      QLineEdit::Password );
    
    // do nothing if the user hit the cancel button
    if( text.isEmpty() ) break;

    vtkSmartPointer< Birch::User > user = vtkSmartPointer< Birch::User >::New();
    user->Load( "name", "administrator" );
    if( user->IsPassword( text.toStdString().c_str() ) )
    {
      // create a progress dialog to observe the progress of the update
      QProgressDialog dialog( this );
      dialog.setModal( true );
      dialog.setWindowTitle( tr( "Updating Study Database" ) );
      dialog.setMessage( tr( "Please wait while the study database is updated." ) );
      dialog.open();
      Birch::Study::UpdateData();
      dialog.accept();
      break;
    }
    attempt++;
  }
*/
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotAbout()
{
  QAboutDialog dialog( this );
  dialog.setModal( true );
  dialog.setWindowTitle( tr( "About Birch" ) );
  dialog.exec();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotManual()
{
  // TODO: open link to Birch manual
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::readSettings()
{
  QSettings settings( "CLSA", "Birch" );
  
  settings.beginGroup( "MainBirchWindow" );
  if( settings.contains( "size" ) ) this->resize( settings.value( "size" ).toSize() );
  if( settings.contains( "pos" ) ) this->move( settings.value( "pos" ).toPoint() );
  if( settings.contains( "maximized" ) && settings.value( "maximized" ).toBool() )
    this->showMaximized();
  settings.endGroup();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotTreeSelectionChanged()
{
  Birch::Application *app = Birch::Application::GetInstance();
  
  QList<QTreeWidgetItem*> list = this->ui->studyTreeWidget->selectedItems();
  if( 0 < list.size() )
  {
    std::map< QTreeWidgetItem*, vtkSmartPointer<Birch::ActiveRecord> >::iterator it;
    it = this->treeModelMap.find( list.at( 0 ) );
    if( it != this->treeModelMap.end() )
    {
      Birch::ActiveRecord *record = it->second;
      app->SetActiveImage( Birch::Image::SafeDownCast( record ) );
    }
  }

  this->updateInterface();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::slotRatingSliderChanged( int value )
{
  Birch::Application *app = Birch::Application::GetInstance();

  // make sure we have an active image
  Birch::User *user = app->GetActiveUser();
  if( !user ) throw std::runtime_error( "Rating slider modified without an active user" );
  Birch::Image *image = app->GetActiveImage();
  if( !image ) throw std::runtime_error( "Rating slider modified without an active image" );

  // See if we have a rating for this user and image
  std::map< std::string, std::string > map;
  map["user_id"] = user->Get( "id" ).ToString();
  map["image_id"] = image->Get( "id" ).ToString();
  vtkSmartPointer< Birch::Rating > rating = vtkSmartPointer< Birch::Rating >::New();
  if( !rating->Load( map ) )
  { // no record exists, set the user and image ids
    rating->Set( "user_id", user->Get( "id" ).ToInt() );
    rating->Set( "image_id", image->Get( "id" ).ToInt() );
  }

  if( 0 == value ) rating->SetNull( "rating" );
  else rating->Set( "rating", value );

  rating->Save();
  this->updateRating();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::writeSettings()
{
  QSettings settings( "CLSA", "Birch" );
  
  settings.beginGroup( "MainBirchWindow" );
  settings.setValue( "size", this->size() );
  settings.setValue( "pos", this->pos() );
  settings.setValue( "maximized", this->isMaximized() );
  settings.endGroup();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::updateStudyInformation()
{
  QString interviewerString = tr( "N/A" );
  QString siteString = tr( "N/A" );
  QString dateString = tr( "N/A" );

  // fill in the active study information
  Birch::Study *study = Birch::Application::GetInstance()->GetActiveStudy();
  if( study )
  {
    interviewerString = study->Get( "interviewer" ).ToString().c_str();
    siteString = study->Get( "site" ).ToString().c_str();
    dateString = study->Get( "datetime_acquired" ).ToString().c_str();
  }

  this->ui->interviewerValueLabel->setText( interviewerString );
  this->ui->siteValueLabel->setText( siteString );
  this->ui->dateValueLabel->setText( dateString );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::updateStudyTreeWidget()
{
  Birch::Study *study = Birch::Application::GetInstance()->GetActiveStudy();

  // stop the tree's signals until we are done
  bool oldSignalState = this->ui->studyTreeWidget->blockSignals( true );

  // if a study is open then populate the study tree
  this->treeModelMap.clear();
  this->ui->studyTreeWidget->clear();
  if( study )
  {
    // get the active image so that we can highlight it
    Birch::Image *activeImage = Birch::Application::GetInstance()->GetActiveImage();
    QTreeWidgetItem *selectedItem = NULL;

    // make root the study's UID
    QString name = tr( "Study: " );
    name += study->Get( "uid" ).ToString().c_str();
    QTreeWidgetItem *root = new QTreeWidgetItem( this->ui->studyTreeWidget );
    root->setText( 0, name );
    root->setExpanded( true );
    root->setFlags( Qt::ItemIsEnabled );
    this->ui->studyTreeWidget->addTopLevelItem( root );

    // make each image a child of the root
    std::vector< vtkSmartPointer< Birch::Image > > imageList;
    std::vector< vtkSmartPointer< Birch::Image > >::iterator imageIt;
    study->GetList( &imageList );
    for( imageIt = imageList.begin(); imageIt != imageList.end(); ++imageIt )
    {
      Birch::Image *image = imageIt->GetPointer();
      name = tr( "Laterality" );
      name += image->Get( "laterality" ).ToString().c_str();
      QTreeWidgetItem *imageItem = new QTreeWidgetItem( root );
      this->treeModelMap[imageItem] = *imageIt;
      imageItem->setText( 0, name );
      imageItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
      if( activeImage && activeImage->Get( "id" ).ToInt() == image->Get( "id" ).ToInt() )
        selectedItem = imageItem;
    }
      
    if( selectedItem ) this->ui->studyTreeWidget->setCurrentItem( selectedItem );
  }

  // re-enable the tree's signals
  this->ui->studyTreeWidget->blockSignals( oldSignalState );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::updateMedicalImageWidget()
{
  Birch::Image *image = Birch::Application::GetInstance()->GetActiveImage();

  if( image )
  {
    this->ui->medicalImageWidget->loadImage( QString( image->GetFileName().c_str() ) );
    // TODO: in some situations we may not want to display the static images
    // for example, when a reader should be blinded to any previous slice selections
  }
  else
  {
    this->ui->medicalImageWidget->resetImage();
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::updateRating()
{
  // stop the rating slider's signals until we are done
  bool oldSignalState = this->ui->ratingSlider->blockSignals( true );

  int ratingValue = 0;
  Birch::Application *app = Birch::Application::GetInstance();

  // make sure we have an active image
  Birch::User *user = app->GetActiveUser();
  Birch::Image *image = app->GetActiveImage();

  if( user && image )
  {
    std::map< std::string, std::string > map;
    map["user_id"] = user->Get( "id" ).ToString();
    map["image_id"] = image->Get( "id" ).ToString();
    vtkSmartPointer< Birch::Rating > rating = vtkSmartPointer< Birch::Rating >::New();
    
    if( rating->Load( map ) )
    {
      vtkVariant v = rating->Get( "rating" );
      if( v.IsValid() ) ratingValue = v.ToInt();
    }
  }

  this->ui->ratingSlider->setValue( ratingValue );
  this->ui->ratingValueLabel->setText( 0 == ratingValue ? tr( "N/A" ) : QString::number( ratingValue ) );

  // re-enable the rating slider's signals
  this->ui->ratingSlider->blockSignals( oldSignalState );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QMainBirchWindow::updateInterface()
{
  Birch::Application *app = Birch::Application::GetInstance();
  Birch::Study *study = app->GetActiveStudy();
  Birch::Image *image = app->GetActiveImage();
  bool loggedIn = NULL != app->GetActiveUser();

  // login button (login/logout)
  this->ui->actionLogin->setText( tr( loggedIn ? "Logout" : "Login" ) );

  // set all widget enable states
  this->ui->actionOpenStudy->setEnabled( loggedIn );
  this->ui->unratedCheckBox->setEnabled( study );
  this->ui->actionPreviousStudy->setEnabled( study );
  this->ui->actionNextStudy->setEnabled( study );
  this->ui->previousStudyPushButton->setEnabled( study );
  this->ui->nextStudyPushButton->setEnabled( study );
  this->ui->ratingSlider->setEnabled( image );
  this->ui->notePushButton->setEnabled( false ); // TODO: notes aren't implemented
  this->ui->studyTreeWidget->setEnabled( study );
  this->ui->medicalImageWidget->setEnabled( loggedIn );

  this->updateStudyTreeWidget();
  this->updateStudyInformation();
  this->updateMedicalImageWidget();
  this->updateRating();
}
