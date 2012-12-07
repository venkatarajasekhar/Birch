/*=========================================================================

  Program:  Birch (CLSA Retinal Image Viewer)
  Module:   QMainBirchWindow.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/

#ifndef __QMainBirchWindow_h
#define __QMainBirchWindow_h

#include <QMainWindow>

#include "Utilities.h"

#include "vtkSmartPointer.h"

#include <map>

namespace Birch { class ActiveRecord; };
class Ui_QMainBirchWindow;
class QTreeWidgetItem;

class QMainBirchWindow : public QMainWindow
{
  Q_OBJECT

public:
  QMainBirchWindow( QWidget* parent = 0 );
  ~QMainBirchWindow();

  
public slots:
  // action event functions
  virtual void slotOpenStudy();
  virtual void slotPreviousStudy();
  virtual void slotNextStudy();
  virtual void slotLogin();
  virtual void slotUserManagement();
  virtual void slotUpdateStudyDatabase();
  virtual void slotTreeSelectionChanged();
  virtual void slotRatingSliderChanged( int );

  // help event functions
  virtual void slotAbout();
  virtual void slotManual();

protected:
  // called whenever the main window is closed
  virtual void closeEvent( QCloseEvent *event );

  // read/write application GUI settings
  virtual void readSettings();
  virtual void writeSettings();
  virtual void updateStudyInformation();
  virtual void updateStudyTreeWidget();
  virtual void updateMedicalImageWidget();
  virtual void updateRating();
  virtual void updateInterface();

  std::map< QTreeWidgetItem*, vtkSmartPointer<Birch::ActiveRecord> > treeModelMap;

protected slots:

private:
  // Designer form
  Ui_QMainBirchWindow *ui;
};

#endif
