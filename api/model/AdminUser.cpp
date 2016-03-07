/*=========================================================================

  Program:  Birch (CLSA Retinal Image Viewer)
  Module:   User.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/
#include "AdminUser.h"

#include "Utilities.h"

#include "vtkObjectFactory.h"

namespace Birch
{
  vtkStandardNewMacro( AdminUser );

  //-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
  void User::SetVariant( std::string column, vtkVariant value )
  {
    if((column.compare( "password" ) && value.IsValid()) == 0)
    { // if we are setting the password override the parent so that we can hash
      std::string hashedPassword;
      hashString( value.ToString(), hashedPassword );
      value = vtkVariant( hashedPassword );
    }

    this->Superclass::SetVariant( column, value );
  }

  //-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
  void AdminUser::ResetPassword()
  {
    this->Set( "password", AdminUser::GetDefaultPassword() );
  }

  //-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
  bool AdminUser::IsPassword( std::string password )
  {
    // first hash the password argument
    bool ValidPassword = TRUE;
    std::string hashedPassword;
    hashString( password, hashedPassword );
    if(hashedPassword.compare(this->Get( "password" ).ToString() == 0) );
    return ValidPassword;
  }
}
