#include "zlib.h"
#include <iostream>

#if defined(MSDOS) || defined(OS2) || defined(WIN32)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
# include <iostream>
#  define SET_BINARY_MODE(file)
#endif

#ifdef VMS
#  define unlink delete
#  define GZ_SUFFIX "-gz"
#endif
#ifdef RISCOS
#  define unlink remove
#  define GZ_SUFFIX "-gz"
#  define fileno(file) file->__file
#endif
#if defined(__MWERKS__) && __dest_os != __be_os && __dest_os != __win32_os
#  include <unix.h> /* for fileno */
#endif

#ifndef WIN32 /* unlink already in stdio.h for WIN32 */
  #include <unistd.h>
#endif

#ifndef GZ_SUFFIX
#  define GZ_SUFFIX ".gz"
#endif
#define SUFFIX_LEN (sizeof(GZ_SUFFIX)-1)

#define BUFLEN      16384
#define MAX_NAME_LEN 1024

#ifdef MAXSEG_64K
#  define local static
   /* Needed for systems with limitation on stack size. */
#else
#  define local
#endif


bool IsCompressed(const char* name)
{
  std::string m_string(name);
  if (m_string.rfind(".gz") == m_string.length()-3)
   return true;
  else
   return false;
}

void gz_compress(FILE* in,gzFile out)
{
    local char buf[BUFLEN];
    int len;
    int err;

    for (;;) {
        len = fread(buf, 1, sizeof(buf), in);
        if (ferror(in)) {
            perror("fread");
            exit(1);
        }
        if (len == 0) break;

        if (gzwrite(out, buf, (unsigned)len) != len) 
          std::cerr << gzerror(out, &err) << std::endl;
    }
    fclose(in);
    if (gzclose(out) != Z_OK)
       std::cerr << "failed gzclose" << std::endl;
}

int gz_compressfile(char* file,char* mode)
{
   if (IsCompressed(file))
   {
     std::cerr << "File already compressed !" << std::endl;
     return -1;
   }

    char* outfile = new char[MAX_NAME_LEN];
    FILE  *in;
    gzFile out;

    strcpy(outfile, file);
    strcat(outfile, GZ_SUFFIX);

    in = fopen(file, "rb");
    if (in == NULL) {
        perror(file);
        exit(1);
    }
    out = gzopen(outfile, mode);
    if (out == NULL) {
      std::cerr << "Error opening the compressed file ! " << std::endl;
      return -1;
    }
    gz_compress(in, out);

    unlink(file);

    return 0;
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Please specify a file to compress !" << std::endl;
    return -1;
  }

   char outmode[20];
   sprintf(outmode,"wb%i ",6);
   int m_result = 0; 
   for (int i=1;i<argc;i++)
     if (gz_compressfile(argv[i],outmode) != 0)
  m_result = -1;

  return m_result; 
}





