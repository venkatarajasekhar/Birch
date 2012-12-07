/*=========================================================================

  Program:  Birch (CLSA Ultrasound Image Viewer)
  Module:   QBirchApplication.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/

#ifndef __QBirchApplication_h
#define __QBirchApplication_h

#include <QApplication>

#include "Utilities.h"

class Ui_QBirchApplication;

class QBirchApplication : public QApplication
{
public:
  QBirchApplication( int &argc, char **argv ) : QApplication( argc, argv ) {}
  bool notify( QObject *pObject, QEvent *pEvent );
};

#endif
