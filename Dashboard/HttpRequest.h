#ifndef HttpRequest_h
#define HttpRequest_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <netdb.h>
#endif

#define MEM_BUFFER_SIZE 10

typedef struct
{ 
  char* headerSend;            
  char* headerReceive;
  char* message;              
  long messageLength;
} MessageStruct;

typedef struct
{
    unsigned  char *buffer;
    unsigned  char *position;
    size_t    size;
} MemBuffer;

typedef struct
{
  std::string name;
  std::string value;
} Paramstruct;

class HttpRequest  
{
public:
  HttpRequest();
  virtual ~HttpRequest();

  void AddParam(std::string name,std::string value);
  void SetFile(std::string filename);
  char* Send(std::string url);
  std::string GetHostName();
  std::string GetHostIp();

private:
  void    MemBufferCreate(MemBuffer *b);
  void    MemBufferGrow(MemBuffer *b);
  void    MemBufferAddByte(MemBuffer *b, unsigned char byt);
  void    MemBufferAddBuffer(MemBuffer *b, unsigned char *buffer, size_t size);
  int    GetHostAddress(char* host);
  void    SendString(int sock,std::string str);
        bool          ValidHostChar(char ch);
  void    ParseURL(const char* url,char* protocol,int lprotocol, char* host,int lhost,char* request,int lrequest,int *port);
  int      SendHTTP(const char*  url,char* headers,unsigned char *post, unsigned int postLength,MessageStruct *req);
  std::string CreateFile(std::string filename);

  std::string m_filename;
  std::vector<Paramstruct> m_paramlist;
};

#endif
