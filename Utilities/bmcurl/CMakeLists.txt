# Code in this directory is identical to the code in the CURL library that
#   is included with CMake.   This CMakeLists.txt file has been extended
#   to support the external use of the library via CMake and to support the
#   integration with Slicer.   These changes have not been committed back to
#   the CMake project.
# This Project/CMakeLists.txt is odd in that the library it produces is
#   called bmcurl but CMake project is called LIBCURL.  This was maintained to
#   keep compatibility with CMake's CURL distribution.

cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(LIBCURL C)

#-----------------------------------------------------------------------------
# Disable MSVC 8 warnings
if(WIN32)
  option(DISABLE_MSVC8_DEPRECATED_WARNINGS
    "Disable Visual Studio 8 deprecated warnings" ON)
  mark_as_advanced(FORCE DISABLE_MSVC8_DEPRECATED_WARNINGS)
  if(DISABLE_MSVC8_DEPRECATED_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  endif(DISABLE_MSVC8_DEPRECATED_WARNINGS)
endif(WIN32)

#-----------------------------------------------------------------------------
# CMakeLists.txt custom to Slicer...

if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBCURL_BINARY_DIR}/bin CACHE PATH
      "Single output directory for building all libraries.")
endif(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
mark_as_advanced(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)

if(NOT LIBRARY_OUTPUT_DIRECTORY)
  set(LIBRARY_OUTPUT_DIRECTORY ${LIBCURL_BINARY_DIR}/bin CACHE PATH
      "Single output directory for building all libraries.")
endif(NOT LIBRARY_OUTPUT_DIRECTORY)
mark_as_advanced(LIBRARY_OUTPUT_DIRECTORY)

set(LIBCURL_LIBRARY_DIR "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

set(LIBCURL_RUNTIME_DIR "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

set(LIBCURL_INCLUDE_PATH "${LIBCURL_SOURCE_DIR}/curl")


if(WIN32)
  set(LIBCURL_LIBRARIES bmcurl Winmm.lib)
else(WIN32)
  set(LIBCURL_LIBRARIES z bmcurl)
endif(WIN32)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBCURL_BINARY_DIR}/bin CACHE PATH
      "Single output directory for building all executables.")
endif(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
mark_as_advanced(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

# More CMakeLists.txt custom to Slicer at end of file
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# CMakeLists.txt from CMake's curl library...

include_regular_expression("^.*$")

# Setup package meta-data
set(PACKAGE "curl")
set(VERSION "7.12.1")
set(PACKAGE_TARNAME "curl")
set(PACKAGE_BUGREPORT " ")
set(PACKAGE_NAME "curl")
set(PACKAGE_VERSION "-")
set(PACKAGE_STRING "curl-")
set(PACKAGE_BUGREPORT "a suitable curl mailing list => http://curl.haxx.se/mail/")
set(OPERATING_SYSTEM "${CMAKE_SYSTEM_NAME}")

# We need ansi c-flags, especially on HP
set(CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_REQUIRED_FLAGS ${CMAKE_ANSI_CFLAGS})

# Disable warnings on Borland to avoid changing 3rd party code.
if(BORLAND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w-")
endif(BORLAND)

# If we are on AIX, do the _ALL_SOURCE magic
if(${CMAKE_SYSTEM_NAME} MATCHES AIX)
  set(_ALL_SOURCE 1)
endif(${CMAKE_SYSTEM_NAME} MATCHES AIX)

# Include all the necessary files for macros
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)
include(curlCheckCAcceptsFlags)

set(libCurl_SRCS
  base64.c
  connect.c
  content_encoding.c
  cookie.c
  dict.c
  easy.c
  escape.c
  file.c
  formdata.c
  ftp.c
  getdate.c
  getenv.c
  getinfo.c
  hash.c
  hostares.c
  hostasyn.c
  hostip.c
  hostip4.c
  hostip6.c
  hostsyn.c
  hostthre.c
  http.c
  http_chunks.c
  http_digest.c
  http_negotiate.c
  http_ntlm.c
  if2ip.c
  inet_ntop.c
  inet_pton.c
  krb4.c
  llist.c
  md5.c
  memdebug.c
  mprintf.c
  multi.c
  netrc.c
  progress.c
  sendf.c
  share.c
  speedcheck.c
  ssluse.c
  strequal.c
  strerror.c
  telnet.c
  timeval.c
  transfer.c
  url.c
  version.c
  )

set(CURL_DISABLE_LDAP 1)
if(NOT CURL_DISABLE_LDAP)
  set(libCurl_SRCS
    ${libCurl_SRCS}
    ldap.c
    )
endif(NOT CURL_DISABLE_LDAP)

# if we have Kerberos 4, right now this is never on
#option(CURL_KRB4 "Use Kerberos 4" OFF)
if(CURL_KRB4)
  set(libCurl_SRCS ${libCurl_SRCS}
    krb4.c
    security.c
    )
endif(CURL_KRB4)

#option(CURL_MALLOC_DEBUG "Debug mallocs in Curl" OFF)
mark_as_advanced(CURL_MALLOC_DEBUG)
if(CURL_MALLOC_DEBUG)
  set(libCurl_SRCS ${libCurl_SRCS}
    memdebug.c
    )
endif(CURL_MALLOC_DEBUG)

# On windows preload settings
if(WIN32)
  include(${LIBCURL_SOURCE_DIR}/Platforms/WindowsCache.cmake)
endif(WIN32)

# This macro checks if the symbol exists in the library and if it
# does, it appends library to the list.
set(CURL_LIBS "")
macro(CHECK_LIBRARY_EXISTS_CONCAT LIBRARY SYMBOL VARIABLE)
  CHECK_LIBRARY_EXISTS("${LIBRARY};${CURL_LIBS}" ${SYMBOL} ""
    ${VARIABLE})
  if(${VARIABLE})
    set(CURL_LIBS ${CURL_LIBS} ${LIBRARY})
  endif(${VARIABLE})
endmacro(CHECK_LIBRARY_EXISTS_CONCAT)

# Check for all needed libraries
CHECK_LIBRARY_EXISTS_CONCAT("dl"     dlopen       HAVE_LIBDL)
CHECK_LIBRARY_EXISTS_CONCAT("ucb"    gethostname  HAVE_LIBUCB)
CHECK_LIBRARY_EXISTS_CONCAT("socket" connect      HAVE_LIBSOCKET)
CHECK_LIBRARY_EXISTS("c" gethostbyname "" NOT_NEED_LIBNSL)

if(NOT NOT_NEED_LIBNSL)
  CHECK_LIBRARY_EXISTS_CONCAT("nsl"    gethostbyname  HAVE_LIBNSL)
endif(NOT NOT_NEED_LIBNSL)

CHECK_LIBRARY_EXISTS_CONCAT("ws2_32" getch        HAVE_LIBWS2_32)
CHECK_LIBRARY_EXISTS_CONCAT("winmm"  getch        HAVE_LIBWINMM)
if(NOT CURL_SPECIAL_LIBZ)
  CHECK_LIBRARY_EXISTS_CONCAT("z"      inflateEnd   HAVE_LIBZ)
endif(NOT CURL_SPECIAL_LIBZ)

#option(CMAKE_USE_OPENSSL "Use OpenSSL code. Experimental" OFF)
mark_as_advanced(CMAKE_USE_OPENSSL)
if(CMAKE_USE_OPENSSL)
  CHECK_LIBRARY_EXISTS_CONCAT("crypto" CRYPTO_lock  HAVE_LIBCRYPTO)
  CHECK_LIBRARY_EXISTS_CONCAT("ssl"    SSL_connect  HAVE_LIBSSL)
endif(CMAKE_USE_OPENSSL)

# Check for symbol dlopen (same as HAVE_LIBDL)
CHECK_LIBRARY_EXISTS("${CURL_LIBS}" dlopen "" HAVE_DLOPEN)

# For other tests to use the same libraries
set(CMAKE_REQUIRED_LIBRARIES ${CURL_LIBS})

if(CURL_SPECIAL_LIBZ)
  set(CURL_LIBS ${CURL_LIBS} "${CURL_SPECIAL_LIBZ}")
  include_directories(${CURL_SPECIAL_LIBZ_INCLUDES})
  set(HAVE_LIBZ 0)
  set(HAVE_ZLIB_H 0)
endif(CURL_SPECIAL_LIBZ)


# If we have features.h, then do the _BSD_SOURCE magic
CHECK_INCLUDE_FILE("features.h"       HAVE_FEATURES_H)
if(HAVE_FEATURES_H)
  set_source_files_properties(
    cookie.c
    easy.c
    formdata.c
    getenv.c
    hash.c
    http.c
    if2ip.c
    mprintf.c
    multi.c
    sendf.c
    telnet.c
    transfer.c
    url.c
    COMPILE_FLAGS -D_BSD_SOURCE)
endif(HAVE_FEATURES_H)

# Check if header file exists and add it to the list.
macro(CHECK_INCLUDE_FILE_CONCAT FILE VARIABLE)
  CHECK_INCLUDE_FILES("${CURL_INCLUDES};${FILE}" ${VARIABLE})
  if(${VARIABLE})
    set(CURL_INCLUDES ${CURL_INCLUDES} ${FILE})
  endif(${VARIABLE})
endmacro(CHECK_INCLUDE_FILE_CONCAT)

# Check for header files
CHECK_INCLUDE_FILE_CONCAT("stdio.h"          HAVE_STDIO_H)
CHECK_INCLUDE_FILE_CONCAT("stddef.h"         HAVE_STDDEF_H)
CHECK_INCLUDE_FILE_CONCAT("sys/types.h"      HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CONCAT("inttypes.h"       HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE_CONCAT("alloca.h"         HAVE_ALLOCA_H)
CHECK_INCLUDE_FILE_CONCAT("arpa/inet.h"      HAVE_ARPA_INET_H)
CHECK_INCLUDE_FILE_CONCAT("dlfcn.h"          HAVE_DLFCN_H)
CHECK_INCLUDE_FILE_CONCAT("fcntl.h"          HAVE_FCNTL_H)
CHECK_INCLUDE_FILE_CONCAT("malloc.h"         HAVE_MALLOC_H)
CHECK_INCLUDE_FILE_CONCAT("memory.h"         HAVE_MEMORY_H)
CHECK_INCLUDE_FILE_CONCAT("netdb.h"          HAVE_NETDB_H)
CHECK_INCLUDE_FILE_CONCAT("sys/poll.h"       HAVE_SYS_POLL_H)
CHECK_INCLUDE_FILE_CONCAT("assert.h"         HAVE_ASSERT_H)
CHECK_INCLUDE_FILE_CONCAT("limits.h"         HAVE_LIMITS_H)

if(CMAKE_USE_OPENSSL)
  CHECK_INCLUDE_FILE_CONCAT("openssl/x509.h"   HAVE_OPENSSL_X509_H)
  CHECK_INCLUDE_FILE_CONCAT("openssl/engine.h" HAVE_OPENSSL_ENGINE_H)
  CHECK_INCLUDE_FILE_CONCAT("openssl/rsa.h"    HAVE_OPENSSL_RSA_H)
  CHECK_INCLUDE_FILE_CONCAT("openssl/crypto.h" HAVE_OPENSSL_CRYPTO_H)
  CHECK_INCLUDE_FILE_CONCAT("openssl/pem.h"    HAVE_OPENSSL_PEM_H)
  CHECK_INCLUDE_FILE_CONCAT("openssl/ssl.h"    HAVE_OPENSSL_SSL_H)
  CHECK_INCLUDE_FILE_CONCAT("openssl/err.h"    HAVE_OPENSSL_ERR_H)
  CHECK_INCLUDE_FILE_CONCAT("openssl/rand.h"   HAVE_OPENSSL_RAND_H)
endif(CMAKE_USE_OPENSSL)

if(NOT CURL_SPECIAL_LIBZ)
  CHECK_INCLUDE_FILE_CONCAT("zlib.h"           HAVE_ZLIB_H)
endif(NOT CURL_SPECIAL_LIBZ)
CHECK_INCLUDE_FILE_CONCAT("sys/socket.h"     HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILE_CONCAT("netinet/in.h"     HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILE_CONCAT("net/if.h"         HAVE_NET_IF_H)
CHECK_INCLUDE_FILE_CONCAT("netinet/if_ether.h"
  HAVE_NETINET_IF_ETHER_H)
CHECK_INCLUDE_FILE_CONCAT("netinet/tcp.h"
  HAVE_NETINET_TCP_H)
CHECK_INCLUDE_FILE_CONCAT("sys/select.h"    HAVE_SYS_SELECT_H)
CHECK_INCLUDE_FILE_CONCAT("utime.h"         HAVE_UTIME_H)
CHECK_INCLUDE_FILE_CONCAT("netinet/in.h"    HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILE_CONCAT("pwd.h"           HAVE_PWD_H)
CHECK_INCLUDE_FILE_CONCAT("sgtty.h"         HAVE_SGTTY_H)
CHECK_INCLUDE_FILE_CONCAT("stdint.h"        HAVE_STDINT_H)
CHECK_INCLUDE_FILE_CONCAT("stdlib.h"        HAVE_STDLIB_H)
CHECK_INCLUDE_FILE_CONCAT("string.h"        HAVE_STRING_H)
CHECK_INCLUDE_FILE_CONCAT("strings.h"       HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CONCAT("sys/param.h"     HAVE_SYS_PARAM_H)
CHECK_INCLUDE_FILE_CONCAT("sys/stat.h"      HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE_CONCAT("sys/time.h"      HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE_CONCAT("termios.h"       HAVE_TERMIOS_H)
CHECK_INCLUDE_FILE_CONCAT("termio.h"        HAVE_TERMIO_H)
CHECK_INCLUDE_FILE_CONCAT("io.h"            HAVE_IO_H)
CHECK_INCLUDE_FILE_CONCAT("time.h"          HAVE_TIME_H)
CHECK_INCLUDE_FILE_CONCAT("unistd.h"        HAVE_UNISTD_H)
CHECK_INCLUDE_FILE_CONCAT("sys/utime.h"     HAVE_SYS_UTIME_H)
CHECK_INCLUDE_FILE_CONCAT("winsock.h"       HAVE_WINSOCK_H)
CHECK_INCLUDE_FILE_CONCAT("sockio.h"        HAVE_SOCKIO_H)
CHECK_INCLUDE_FILE_CONCAT("sys/sockio.h"    HAVE_SYS_SOCKIO_H)
CHECK_INCLUDE_FILE_CONCAT("x509.h"          HAVE_X509_H)
CHECK_INCLUDE_FILE_CONCAT("setjmp.h"        HAVE_SETJMP_H)
CHECK_INCLUDE_FILE_CONCAT("signal.h"        HAVE_SIGNAL_H)
CHECK_INCLUDE_FILE_CONCAT("sys/ioctl.h"     HAVE_SYS_IOCTL_H)
CHECK_INCLUDE_FILE_CONCAT("sys/utsname.h"   HAVE_SYS_UTSNAME_H)
CHECK_TYPE_SIZE(size_t  SIZEOF_SIZE_T)
CHECK_TYPE_SIZE(ssize_t  SIZEOF_SSIZE_T)
CHECK_TYPE_SIZE("long long"  SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("long"  SIZEOF_LONG)
CHECK_TYPE_SIZE("__int64"  SIZEOF___INT64)
CHECK_TYPE_SIZE("long double"  SIZEOF_LONG_DOUBLE)
if(NOT HAVE_SIZEOF_SSIZE_T)
  if(SIZEOF_LONG EQUAL SIZEOF_SIZE_T)
    set(ssize_t long)
  endif(SIZEOF_LONG EQUAL SIZEOF_SIZE_T)
  if(NOT ssize_t AND SIZEOF___INT64 EQUAL SIZEOF_SIZE_T)
    set(ssize_t __int64)
  endif(NOT ssize_t AND SIZEOF___INT64 EQUAL SIZEOF_SIZE_T)
endif(NOT HAVE_SIZEOF_SSIZE_T)

if(HAVE_SIZEOF_LONG_LONG)
  set(HAVE_LONGLONG 1)
endif(HAVE_SIZEOF_LONG_LONG)

find_file(RANDOM_FILE urandom /dev)
mark_as_advanced(RANDOM_FILE)

# Check for some functions that are used
CHECK_SYMBOL_EXISTS(socket        "${CURL_INCLUDES}" HAVE_SOCKET)
CHECK_SYMBOL_EXISTS(poll          "${CURL_INCLUDES}" HAVE_POLL)
CHECK_SYMBOL_EXISTS(select        "${CURL_INCLUDES}" HAVE_SELECT)
CHECK_SYMBOL_EXISTS(strdup        "${CURL_INCLUDES}" HAVE_STRDUP)
CHECK_SYMBOL_EXISTS(strstr        "${CURL_INCLUDES}" HAVE_STRSTR)
CHECK_SYMBOL_EXISTS(strtok_r      "${CURL_INCLUDES}" HAVE_STRTOK_R)
CHECK_SYMBOL_EXISTS(strftime      "${CURL_INCLUDES}" HAVE_STRFTIME)
CHECK_SYMBOL_EXISTS(uname         "${CURL_INCLUDES}" HAVE_UNAME)
CHECK_SYMBOL_EXISTS(strcasecmp    "${CURL_INCLUDES}" HAVE_STRCASECMP)
CHECK_SYMBOL_EXISTS(stricmp       "${CURL_INCLUDES}" HAVE_STRICMP)
CHECK_SYMBOL_EXISTS(strcmpi       "${CURL_INCLUDES}" HAVE_STRCMPI)
CHECK_SYMBOL_EXISTS(strncmpi      "${CURL_INCLUDES}" HAVE_STRNCMPI)
if(NOT HAVE_STRNCMPI)
  set(HAVE_STRCMPI)
endif(NOT HAVE_STRNCMPI)
CHECK_SYMBOL_EXISTS(gethostbyaddr "${CURL_INCLUDES}" HAVE_GETHOSTBYADDR)
CHECK_SYMBOL_EXISTS(gettimeofday  "${CURL_INCLUDES}" HAVE_GETTIMEOFDAY)
CHECK_SYMBOL_EXISTS(inet_addr     "${CURL_INCLUDES}" HAVE_INET_ADDR)
CHECK_SYMBOL_EXISTS(inet_pton     "${CURL_INCLUDES}" HAVE_INET_PTON)
CHECK_SYMBOL_EXISTS(inet_ntoa     "${CURL_INCLUDES}" HAVE_INET_NTOA)
CHECK_SYMBOL_EXISTS(inet_ntoa_r   "${CURL_INCLUDES}" HAVE_INET_NTOA_R)
CHECK_SYMBOL_EXISTS(tcsetattr     "${CURL_INCLUDES}" HAVE_TCSETATTR)
CHECK_SYMBOL_EXISTS(tcgetattr     "${CURL_INCLUDES}" HAVE_TCGETATTR)
CHECK_SYMBOL_EXISTS(perror        "${CURL_INCLUDES}" HAVE_PERROR)
CHECK_SYMBOL_EXISTS(closesocket   "${CURL_INCLUDES}" HAVE_CLOSESOCKET)
CHECK_SYMBOL_EXISTS(setvbuf       "${CURL_INCLUDES}" HAVE_SETVBUF)
CHECK_SYMBOL_EXISTS(sigsetjmp     "${CURL_INCLUDES}" HAVE_SIGSETJMP)
CHECK_SYMBOL_EXISTS(getpass_r     "${CURL_INCLUDES}" HAVE_GETPASS_R)
CHECK_SYMBOL_EXISTS(strlcat       "${CURL_INCLUDES}" HAVE_STRLCAT)
CHECK_SYMBOL_EXISTS(getpwuid      "${CURL_INCLUDES}" HAVE_GETPWUID)
CHECK_SYMBOL_EXISTS(geteuid       "${CURL_INCLUDES}" HAVE_GETEUID)
CHECK_SYMBOL_EXISTS(utime         "${CURL_INCLUDES}" HAVE_UTIME)
if(CMAKE_USE_OPENSSL)
  CHECK_SYMBOL_EXISTS(RAND_status   "${CURL_INCLUDES}" HAVE_RAND_STATUS)
  CHECK_SYMBOL_EXISTS(RAND_screen   "${CURL_INCLUDES}" HAVE_RAND_SCREEN)
  CHECK_SYMBOL_EXISTS(RAND_egd      "${CURL_INCLUDES}" HAVE_RAND_EGD)
  CHECK_SYMBOL_EXISTS(CRYPTO_cleanup_all_ex_data "${CURL_INCLUDES}"
    HAVE_CRYPTO_CLEANUP_ALL_EX_DATA)
endif(CMAKE_USE_OPENSSL)
CHECK_SYMBOL_EXISTS(gmtime_r      "${CURL_INCLUDES}" HAVE_GMTIME_R)
CHECK_SYMBOL_EXISTS(localtime_r   "${CURL_INCLUDES}" HAVE_LOCALTIME_R)

CHECK_SYMBOL_EXISTS(gethostbyname   "${CURL_INCLUDES}" HAVE_GETHOSTBYNAME)
CHECK_SYMBOL_EXISTS(gethostbyname_r "${CURL_INCLUDES}" HAVE_GETHOSTBYNAME_R)
CHECK_SYMBOL_EXISTS(gethostbyaddr_r "${CURL_INCLUDES}" HAVE_GETHOSTBYADDR_R)

CHECK_SYMBOL_EXISTS(signal        "${CURL_INCLUDES}" HAVE_SIGNAL_FUNC)
CHECK_SYMBOL_EXISTS(SIGALRM       "${CURL_INCLUDES}" HAVE_SIGNAL_MACRO)
if(HAVE_SIGNAL_FUNC AND HAVE_SIGNAL_MACRO)
  set(HAVE_SIGNAL 1)
endif(HAVE_SIGNAL_FUNC AND HAVE_SIGNAL_MACRO)
CHECK_SYMBOL_EXISTS(uname         "${CURL_INCLUDES}" HAVE_UNAME)
CHECK_SYMBOL_EXISTS(strtoll       "${CURL_INCLUDES}" HAVE_STRTOLL)
CHECK_SYMBOL_EXISTS(_strtoi64     "${CURL_INCLUDES}" HAVE__STRTOI64)
CHECK_SYMBOL_EXISTS(strerror_r    "${CURL_INCLUDES}" HAVE_STRERROR_R)
CHECK_SYMBOL_EXISTS(siginterrupt  "${CURL_INCLUDES}" HAVE_SIGINTERRUPT)
CHECK_SYMBOL_EXISTS(perror        "${CURL_INCLUDES}" HAVE_PERROR)

# only build compat strtok if we need to
if(NOT HAVE_STRTOK_R)
  set(libCurl_SRCS ${libCurl_SRCS}
    strtok.c
    )
endif(NOT HAVE_STRTOK_R)
if(NOT HAVE_STRTOLL AND NOT HAVE__STRTOI64)
  set(libCurl_SRCS ${libCurl_SRCS}
    strtoofft.c
    )
endif(NOT HAVE_STRTOLL AND NOT HAVE__STRTOI64)

# sigaction and sigsetjmp are special. Use special mechanism for
# detecting those, but only if previous attempt failed.
if(HAVE_SIGNAL_H)
  CHECK_SYMBOL_EXISTS(sigaction "signal.h" HAVE_SIGACTION)
endif(HAVE_SIGNAL_H)

if(NOT HAVE_SIGSETJMP)
  if(HAVE_SETJMP_H)
    CHECK_SYMBOL_EXISTS(sigsetjmp "setjmp.h" HAVE_MACRO_SIGSETJMP)
    if(HAVE_MACRO_SIGSETJMP)
      set(HAVE_SIGSETJMP 1)
    endif(HAVE_MACRO_SIGSETJMP)
  endif(HAVE_SETJMP_H)
endif(NOT HAVE_SIGSETJMP)

# For other curl specific tests, use this macro.
macro(CURL_INTERNAL_TEST CURL_TEST)
  if("${CURL_TEST}" MATCHES "^${CURL_TEST}$")
    set(MACRO_CHECK_FUNCTION_DEFINITIONS
      "-D${CURL_TEST} ${CMAKE_REQUIRED_FLAGS}")
    if(CMAKE_REQUIRED_LIBRARIES)
      set(CURL_TEST_ADD_LIBRARIES
        "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    endif(CMAKE_REQUIRED_LIBRARIES)

    message(STATUS "Performing Curl Test ${CURL_TEST}")
    try_compile(${CURL_TEST}
      ${CMAKE_BINARY_DIR}
      ${LIBCURL_SOURCE_DIR}/CMake/CurlTests.c
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
      "${CURL_TEST_ADD_LIBRARIES}"
      OUTPUT_VARIABLE OUTPUT)
    if(${CURL_TEST})
      set(${CURL_TEST} 1 CACHE INTERNAL "Curl test ${FUNCTION}")
      message(STATUS "Performing Curl Test ${CURL_TEST} - Success")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Performing Curl Test ${CURL_TEST} passed with the following output:\n"
        "${OUTPUT}\n")
    else(${CURL_TEST})
      message(STATUS "Performing Curl Test ${CURL_TEST} - Failed")
      set(${CURL_TEST} "" CACHE INTERNAL "Curl test ${FUNCTION}")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
        "Performing Curl Test ${CURL_TEST} failed with the following output:\n"
        "${OUTPUT}\n")
    endif(${CURL_TEST})
  endif("${CURL_TEST}" MATCHES "^${CURL_TEST}$")
endmacro(CURL_INTERNAL_TEST)
macro(CURL_INTERNAL_TEST_RUN CURL_TEST)
  if("${CURL_TEST}_COMPILE" MATCHES "^${CURL_TEST}_COMPILE$")
    set(MACRO_CHECK_FUNCTION_DEFINITIONS
      "-D${CURL_TEST} ${CMAKE_REQUIRED_FLAGS}")
    if(CMAKE_REQUIRED_LIBRARIES)
      set(CURL_TEST_ADD_LIBRARIES
        "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    endif(CMAKE_REQUIRED_LIBRARIES)

    message(STATUS "Performing Curl Test ${CURL_TEST}")
    try_run(${CURL_TEST} ${CURL_TEST}_COMPILE
      ${CMAKE_BINARY_DIR}
      ${LIBCURL_SOURCE_DIR}/CMake/CurlTests.c
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
      "${CURL_TEST_ADD_LIBRARIES}"
      OUTPUT_VARIABLE OUTPUT)
    if(${CURL_TEST}_COMPILE AND NOT ${CURL_TEST})
      set(${CURL_TEST} 1 CACHE INTERNAL "Curl test ${FUNCTION}")
      message(STATUS "Performing Curl Test ${CURL_TEST} - Success")
    else(${CURL_TEST}_COMPILE AND NOT ${CURL_TEST})
      message(STATUS "Performing Curl Test ${CURL_TEST} - Failed")
      set(${CURL_TEST} "" CACHE INTERNAL "Curl test ${FUNCTION}")
      file(APPEND "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log"
        "Performing Curl Test ${CURL_TEST} failed with the following output:\n"
        "${OUTPUT}")
      if(${CURL_TEST}_COMPILE)
        file(APPEND
          "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log"
          "There was a running problem of this test\n")
      endif(${CURL_TEST}_COMPILE)
      file(APPEND "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log"
        "\n\n")
    endif(${CURL_TEST}_COMPILE AND NOT ${CURL_TEST})
  endif("${CURL_TEST}_COMPILE" MATCHES "^${CURL_TEST}_COMPILE$")
endmacro(CURL_INTERNAL_TEST_RUN)

# Do curl specific tests
#option(CURL_HAVE_DISABLED_NONBLOCKING "Disable non-blocking socket detection" OFF)
set(CURL_NONBLOCKING_TESTS)
if(NOT CURL_HAVE_DISABLED_NONBLOCKING)
  set(CURL_NONBLOCKING_TESTS
    HAVE_FIONBIO
    HAVE_IOCTLSOCKET
    HAVE_IOCTLSOCKET_CASE
    HAVE_O_NONBLOCK
    HAVE_SO_NONBLOCK
    )
endif(NOT CURL_HAVE_DISABLED_NONBLOCKING)
foreach(CURL_TEST
    ${CURL_NONBLOCKING_TESTS}
    TIME_WITH_SYS_TIME
    HAVE_O_NONBLOCKHAVE_GETHOSTBYADDR_R_5
    HAVE_GETHOSTBYADDR_R_7
    HAVE_GETHOSTBYADDR_R_8
    HAVE_GETHOSTBYADDR_R_5_REENTRANT
    HAVE_GETHOSTBYADDR_R_7_REENTRANT
    HAVE_GETHOSTBYADDR_R_8_REENTRANT
    HAVE_GETHOSTBYNAME_R_3
    HAVE_GETHOSTBYNAME_R_5
    HAVE_GETHOSTBYNAME_R_6
    HAVE_GETHOSTBYNAME_R_3_REENTRANT
    HAVE_GETHOSTBYNAME_R_5_REENTRANT
    HAVE_GETHOSTBYNAME_R_6_REENTRANT
    HAVE_SOCKLEN_T
    HAVE_IN_ADDR_T
    STDC_HEADERS
    RETSIGTYPE_TEST
    HAVE_INET_NTOA_R_DECL
    HAVE_INET_NTOA_R_DECL_REENTRANT
    HAVE_GETADDRINFO
    HAVE_FILE_OFFSET_BITS
    )
  CURL_INTERNAL_TEST(${CURL_TEST})
endforeach(CURL_TEST)
if(HAVE_FILE_OFFSET_BITS)
  set(_FILE_OFFSET_BITS 64)
endif(HAVE_FILE_OFFSET_BITS)

foreach(CURL_TEST
    HAVE_GLIBC_STRERROR_R
    HAVE_POSIX_STRERROR_R
    HAVE_LONG_LONG_CONSTANT
    )
  CURL_INTERNAL_TEST_RUN(${CURL_TEST})
endforeach(CURL_TEST)

# Check for reentrant
foreach(CURL_TEST
    HAVE_GETHOSTBYADDR_R_5
    HAVE_GETHOSTBYADDR_R_7
    HAVE_GETHOSTBYADDR_R_8
    HAVE_GETHOSTBYNAME_R_3
    HAVE_GETHOSTBYNAME_R_5
    HAVE_GETHOSTBYNAME_R_6
    HAVE_INET_NTOA_R_DECL_REENTRANT)
  if(NOT ${CURL_TEST})
    if(${CURL_TEST}_REENTRANT)
      set(NEED_REENTRANT 1)
    endif(${CURL_TEST}_REENTRANT)
  endif(NOT ${CURL_TEST})
endforeach(CURL_TEST)

if(NEED_REENTRANT)
  foreach(CURL_TEST
      HAVE_GETHOSTBYADDR_R_5
      HAVE_GETHOSTBYADDR_R_7
      HAVE_GETHOSTBYADDR_R_8
      HAVE_GETHOSTBYNAME_R_3
      HAVE_GETHOSTBYNAME_R_5
      HAVE_GETHOSTBYNAME_R_6)
    set(${CURL_TEST} 0)
    if(${CURL_TEST}_REENTRANT)
      set(${CURL_TEST} 1)
    endif(${CURL_TEST}_REENTRANT)
  endforeach(CURL_TEST)
endif(NEED_REENTRANT)

if(HAVE_INET_NTOA_R_DECL_REENTRANT)
  set(HAVE_INET_NTOA_R_DECL 1)
  set(NEED_REENTRANT 1)
endif(HAVE_INET_NTOA_R_DECL_REENTRANT)

# Some other minor tests

if(NOT HAVE_SOCKLEN_T)
  set(socklen_t "int")
endif(NOT HAVE_SOCKLEN_T)

if(NOT HAVE_IN_ADDR_T)
  set(in_addr_t "unsigned long")
endif(NOT HAVE_IN_ADDR_T)

# Fix libz / zlib.h

if(NOT CURL_SPECIAL_LIBZ)
  if(NOT HAVE_LIBZ)
    set(HAVE_ZLIB_H 0)
  endif(NOT HAVE_LIBZ)

  if(NOT HAVE_ZLIB_H)
    set(HAVE_LIBZ 0)
  endif(NOT HAVE_ZLIB_H)
endif(NOT CURL_SPECIAL_LIBZ)

if(_FILE_OFFSET_BITS)
  set(_FILE_OFFSET_BITS 64)
endif(_FILE_OFFSET_BITS)
set(CMAKE_REQUIRED_FLAGS "-D_FILE_OFFSET_BITS=64")
set(CMAKE_EXTRA_INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/curl/curl.h")
CHECK_TYPE_SIZE("curl_off_t" SIZEOF_CURL_OFF_T)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(CMAKE_REQUIRED_FLAGS)


# Check for nonblocking
set(HAVE_DISABLED_NONBLOCKING 1)
if(HAVE_FIONBIO OR
    HAVE_IOCTLSOCKET OR
    HAVE_IOCTLSOCKET_CASE OR
    HAVE_O_NONBLOCK)
  set(HAVE_DISABLED_NONBLOCKING)
endif(HAVE_FIONBIO OR
  HAVE_IOCTLSOCKET OR
  HAVE_IOCTLSOCKET_CASE OR
  HAVE_O_NONBLOCK)

if(RETSIGTYPE_TEST)
  set(RETSIGTYPE void)
else(RETSIGTYPE_TEST)
  set(RETSIGTYPE int)
endif(RETSIGTYPE_TEST)

if(CMAKE_COMPILER_IS_GNUCC AND APPLE)
  # -no-cpp-precomp and -Wno-long-double were compiler flags present
  # only in Apple's gcc and not in the FSF gcc. The flags are obsolete
  # and totally removed in gcc 4.2 and later. I believe they are only
  # needed with gcc 3.3 and earlier.

  # The Mac version of GCC warns about use of long double.  Disable it.
  curlCHECK_C_ACCEPTS_FLAGS("-Wno-long-double" C_HAS_LONGDOUBLE_FLAG)
  if(C_HAS_LONGDOUBLE_FLAG)
  get_source_file_property(MPRINTF_COMPILE_FLAGS mprintf.c COMPILE_FLAGS)
    if(MPRINTF_COMPILE_FLAGS)
      set(MPRINTF_COMPILE_FLAGS "${MPRINTF_COMPILE_FLAGS} -Wno-long-double")
    else(MPRINTF_COMPILE_FLAGS)
      set(MPRINTF_COMPILE_FLAGS "-Wno-long-double")
    endif(MPRINTF_COMPILE_FLAGS)
  set_source_files_properties(mprintf.c PROPERTIES COMPILE_FLAGS ${MPRINTF_COMPILE_FLAGS})
  endif(C_HAS_LONGDOUBLE_FLAG)
endif(CMAKE_COMPILER_IS_GNUCC AND APPLE)

# The rest of the build

include_directories(${LIBCURL_SOURCE_DIR})
include_directories(${LIBCURL_BINARY_DIR})
add_definitions(-DLIBCURL_HAVE_CONFIG_H)
configure_file(${LIBCURL_SOURCE_DIR}/config.h.in
  ${LIBCURL_BINARY_DIR}/LIBCURLConfig.h)

add_library(bmcurl ${libCurl_SRCS})
target_link_libraries(bmcurl ${CURL_LIBS} ${ITK_LIBRARIES})

option(CURL_TESTING "Do libCurl testing" OFF)
if(CURL_TESTING)
  subdirs(Testing)
endif(CURL_TESTING)

add_executable(LIBCURL Testing/curltest.c)
target_link_libraries(LIBCURL bmcurl ${CMAKE_DL_LIBS})
add_test(curl "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/LIBCURL")

#-----------------------------------------------------------------------------
# CMakeLists.txt custom to using LIBCURL as an external project

#include(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
#CMAKE_EXPORT_BUILD_SETTINGS(${LIBCURL_BINARY_DIR}/LIBCURLBuildSettings.cmake)

#export_library_dependencies(
   #${LIBCURL_BINARY_DIR}/LIBCURLLibraryDepends.cmake)

#set(LIBCURL_BUILD_SETTINGS_FILE_CONFIG
   #${LIBCURL_BINARY_DIR}/LIBCURLBuildSettings.cmake)

#set(LIBCURL_LIBRARY_DEPENDS_FILE
   #${LIBCURL_BINARY_DIR}/LIBCURLLibraryDepends.cmake)

set(LIBCURL_CONFIG_INSTALL_ONLY)

set(LIBCURL_USE_FILE_CONFIG ${LIBCURL_SOURCE_DIR}/CMake/UseLIBCURL.cmake)

# The library directories.
set(LIBCURL_LIBRARY_DIRS_CONFIG ${LIBCURL_LIBRARY_DIR})

# The runtime directories.
set(LIBCURL_RUNTIME_DIRS_CONFIG ${LIBCURL_RUNTIME_DIR})

# The include directories.
set(LIBCURL_INCLUDE_DIRS_CONFIG ${LIBCURL_INCLUDE_PATH})

set(LIBCURL_LIBRARIES_CONFIG ${LIBCURL_LIBRARIES} ${CURL_LIBS})

# The build configuration information.
set(LIBCURL_CONFIGURATION_TYPES_CONFIG ${CMAKE_CONFIGURATION_TYPES})

set(LIBCURL_BUILD_TYPE_CONFIG ${CMAKE_BUILD_TYPE})

# Configure LIBCURLConfig.cmake for the build tree.
configure_file(
  ${LIBCURL_SOURCE_DIR}/CMake/LIBCURLConfig.cmake.in
  ${LIBCURL_BINARY_DIR}/LIBCURLConfig.cmake @ONLY IMMEDIATE
  )

