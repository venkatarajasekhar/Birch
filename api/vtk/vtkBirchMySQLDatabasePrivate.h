#ifndef __vtkBirchMySQLDatabasePrivate_h
#define __vtkBirchMySQLDatabasePrivate_h

#ifdef _WIN32
# include <winsock.h> // mysql.h relies on the typedefs from here
#endif

#include <mysql.h> // needed for MYSQL typedefs

class vtkBirchMySQLDatabasePrivate
{
public:
  vtkBirchMySQLDatabasePrivate() :
    Connection( NULL )
  {
  mysql_init( &this->NullConnection );
  }

  MYSQL NullConnection;
  MYSQL *Connection;
};

#endif // __vtkBirchMySQLDatabasePrivate_h
