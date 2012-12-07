/*=========================================================================

  Program:  Birch (CLSA Ultrasound Image Viewer)
  Module:   vtkXMLSessionFileWriter.cpp
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/
#include "vtkXMLSessionFileWriter.h"

#include "Session.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkDataSetAttributes.h"
#include "vtkDoubleArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkIntArray.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkVariantArray.h"

#include <stdexcept>

vtkStandardNewMacro( vtkXMLSessionFileWriter );

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLSessionFileWriter::WriteContents()
{
  Session *input = Session::SafeDownCast( this->GetInput() );
  this->StartElement( "Session" );
  this->Write( "Camera", input->GetCamera() );
  this->EndElement();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLSessionFileWriter::Write( const char* name, vtkCamera* camera )
{
  if( NULL == camera ) return;
  std::runtime_error e( "Error writing camera to CLSAVis session file." );

  this->StartElement( name );
  this->WriteTuple( "ClippingRange", camera->GetClippingRange(), 2 );
  this->WriteValue( "Distance", camera->GetDistance() );
  this->WriteValue( "FocalDisk", camera->GetFocalDisk() );
  this->WriteTuple( "FocalPoint", camera->GetFocalPoint(), 3 );
  this->WriteTuple( "ViewShear", camera->GetViewShear(), 3 );
  this->WriteValue( "ParallelProjection", camera->GetParallelProjection() );
  this->WriteValue( "ParallelScale", camera->GetParallelScale() );
  this->WriteTuple( "Position", camera->GetPosition(), 3 );
  this->WriteValue( "LeftEye", camera->GetLeftEye() );
  this->WriteValue( "Thickness", camera->GetThickness() );
  this->WriteValue( "ViewAngle", camera->GetViewAngle() );
  this->WriteValue( "UseHorizontalViewAngle", camera->GetUseHorizontalViewAngle() );
  this->WriteTuple( "ViewUp", camera->GetViewUp(), 3 );
  this->WriteTuple( "WindowCenter", camera->GetWindowCenter(), 2 );
  this->WriteTuple( "ScreenBottomLeft", camera->GetScreenBottomLeft(), 3 );
  this->WriteTuple( "ScreenBottomRight", camera->GetScreenBottomRight(), 3 );
  this->WriteTuple( "ScreenTopRight", camera->GetScreenTopRight(), 3 );
  this->EndElement(); // close the Camera element
}
