/*=========================================================================

  Program:   bmTester
  Module:    bmTester.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include <iostream>
#include <math.h>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Please set one, and only one, argument !" << std::endl;
    return -1;
  }
  else
  {
    int m_option = atoi(argv[1]);
    float m_value = -1;
    float i =0;
    switch(m_option)
    {
      case 0 : break;

      case 1 : std::cout << "Test cout !" << std::endl;
               break;

      case 2 : std::cerr << "Test cerr !" << std::endl;
               break;

      case 3 : std::cout << "Test cout !" << std::endl;
               std::cerr << "Test cerr !" << std::endl;
               break;

      case 4 : m_value = 0;
           std::cout << "Start function 4" << std::endl;
               for (i=1;i<9999999;i++)
               {
                  m_value += sqrt(i)/(i*i);
               }
               std::cout << "Value: " << m_value << std::endl;
               break;

      case 5 : m_value = 0;
           std::cout << "Start function 5" << std::endl;
               for (i=1;i<99999999;i++)
               {
                  m_value += sqrt(i)*sqrt(i)/(i*i);
               }
               std::cerr << "Value (error): " << m_value << std::endl;
               break;

      case 6 : m_value = 0;
                std::cout << "Start function 6" << std::endl;
               for (i=1;i<9999999;i++)
               {
                  m_value += sqrt(i)*sqrt(i)/(i*i);
               }
               std::cout << "Value: " << m_value << std::endl;
               std::cerr << "Value (error): " << m_value << std::endl;
               break;

      case 7 : m_value = 0;
               for (i=1;i<9999999;i++)
               {
                  m_value += sqrt(i)*sqrt(i)/(i*i);
               }
               break;

      default: break;
    
    }
  }
 

  return 0;
}
