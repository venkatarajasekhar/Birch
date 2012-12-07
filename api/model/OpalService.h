/*=========================================================================

  Program:  Birch (CLSA Retinal Image Viewer)
  Module:   OpalService.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/

/**
 * @class OpalService
 * @namespace Birch
 * 
 * @author Patrick Emond <emondpd@mcmaster.ca>
 * @author Dean Inglis <inglisd@mcmaster.ca>
 * 
 * @brief Class for interacting with Opal
 * 
 * This class provides a programming interface to Opal's RESTful interface by using the
 * curl library.  A description of Opal can be found
 * <a href="http://www.obiba.org/?q=node/63">here</a>.
 */

#ifndef __OpalService_h
#define __OpalService_h

#include "ModelObject.h"

#include "vtkSmartPointer.h"
#include "vtkBirchMySQLQuery.h"

#include <iostream>
#include <json/reader.h>
#include <map>
#include <vector>

class vtkBirchMySQLOpalService;

/**
 * @addtogroup Birch
 * @{
 */

namespace Birch
{
  class User;
  class OpalService : public ModelObject
  {
  public:
    static OpalService *New();
    vtkTypeMacro( OpalService, ModelObject );

    /**
     * Defines connection parameters to use when communicating with the Opal server
     */
    void Setup( std::string username, std::string password, std::string host, int port );

    /**
     * Returns a list of all identifiers in a particular data source and table
     * @param dataSource string
     * @param table string
     */
    std::vector< std::string > GetIdentifiers( std::string dataSource, std::string table );

    /**
     * Returns a map of all values for a particular data source, table and variable
     * @param dataSource string
     * @param table string
     * @param variable string
     * @param offset int The offset to begin the list at.
     * @param limit int The limit of how many key/value pairs to return
     */
    std::map< std::string, std::string > GetValueList(
      std::string dataSource, std::string table, std::string variable, int offset = 0, int limit = 100 );

    /**
     * Returns the value of a particular data source, table and variable name
     * @param dataSource string
     * @param table string
     * @param variable string
     */
    void GetValue( std::string dataSource, std::string table, std::string variable );

  protected:
    OpalService();
    ~OpalService() {}

    /**
     * Returns the response provided by Opal for a given service path
     * @param servicePath string
     * @throws runtime_error
     */
    virtual Json::Value Read( std::string servicePath );

    std::map< std::string,std::map< std::string,std::map< std::string, std::string > > > Columns;
    std::string Username;
    std::string Password;
    std::string Host;
    int Port;

  private:
    OpalService( const OpalService& ); // Not implemented
    void operator=( const OpalService& ); // Not implemented
  };
}

/** @} end of doxygen group */

#endif
