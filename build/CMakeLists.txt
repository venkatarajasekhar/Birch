CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( Birch )

# Set the version
SET( BIRCH_VERSION_MAJOR 0 )
SET( BIRCH_VERSION_MINOR 1 )
SET( BIRCH_VERSION_PATCH 0 )

# Icons sizes to build
SET( BIRCH_ICON_SIZES 16x16 22x22 24x24 32x32 48x48 64x64 128x128 )

# Look in the build directory for cmake modules
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/" )

# Create a salt string which can be set at build time
SET( BIRCH_SALT_STRING "this is salt" CACHE STRING "This is salt for encrypting passwords")

# We need VTK
FIND_PACKAGE( VTK REQUIRED )
INCLUDE( ${VTK_USE_FILE} )

# We need GDCM
FIND_PACKAGE( GDCM REQUIRED )
INCLUDE( ${GDCM_USE_FILE} )

# We need QT4
SET( QT_MIN_VERSION 4.0.0 )
FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

# We need libxml2
FIND_PACKAGE( LibXml2 REQUIRED )

# We need Crypto++
FIND_PACKAGE( CRYPTO++ REQUIRED )

# We need JSONCpp
FIND_PACKAGE( JSONCpp REQUIRED )

# We need MySQL
FIND_PACKAGE( MySQL REQUIRED )

# We need convert
IF( UNIX AND NOT APPLE )
  FIND_PACKAGE( ImageMagick COMPONENTS convert REQUIRED )
ENDIF( UNIX AND NOT APPLE )

SET( BIRCH_ROOT_DIR ${PROJECT_SOURCE_DIR}/.. )
SET( BIRCH_AUX_DIR ${BIRCH_ROOT_DIR}/aux )
SET( BIRCH_API_DIR ${BIRCH_ROOT_DIR}/api )
SET( BIRCH_MODEL_DIR ${BIRCH_API_DIR}/model )
SET( BIRCH_QT_DIR ${BIRCH_API_DIR}/interface/qt )
SET( BIRCH_VTK_DIR ${BIRCH_API_DIR}/vtk )
SET( BIRCH_CONFIG_FILE ${PROJECT_BINARY_DIR}/config.xml )
SET( BIRCH_OPAL_SCRIPT ${PROJECT_BINARY_DIR}/opal.py )
SET( BIRCH_DOC_DIR ${BIRCH_ROOT_DIR}/doc )
SET( BIRCH_DOXY_DIR ${PROJECT_BINARY_DIR}/doxygen )

# Configure the utitlities header
CONFIGURE_FILE( ${BIRCH_API_DIR}/Utilities.h.in
                ${PROJECT_BINARY_DIR}/Utilities.h @ONLY IMMEDIATE )

# Copy the readme file to build
CONFIGURE_FILE( ${BIRCH_ROOT_DIR}/readme
                ${PROJECT_BINARY_DIR}/readme COPYONLY )

# Copy the config xml file to the build path (if not found)
IF( NOT EXISTS ${BIRCH_CONFIG_FILE} )
  CONFIGURE_FILE( ${BIRCH_AUX_DIR}/config.xml
                  ${BIRCH_CONFIG_FILE} COPYONLY )
ENDIF( NOT EXISTS ${BIRCH_CONFIG_FILE} )

# Copy the opal script to the build path (if not found)
IF( NOT EXISTS ${BIRCH_OPAL_SCRIPT} )
  CONFIGURE_FILE( ${BIRCH_AUX_DIR}/opal.py
                  ${BIRCH_OPAL_SCRIPT} COPYONLY )
ENDIF( NOT EXISTS ${BIRCH_OPAL_SCRIPT} )

# Set up include directories
SET( BIRCH_INCLUDE_DIR
  ${BIRCH_MODEL_DIR}
  ${BIRCH_QT_DIR}
  ${BIRCH_VTK_DIR}
)

SET( BIRCH_SOURCE
  ${BIRCH_API_DIR}/Birch.cxx

  ${BIRCH_MODEL_DIR}/ActiveRecord.cxx
  ${BIRCH_MODEL_DIR}/Configuration.cxx
  ${BIRCH_MODEL_DIR}/Database.cxx
  ${BIRCH_MODEL_DIR}/Image.cxx
  ${BIRCH_MODEL_DIR}/ModelObject.cxx
  ${BIRCH_MODEL_DIR}/OpalService.cxx
  ${BIRCH_MODEL_DIR}/Rating.cxx
  ${BIRCH_MODEL_DIR}/Study.cxx
  ${BIRCH_MODEL_DIR}/User.cxx
  ${BIRCH_MODEL_DIR}/Application.cxx

  ${BIRCH_VTK_DIR}/vtkMedicalImageViewer.cxx
  ${BIRCH_VTK_DIR}/vtkBirchMySQLDatabase.cxx
  ${BIRCH_VTK_DIR}/vtkBirchMySQLQuery.cxx
  ${BIRCH_VTK_DIR}/vtkXMLFileReader.cxx
  ${BIRCH_VTK_DIR}/vtkXMLConfigurationFileReader.cxx
  
  ${BIRCH_QT_DIR}/QBirchApplication.cxx
  ${BIRCH_QT_DIR}/QAboutDialog.cxx
  ${BIRCH_QT_DIR}/QLoginDialog.cxx
  ${BIRCH_QT_DIR}/QMainBirchWindow.cxx
  ${BIRCH_QT_DIR}/QMedicalImageWidget.cxx
  ${BIRCH_QT_DIR}/QProgressDialog.cxx
  ${BIRCH_QT_DIR}/QSelectStudyDialog.cxx
  ${BIRCH_QT_DIR}/QUserListDialog.cxx
)

SET_SOURCE_FILES_PROPERTIES(
  ${BIRCH_MODEL_DIR}/ActiveRecord.cxx
  ${BIRCH_MODEL_DIR}/ModelObject.cxx

  ${BIRCH_VTK_DIR}/vtkXMLFileReader.cxx
  ${BIRCH_VTK_DIR}/vtkXMLFileWriter.cxx
  ABSTRACT )

SET( BIRCH_UIS
  ${BIRCH_QT_DIR}/QAboutDialog.ui
  ${BIRCH_QT_DIR}/QLoginDialog.ui
  ${BIRCH_QT_DIR}/QMainBirchWindow.ui
  ${BIRCH_QT_DIR}/QMedicalImageWidget.ui
  ${BIRCH_QT_DIR}/QProgressDialog.ui
  ${BIRCH_QT_DIR}/QSelectStudyDialog.ui
  ${BIRCH_QT_DIR}/QUserListDialog.ui
)

SET( BIRCH_HEADERS
  ${BIRCH_QT_DIR}/QAboutDialog.h
  ${BIRCH_QT_DIR}/QLoginDialog.h
  ${BIRCH_QT_DIR}/QMainBirchWindow.h
  ${BIRCH_QT_DIR}/QMedicalImageWidget.h
  ${BIRCH_QT_DIR}/QProgressDialog.h
  ${BIRCH_QT_DIR}/QSelectStudyDialog.h
  ${BIRCH_QT_DIR}/QUserListDialog.h
)

# Wrap QT files
QT4_WRAP_UI( UIHeaders ${BIRCH_UIS} )
QT4_WRAP_CPP( MOCSrcs ${BIRCH_HEADERS} )
ADD_DEFINITIONS( -DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT )
SET_SOURCE_FILES_PROPERTIES( ${BIRCH_SOURCE} PROPERTIES OBJECT_DEPENDS "${UIHeaders}" )

# Use the include path and library for Qt
INCLUDE_DIRECTORIES(
  ${BIRCH_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CRYPTO++_INCLUDE_DIR}
  ${JSONCPP_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIRECTORIES}
)

# Targets
ADD_EXECUTABLE( birch ${BIRCH_SOURCE} ${BIRCH_UISrcs} ${MOCSrcs} )

TARGET_LINK_LIBRARIES( birch
  QVTK
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkgdcm
  ${QT_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  ${CRYPTO++_LIBRARIES}
  ${JSONCPP_LIBRARIES}
)
INSTALL( TARGETS birch RUNTIME DESTINATION bin )

ADD_CUSTOM_TARGET( dist
  COMMAND git archive --prefix=${BIRCH_ARCHIVE_NAME}/ HEAD
    | bzip2 > ${CMAKE_BINARY_DIR}/${BIRCH_ARCHIVE_NAME}.tar.bz2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Desktop entries and icons
IF( UNIX AND NOT APPLE )
  INSTALL( FILES desktop/birch.desktop DESTINATION share/applications )

  INSTALL( FILES desktop/logo_clsa.png DESTINATION share/icons/hicolor/scalable/apps )
  FOREACH( SIZE ${BIRCH_ICON_SIZES} )
    ADD_CUSTOM_COMMAND( OUTPUT logo_clsa_${SIZE}.png
      COMMAND convert -background none -resize ${SIZE} ${BIRCH_AUX_DIR}/logo_clsa.png logo_clsa_${SIZE}.png
      COMMENT "Generating ${SIZE} icon"
    )
    SET( BIRCH_ICON_FILES ${BIRCH_ICON_FILES} logo_clsa_${SIZE}.png )
    INSTALL( FILES ${CMAKE_BINARY_DIR}/logo_clsa_${SIZE}.png
      DESTINATION share/icons/hicolor/${SIZE}/apps
      RENAME logo_clsa.png
    )
  ENDFOREACH( SIZE )
  ADD_CUSTOM_TARGET( icons ALL DEPENDS ${BIRCH_ICON_FILES} )
ENDIF( UNIX AND NOT APPLE )

# Build doxygen documentation ?
INCLUDE (${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL)
IF( BUILD_DOCUMENTATION )

  SET( HAVE_DOT_YESNO NO )
  IF( DOT )
    SET( HAVE_DOT_YESNO YES )
    IF( NOT DOT_PATH )
      GET_FILENAME_COMPONENT( DOT_PATH ${DOT} PATH )
    ENDIF( NOT DOT_PATH )
  ENDIF( DOT )

  CONFIGURE_FILE(
    ${BIRCH_DOC_DIR}/doc_mainpage.dox.in
    ${BIRCH_DOXY_DIR}/doc_mainpage.dox )

  CONFIGURE_FILE(
    ${BIRCH_DOC_DIR}/doc_makeall.cmake.in
    ${BIRCH_DOXY_DIR}/doc_makeall.cmake
    @ONLY )
  
  CONFIGURE_FILE(
    ${BIRCH_DOC_DIR}/doc_mainpage.dox.in
    ${BIRCH_DOXY_DIR}/doc_mainpage.dox )

  SET( DOXY_INPUT_SOURCE 
    ${BIRCH_API_DIR}
    ${BIRCH_MODEL_DIR}
    ${BIRCH_VTK_DIR}
    ${BIRCH_QT_DIR}
    ${BIRCH_DOXY_DIR}/doc_mainpage.dox )

  string( REPLACE ";" "\" \"" BIRCH_DOXY_LIST "${DOXY_INPUT_SOURCE}" )
  
  CONFIGURE_FILE(
    ${BIRCH_DOC_DIR}/config.dox.in
    ${BIRCH_DOXY_DIR}/config.dox )

# Uncommenting this block will force a build of the documentation
# every time cmake is run in the build directory
# 
#  execute_process(COMMAND 
#    ${CMAKE_COMMAND} -P ${BIRCH_DOXY_DIR}/doc_makeall.cmake
#    DEPENDS ${BIRCH_DOXY_DIR}/doc_makeall.cmake 
#    WORKING DIR ${BIRCH_DOXY_DIR}/doxygen 
#    RESULT_VARIABLE rv )

ENDIF( BUILD_DOCUMENTATION )
