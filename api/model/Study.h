/*=========================================================================

  Program:  Birch (CLSA Retinal Image Viewer)
  Module:   Study.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/

/**
 * @class Study
 * @namespace Birch
 * 
 * @author Patrick Emond <emondpd@mcmaster.ca>
 * @author Dean Inglis <inglisd@mcmaster.ca>
 * 
 * @brief An active record for the Study table
 */

#ifndef __Study_h
#define __Study_h

#include "ActiveRecord.h"

#include <iostream>
#include <vector>

/**
 * @addtogroup Birch
 * @{
 */

namespace Birch
{
  class Study : public ActiveRecord
  {
  public:
    static Study *New();
    vtkTypeMacro( Study, ActiveRecord );
//    static void UpdateData();
//    static std::vector< std::string > GetIdentifierList();
    std::string GetName() { return "Study"; }

    /**
     * Returns the next study in UID order.
     */
    vtkSmartPointer<Study> GetNext();

    /**
     * Makes the current record the next record in UID order.
     */
    void Next();

    /**
     * Returns the previous study in UID order.
     */
    vtkSmartPointer<Study> GetPrevious();

    /**
     * Makes the current record the previous record in UID order.
     */
    void Previous();

    /**
     * Returns a vector of all UIDs alphabetically ordered
     */
    static std::vector< std::string > GetUIDList();

    /**
     * Returns whether a user has rated all images associated with the study.
     * If the study has no images this method returns true.
     */
    bool IsRatedBy( User* user );

  protected:
    Study() {}
    ~Study() {}

  private:
    Study( const Study& ); // Not implemented
    void operator=( const Study& ); // Not implemented
  };
}

/** @} end of doxygen group */

#endif
