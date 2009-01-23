/*=========================================================================

  Program:   BatchMake
  Module:    BMString.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __BMString_h_
#define __BMString_h_

//#include <ostream>
//#include <iostream>
//#include <stdio.h>
#include <string>
#include "MString.h"

class BMString
{
public:
  BMString();
  BMString(const MString& string);
  BMString(const BMString& string);
  BMString(int value);
  BMString(float value);
  BMString(double value);
  BMString(std::string string);
  BMString(const char* s) {m_value = s;}
  ~BMString();

  /** returns a copy of the internal string */
  std::string GetValue()const;
  //const std::string& GetConstValue() const;
  
  BMString& arg(int value);
  BMString& arg(float value);
  BMString& arg(unsigned int value);
  BMString& arg(unsigned long value);
  
  /** the returned value is valid only until next call to latin1. Don't 
   *  modify its content.
  */
  const char* latin1()const;
  /** Protected version of BMString::latin1(), if the length is null, returns
   *  a valid char* ("").
  */
  const char* toChar()const;

  int toInt()const;
  float toFloat()const;
  double toDouble()const;
  bool toBool()const;
  int length()const;
  bool isEmpty()const;
    
  bool operator==(const BMString&)const;
  bool operator==(const std::string&)const;
  bool operator==(const char*)const;
  
  bool operator!=(const BMString&)const;
  bool operator!=(const std::string&)const;
  bool operator!=(const char*)const;
  
  bool operator<(const BMString& )const;
  bool operator<(const std::string&)const;
  
  BMString& operator=(const BMString &);
  BMString& operator=(const char*);
  BMString& operator+=(const BMString&);
  BMString& operator+=(const char*);
  BMString& operator+=(char);
  
  BMString operator+(const BMString&)const;
  BMString operator+(const char*)const;
  
  // shift the string the the left
  BMString& operator+(int offset);
  /** prefix operator that shift the string to the left.
   *  In other words it removes the first char. or we "go" to the next char
  */
  BMString& operator++();
  char& operator[](int);
  const char& operator[](int)const;
    
  int find(const char*,int offset=0)const;
  /** if offset == -1, searches from the end of the string
  */
  int rfind(const char*,int offset=-1)const;
  
  /** truncate the string */
  BMString& mid(int begin,int nb=-1);
  /** returns a substract of the string. */
  BMString midCopy(int begin,int nb=-1)const;
  
  /** keep only the chain from its begin to the first occurence of key.
   *  In other words, truncate from first occurence of key to end
  */
  BMString& begin(const char* key,int offset=0);
  /** return a copy of the chain from its begin to the first occurence of key.
  */
  BMString beginCopy(const char* key,int offset=0)const;
  /** keep only the chain from the first occurence of key to its end.
   *  In other words, truncate from begin to first occurence of key.
  */ 
  BMString& end(const char* key,int offset=0);
  /** return a copy of the chain from the first occurence of key to its end.
  */
  BMString endCopy(const char* key,int offset=0)const;
  
  BMString& rbegin(const char* key,int offset=-1);
  BMString rbeginCopy(const char* key,int offset=-1)const;
  BMString& rend(const char* key,int offset=-1);
  BMString rendCopy(const char* key,int offset=-1)const;
   
  bool isFloat()const;
  bool isInt()const;
  
  /** return true if the first non space, non tab character is equal to key
  */
  bool startWith(char key)const;
  bool endWith(char key)const;
  
  /** remove just the first occurence of a char */
  BMString& removeChar(char key);
  BMString& removeAllChars(char key);
  BMString  removeAllCharsCopy(char key)const;
  BMString& duplicateChar(char key);
  BMString  duplicateCharCopy(char key)const;
  
  BMString& replaceAllChars(char key,char key2);
  BMString replaceAllCharsCopy(char key,char key2)const;
  BMString& replace(const char* key,const char* key2);
  BMString replaceCopy(const char* key,const char* key2)const;
  BMString& toLower();
  BMString& toUpper();
  int count(const char key)const;

  bool isInBetweenChar(char val,long int pos)const;

  //static std::string ConvertWildcardToRegEx(const char* wildcard);
  /** returns the RegExp string */
  BMString ConvertWildcardToRegEx()const;

private:
  std::string m_value;
};

#endif

