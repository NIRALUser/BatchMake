#
# Dart server to submit results (used by client)
#
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
SET (CVS_WEB_URL "http://caddlab.rad.unc.edu/cgi-bin/cvsweb.cgi/BatchMake/")

OPTION(BUILD_DOXYGEN "Build source documentation using doxygen" "On")
SET (DOXYGEN_CONFIG "${PROJECT_BINARY_DIR}/doxygen.config" )
SET (USE_DOXYGEN "On")
SET (DOXYGEN_URL "http://caddlab.rad.unc.edu/Public/Dashboard/Documentation/Doxygen/html/" )


#SET (DOXYGEN_URL "http://${DROP_SITE}/" )
#SET (GNATS_WEB_URL "http://${DROP_SITE}/")

#
# Copy over the testing logo
#
CONFIGURE_FILE(${BatchMake_SOURCE_DIR}/Images/Logo.gif ${BatchMake_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif COPYONLY)
