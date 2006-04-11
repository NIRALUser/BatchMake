#
# Dart server to submit results (used by client)
#
SET (NIGHTLY_START_TIME "21:00:00 EDT")

#SET (DROP_SITE "degas.rad.unc.edu")
#SET (DROP_LOCATION "/DartClient/BatchMake")
#SET (DROP_SITE_USER "dart")
#SET (DROP_SITE_PASSWORD "DartClient")
#SET (TRIGGER_SITE "http://caddlab.rad.unc.edu/cgi-bin/Dart-BatchMake.pl")

SET(DROP_METHOD http)
SET (DROP_SITE "public.kitware.com")
SET (DROP_LOCATION "/cgi-bin/HTTPUploadDartFile.cgi")
SET (TRIGGER_SITE 
  "http://${DROP_SITE}/cgi-bin/Submit-BatchMake-TestingResults.cgi")

# Project Home Page
SET (PROJECT_URL "http://public.kitware.com/BatchMake")

#
# Dart server configuration 
#
SET (CVS_WEB_URL "http://caddlab.rad.unc.edu/cgi-bin/cvsweb.cgi/?cvsroot=BatchMake")

OPTION(BUILD_DOXYGEN "Build source documentation using doxygen" "On")
SET (DOXYGEN_CONFIG "${PROJECT_BINARY_DIR}/doxygen.config" )
SET (USE_DOXYGEN "Off")
SET (DOXYGEN_URL "http://caddlab.rad.unc.edu/Public/Dashboard/BatchMake-Dashboard/Documentation/Doxygen/html/" )
SET (USE_GNATS "On")
SET (GNATS_WEB_URL "http://www.itk.org/Bug")

#
# Copy over the testing logo
#
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Images/logosmall.gif ${BatchMake_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif COPYONLY)
