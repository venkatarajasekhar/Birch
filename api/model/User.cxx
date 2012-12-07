/*=========================================================================

  Program:  Birch (CLSA Ultrasound Image Viewer)
  Module:   User.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/
#include "User.h"

#include "Utilities.h"

#include "vtkObjectFactory.h"

namespace Birch
{
  vtkStandardNewMacro( User );

  //-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
  void User::SetVariant( std::string column, vtkVariant value )
  {
    if( 0 == column.compare( "password" ) && value.IsValid() )
    { // if we are setting the password override the parent so that we can hash
      std::string hashedPassword;
      hashString( value.ToString(), hashedPassword );
      value = vtkVariant( hashedPassword );
    }

    this->Superclass::SetVariant( column, value );
  }

  //-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
  void User::ResetPassword()
  {
    this->Set( "password", User::GetDefaultPassword() );
  }

  //-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
  bool User::IsPassword( std::string password )
  {
    // first hash the password argument
    std::string hashedPassword;
    hashString( password, hashedPassword );
    return 0 == hashedPassword.compare( this->Get( "password" ).ToString() );
  }
}
