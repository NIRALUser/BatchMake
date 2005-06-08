#
# Dart server to submit results (used by client)
#
SET (NIGHTLY_START_TIME "21:00:00 EDT")

SET (DROP_SITE "degas.rad.unc.edu")
SET (DROP_LOCATION "DartClient")
SET (DROP_SITE_USER "dart")
SET (DROP_SITE_PASSWORD "DartClient")
SET (TRIGGER_SITE "http://caddlab.rad.unc.edu/cgi-bin/Dart-BatchMake.pl")

# Project Home Page
SET (PROJECT_URL "http://caddlab.rad.unc.edu/Public")

#
# Dart server configuration 
#
SET (CVS_WEB_URL "http://caddlab.rad.unc.edu/cgi-bin/cvsweb.cgi/?cvsroot=BatchMake")

OPTION(BUILD_DOXYGEN "Build source documentation using doxygen" "On")
SET (DOXYGEN_CONFIG "${PROJECT_BINARY_DIR}/doxygen.config" )
SET (USE_DOXYGEN "On")
SET (DOXYGEN_URL "http://caddlab.rad.unc.edu/Public/Dashboard/BatchMake-Dashboard/Documentation/Doxygen/html/" )
SET (USE_GNATS "On")
SET (GNATS_WEB_URL "http://caddlab.rad.unc.edu/software/itkUNC/Bug/index.php")

#
# Copy over the testing logo
#
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Images/logosmall.gif ${BatchMake_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif COPYONLY)
