/*=========================================================================

  Program:  Birch (CLSA Retinal Image Viewer)
  Module:   Rating.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/

/**
 * @class Rating
 * @namespace Birch
 * 
 * @author Patrick Emond <emondpd@mcmaster.ca>
 * @author Dean Inglis <inglisd@mcmaster.ca>
 * 
 * @brief An active record for the Rating table
 */

#ifndef __Rating_h
#define __Rating_h

#include "ActiveRecord.h"

#include <iostream>
using namespace std;

/**
 * @addtogroup Birch
 * @{
 */

namespace Birch
{
  /* 
  Abstract Class Creation in C++
  Ctor,Dtor is in Protected Mode
*/
 class Rating : public ActiveRecord
  {
  public:
    //static Rating *New();
    vtkTypeMacro( Rating, ActiveRecord );
    string GetName() 
    { 
      return "Rating";
      }

  protected:
     Rating() {}
    ~Rating() {}

  private:
    Rating( const Rating& ); // Not implemented
    void operator=( const Rating& ); // Not implemented
  };
}

/** @} end of doxygen group */

#endif
