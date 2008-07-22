/*=========================================================================

  Program:   bmTester
  Module:    bmCondorTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include <cstring>
#include <iostream>

#ifdef WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif


#include "metaCommand.h"

int main(int argc, char **argv)
{
  MetaCommand command;

  command.AddField("firstFile","firstFile",MetaCommand::STRING,true);
  command.AddField("secondFile","secondFile",MetaCommand::STRING,true);
  command.AddField("operator","operator",MetaCommand::STRING,false);
  command.SetOption("writeOutput","W",false,"Write the paper out",MetaCommand::STRING);

  if(!command.Parse(argc,argv))
    {
    return 0;
    }


  // Do the sum of the two files
  FILE* fic1 = fopen(command.GetValueAsString("firstFile").c_str(),"rb");
  if(!fic1)
    {
    std::cout << "Cannot open file " << command.GetValueAsString("firstFile").c_str() << std::endl;
    return 0;
    }
  int n1;
  fscanf(fic1,"%d",&n1);
  fclose(fic1);

  FILE* fic2 = fopen(command.GetValueAsString("secondFile").c_str(),"rb");
  if(!fic2)
    {
    std::cout << "Cannot open file " << command.GetValueAsString("secondFile").c_str() << std::endl;
    return 0;
    }
  int n2;
  fscanf(fic2,"%d",&n2);
  fclose(fic2);

  if(command.GetValueAsString("writeOutput").size() > 0)
    {
    FILE* outFic = fopen(command.GetValueAsString("writeOutput").c_str(),"wb");
    if(!outFic)
      {
      std::cout << "Cannot open file " << command.GetValueAsString("writeOutput").c_str() << std::endl;
      return 0;
      }
    if(command.GetValueAsString("operator") == "*")
      {
      fprintf(outFic,"%d",n1*n2);
      }
    else if(command.GetValueAsString("operator") == "+")
      {
      fprintf(outFic,"%d",n1+n2);
      }
    
    fclose(outFic);
    }

  int sum = n1+n2;
  std::cout << "SUM = " << sum << std::endl;

#ifdef WIN32
   WSADATA wsaData;
   WSAStartup(MAKEWORD(2,0), &wsaData);
#endif
    
  char localhost[256];
  memset(localhost,0,sizeof(localhost));
  gethostname(localhost, sizeof(localhost));

  std::cout << "This process has been generated by : " << localhost << std::endl;

  return 0;
}

