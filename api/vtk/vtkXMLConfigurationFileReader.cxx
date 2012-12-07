/*=========================================================================

  Program:  Birch (CLSA Retinal Image Viewer)
  Module:   vtkXMLConfigurationFileReader.cpp
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>
  Author: Dean Inglis <inglisd@mcmaster.ca>

=========================================================================*/
#include "vtkXMLConfigurationFileReader.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkVariantArray.h"

#include <algorithm>
#include <stdexcept>

vtkStandardNewMacro( vtkXMLConfigurationFileReader );

// this undef is required on the hp. vtkMutexLock ends up including
// /usr/inclue/dce/cma_ux.h which has the gall to #define read as cma_read
#ifdef read
#undef read
#endif

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
int vtkXMLConfigurationFileReader::ProcessRequest(
  vtkInformation *request,
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  if( request->Has( vtkDemandDrivenPipeline::REQUEST_DATA_OBJECT() ) )
  {
    return this->RequestDataObject( request, inputVector, outputVector );
  }
  else if( request->Has( vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT() ) )
  {
    return 1;
  }
  else if( request->Has( vtkDemandDrivenPipeline::REQUEST_INFORMATION() ) )
  {
    return 1;
  }
  if( request->Has( vtkDemandDrivenPipeline::REQUEST_DATA() ) )
  {
    vtkDebugMacro( << "Reading database file ...");

    try
    {
      std::runtime_error e( "Error reading database file." );

      this->CreateReader();
      
      // parse until we find the session element
      while( this->ParseNode() )
        if( XML_READER_TYPE_ELEMENT == this->CurrentNode.NodeType &&
            0 == xmlStrcasecmp( BAD_CAST "Configuration", this->CurrentNode.Name ) ) break;

      // if we never found the configuration element then throw an exception
      if( XML_READER_TYPE_ELEMENT != this->CurrentNode.NodeType ||
          0 != xmlStrcasecmp( BAD_CAST "Configuration", this->CurrentNode.Name ) )
        throw std::runtime_error( "File does not contain a Configuration element." );

      std::string str;

      int depth = this->CurrentNode.Depth;
      while( this->ParseNode() )
      {
        // loop until we find the closing element at the same depth
        if( XML_READER_TYPE_END_ELEMENT == this->CurrentNode.NodeType &&
            depth == this->CurrentNode.Depth ) break;

        if( XML_READER_TYPE_ELEMENT == this->CurrentNode.NodeType &&
            0 < xmlStrlen( this->CurrentNode.Name ) )
        { // new configuration category
          std::map< std::string, std::string > map;

          // now find all key-value pairs in this category
          while( this->ParseNode() )
          {
            // loop until we find the closing element at the same depth
            if( XML_READER_TYPE_END_ELEMENT == this->CurrentNode.NodeType &&
                depth + 1 == this->CurrentNode.Depth ) break;

            if( XML_READER_TYPE_ELEMENT == this->CurrentNode.NodeType )
            {
              this->ReadValue( str );
              map.insert( std::pair< std::string, std::string >( (char *)this->CurrentNode.Name, str ) );
            }
          }

          this->Settings.insert(
            std::pair< std::string, std::map< std::string, std::string > >(
              (char *)this->CurrentNode.Name, map ) );
        }
      }

      this->FreeReader();
    }
    catch( std::exception &e )
    {
      this->FreeReader();
      throw e;
    }
  }

  return this->Superclass::ProcessRequest( request, inputVector, outputVector );
}
