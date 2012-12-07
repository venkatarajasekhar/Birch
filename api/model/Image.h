/*=========================================================================

  Program:  Birch (CLSA Retinal Image Viewer)
  Module:   Image.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/

/**
 * @class Image
 * @namespace Birch
 * 
 * @author Patrick Emond <emondpd@mcmaster.ca>
 * @author Dean Inglis <inglisd@mcmaster.ca>
 * 
 * @brief An active record for the Image table
 */

#ifndef __Image_h
#define __Image_h

#include "ActiveRecord.h"

#include <iostream>

/**
 * @addtogroup Birch
 * @{
 */

namespace Birch
{
  class User;
  class Image : public ActiveRecord
  {
  public:
    static Image *New();
    vtkTypeMacro( Image, ActiveRecord );
    std::string GetName() { return "Image"; }

    /**
     * Get the file name that this record represents
     */
    std::string GetFileName();

    /**
     * Get whether a particular user has rated this image
     */
    bool IsRatedBy( User* user );

  protected:
    Image() {}
    ~Image() {}

  private:
    Image( const Image& ); // Not implemented
    void operator=( const Image& ); // Not implemented
  };
}

/** @} end of doxygen group */

#endif
