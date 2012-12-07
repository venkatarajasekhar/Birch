/*=========================================================================

  Program:  Birch (CLSA Ultrasound Image Viewer)
  Module:   User.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/

/**
 * @class User
 * @namespace Birch
 * 
 * @author Patrick Emond <emondpd@mcmaster.ca>
 * @author Dean Inglis <inglisd@mcmaster.ca>
 * 
 * @brief An active record for the User table
 */

#ifndef __User_h
#define __User_h

#include "ActiveRecord.h"

#include <iostream>

/**
 * @addtogroup Birch
 * @{
 */

namespace Birch
{
  class User : public ActiveRecord
  {
  public:
    static User *New();
    vtkTypeMacro( User, ActiveRecord );

    virtual void ResetPassword();
    virtual bool IsPassword( std::string );
    static std::string GetDefaultPassword() { return "password"; }
    std::string GetName() { return "User"; }

  protected:
    User() {}
    ~User() {}

    void SetVariant( std::string column, vtkVariant value );

  private:
    User( const User& ); // Not implemented
    void operator=( const User& ); // Not implemented
  };
}

/** @} end of doxygen group */

#endif
