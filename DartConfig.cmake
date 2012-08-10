#
# Dart server to submit results (used by client)
#
set(NIGHTLY_START_TIME "21:00:00 EDT")

#set(DROP_SITE "degas.rad.unc.edu")
#set(DROP_LOCATION "/DartClient/BatchMake")
#set(DROP_SITE_USER "dart")
#set(DROP_SITE_PASSWORD "DartClient")
#set(TRIGGER_SITE "http://caddlab.rad.unc.edu/cgi-bin/Dart-BatchMake.pl")

set(DROP_METHOD http)
set(DROP_SITE "www.cdash.org")
#set(DROP_LOCATION "/cgi-bin/HTTPUploadDartFile.cgi")
set(DROP_LOCATION "/CDash/submit.php?project=BatchMake")
set(TRIGGER_SITE
  "http://${DROP_SITE}/cgi-bin/Submit-BatchMake-TestingResults.cgi")



# Project Home Page
set(PROJECT_URL "http://public.kitware.com/BatchMake")

set(ROLLUP_URL "http://${DROP_SITE}/cgi-bin/BatchMake-rollup-dashboard.sh")

#
# Dart server configuration
#
set(CVS_WEB_URL "http://public.kitware.com/cgi-bin/viewcvs.cgi/")
set(CVS_WEB_CVSROOT "BatchMake")

set(USE_DOXYGEN "Off")
set(USE_GNATS "Off")

#
# Copy over the testing logo
#
configure_file(${BatchMake_SOURCE_DIR}/Images/logosmall.gif ${BatchMake_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif COPYONLY)
