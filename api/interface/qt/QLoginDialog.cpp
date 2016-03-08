/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   QLoginDialog.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/
#include "QLoginDialog.h"
#include "ui_QLoginDialog.h"

#include "Application.h"
#include "AdminUser.h"

#include "vtkSmartPointer.h"

#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>

using namespace std;

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QLoginDialog::QLoginDialog( QWidget* parent )
  : QDialog( parent )
{
  this->ui = new Ui_QLoginDialog;
  this->ui->setupUi( this );

  QObject::connect(
    this->ui->buttonBox, SIGNAL( accepted() ),
    this, SLOT( slotAccepted() ) );

  this->ui->passwordLineEdit->setEchoMode( QLineEdit::Password );
  this->ui->usernameLineEdit->setFocus( Qt::PopupFocusReason ); // make sure username box is focused
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QLoginDialog::~QLoginDialog()
{
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QLoginDialog::slotAccepted()
{
  QString password = this->ui->passwordLineEdit->text().toStdString();

  //vtkSmartPointer< Birch::User > user = vtkSmartPointer< Birch::User >::New();
  if( user->Load( "name", this->ui->usernameLineEdit->text().toStdString() ) && user->IsPassword( password ) )
  { // login successful
    // if the password matches the default password, force the user to change it
    while(password.compare( Birch::User::GetDefaultPassword() ) == 0)
    {
      // prompt for new password
      QString password1 = QInputDialog::getText(
        this,
        QObject::tr( "Change Password" ),
        QObject::tr( "Please provide a new password (cannot be \"password\") for your account:" ),
        QLineEdit::Password );
      /*
      Password Validation,Checking for the password,is that password is either empty 
      or "password",if it is.reset the new password,validate with old password
      if not
      */
      
      if( !(password1.isEmpty() && password1 != "password"))
      {
        errorMessage.setText( tr( "Invalid password, please try again." ) );
        // re-prompt to repeat password
        QString password2 = QInputDialog::getText(
          this,
          QObject::tr( "Re-type Password" ),
          QObject::tr( "Please verify your new password by typing it again:" ),
          QLineEdit::Password );

        if( password1 == password2 )
        {
          // set the replacement password
          password = password1.toStdString();
          user->Set( "password", password );
          user->Save();
        }
      }
     errorMessage.setText( tr( "Invalid password, please try again." ) );
     errorMessage.setText( tr( "Need to Modify the Password" ) );
    }   //While Closed

    // log in the user and mark login time
    Birch::Application::GetInstance()->SetActiveUser( user );
    user->Set( "last_login", Birch::getTime( "%Y-%m-%d %H:%M:%S" ) );
    user->Save();
    this->accept();
  }
  else
  { // login failed
    QMessageBox errorMessage( this );
    errorMessage.setWindowModality( Qt::WindowModal );
    errorMessage.setIcon( QMessageBox::Warning );
    errorMessage.setText( tr( "Invalid username or password, please try again." ) );
    errorMessage.exec();
  }
}
