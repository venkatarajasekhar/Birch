/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   vtkXMLFileReader.cpp
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/
#include "vtkXMLFileReader.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <stdexcept>

// this undef is required on the hp. vtkMutexLock ends up including
// /usr/inclue/dce/cma_ux.h which has the gall to #define read as cma_read
#ifdef read
#undef read
#endif

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
vtkXMLFileReader::vtkXMLFileReader()
{
  this->FileName = "";
  this->Reader = NULL;
  this->SetNumberOfInputPorts( 0 );
  this->SetNumberOfOutputPorts( 1 );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
vtkXMLFileReader::~vtkXMLFileReader()
{
  if( NULL != this->Reader )
  {
    this->FreeReader();
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
int vtkXMLFileReader::ProcessRequest(
  vtkInformation *request,
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  return this->Superclass::ProcessRequest( request, inputVector, outputVector );
}


//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLFileReader::PrintSelf( ostream &os, vtkIndent indent )
{
  Superclass::PrintSelf( os, indent );

  os << indent << "FileName: \"" << this->FileName << "\"" << endl;
  os << indent << "Reader: " << this->Reader << endl;
  os << indent << "CurrentNode: " << endl;
  this->CurrentNode.PrintSelf( os, indent.GetNextIndent() );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLFileReader::SetFileName( std::string filename )
{
  vtkDebugMacro( << this->GetClassName() << " (" << this << "): setting "
                 << "FileName to " << filename.c_str() );

  if( filename != this->FileName )
  {
    this->FileName = filename;
    this->Modified();
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLFileReader::CreateReader()
{
  this->Reader = xmlReaderForFile( this->FileName.c_str(), NULL, 0 );
  if( NULL == this->Reader )
  {
    throw std::runtime_error( "Unable to open file." );
  }
  this->CurrentNode.Clear();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
int vtkXMLFileReader::ParseNode()
{
  if( NULL == this->Reader )
  {
    throw std::runtime_error( "No file opened." );
  }

  int result = xmlTextReaderRead( this->Reader );
  if( -1 == result )
  { // error
    throw std::runtime_error( "Parse error." );
  }
  else if( 0 == result )
  { // end of file
    this->CurrentNode.Clear();
  }
  else
  { // successful read
    this->CurrentNode.Name = xmlTextReaderConstName( this->Reader );
    this->CurrentNode.Depth = xmlTextReaderDepth( this->Reader );
    this->CurrentNode.AttributeCount = xmlTextReaderAttributeCount( this->Reader );
    this->CurrentNode.NodeType = xmlTextReaderNodeType( this->Reader );
    this->CurrentNode.IsEmptyElement = xmlTextReaderIsEmptyElement( this->Reader );
    this->CurrentNode.HasContent = xmlTextReaderHasValue( this->Reader );
    this->CurrentNode.Content = xmlTextReaderConstValue( this->Reader );
  }

  return result;
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLFileReader::RewindReader()
{
  if( NULL == this->Reader )
  {
    throw std::runtime_error( "No file opened." );
  }
  
  // close and reopen the current file
  this->FreeReader();
  this->CreateReader();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLFileReader::FreeReader()
{
  if( NULL == this->Reader )
  {
    throw std::runtime_error( "No file opened." );
  }

  xmlFreeTextReader( this->Reader );
  this->Reader = NULL;
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void vtkXMLFileReader::ReadValue( std::string& value )
{
  // list of expected elements
  xmlChar *read = xmlTextReaderReadString( this->Reader );
  if( NULL == read )
  {
    std::string error = "Failed to read ";
    error += vtkVariant( value ).GetTypeAsString();
    throw( std::runtime_error( error ) );
  }
  value = ( char* ) read;
  delete [] read;

  // TODO: warn if attribute "type" doesn't match "string"
}
