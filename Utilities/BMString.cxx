/*=========================================================================

  Program:   BatchMake
  Module:    BMString.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "BMString.h"
#include <cstring> // strlen
#include <cstdlib> // really needed ?
#include <cctype>

#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>

BMString::BMString()
{
}

BMString::BMString(const BMString& m_string)
{
  m_value = m_string.m_value;
}

BMString::BMString(const MString& m_string)
{
  m_value = m_string.GetConstValue();
}

BMString::BMString(std::string m_string)
{
  m_value = m_string;
}

BMString::BMString(int value)
{
  std::stringstream ss;
  ss << value;
  ss >> m_value;
}

BMString::BMString(float value)
{
  std::stringstream ss;
  ss << value;
  ss >> m_value;
}

BMString::BMString(double value)
{
  std::stringstream ss;
  ss << value;
  ss >> m_value;
}

BMString::~BMString()
{
}

std::string& BMString::GetValue()
{
  return m_value;
}


const std::string& BMString::GetConstValue() const
{
  return m_value;
}

BMString& BMString::arg(int value)
{
  std::string arg;
  std::stringstream ss;
  ss << value;
  ss >> arg;
  size_t pos = m_value.find('%');
  if (pos != std::string::npos)
    {
    m_value.replace(pos,2,arg);
    }
  return *this;
}

BMString& BMString::arg(unsigned int value)
{
  std::string arg;
  std::stringstream ss;
  ss << value;
  ss >> arg;
  size_t pos = m_value.find('%');
  if (pos != std::string::npos)
    {
    m_value.replace(pos,2,arg);
    }
  return *this;
}

BMString& BMString::arg(unsigned long int value)
{
  std::string arg;
  std::stringstream ss;
  ss << value;
  ss >> arg;
  size_t pos = m_value.find('%');
  if (pos != std::string::npos)
    {
    m_value.replace(pos,2,arg);
    }
  return *this;
}

BMString& BMString::arg(float value)
{
  std::string arg;
  std::stringstream ss;
  ss << value;
  ss >> arg;
  size_t pos = m_value.find('%');
  if (pos != std::string::npos)
    {
    m_value.replace(pos,2,arg);
    }
  return *this;
}

const char* BMString::latin1()const
{
  return m_value.c_str();
}

const char* BMString::toChar()const
{
  if (m_value.length() == 0)
    return "";

  return m_value.c_str();
}

MString BMString::toMString()const
{
  return MString(m_value);
}

BMString::operator std::string()const
{
  return m_value;
}

int BMString::toInt()const
{
  return atoi(m_value.c_str());
}

float BMString::toFloat()const
{
  return static_cast<float>(atof(m_value.c_str()));
}

double BMString::toDouble()const
{
  return atof(m_value.c_str());
}

bool BMString::toBool()const
{
  if (atoi(m_value.c_str()) == 0)
    return false;
  else
   return true;
}

int BMString::length()const
{ 
  return m_value.size();
}

bool BMString::isEmpty()const
{
  return m_value.size() == 0;
}

bool BMString::operator==(const BMString& value)const
{
  return this->operator==(value.m_value);
}

bool BMString::operator==(const std::string& value)const
{
  if (m_value.compare(value) == 0)
    return true;
  else
    return false;
}

bool BMString::operator==(const char* value)const
{
  if (m_value.compare(value) == 0)
    return true;
  else
    return false;
}

bool BMString::operator!=(const BMString& value)const
{
  return this->operator!=(value.m_value);
}

bool BMString::operator!=(const std::string& value)const
{
  if (m_value.compare(value) == 0)
    return false;
  else
    return true;
}

bool BMString::operator!=(const char* value)const
{
  if (m_value.compare(value) == 0)
    return false;
  else
    return true;
}

bool BMString::operator<(const BMString& value)const
{
  return this->operator<(value.m_value);
}

bool BMString::operator<(const std::string& value)const
{
  if (m_value.compare(value) < 0)
    return true;
  else
    return false;
}

BMString& BMString::operator=(const BMString& value)
{
  m_value = value.m_value;
  return *this;
}

BMString& BMString::operator=(const char* value)
{
  if(value && strlen(value)>0)
    {  
    m_value = value;
    }
  else
   {
   m_value = "";
   }
  return *this;
}

BMString& BMString::operator+=(const BMString& value)
{
  m_value += value.m_value;
  return *this;
}

BMString& BMString::operator+=(const char* value)
{
  m_value += value;
  return *this;
}

BMString& BMString::operator+=(char value)
{
  m_value += value;
  return *this;
}

BMString BMString::operator+(const BMString& value)const
{
  BMString res(*this);
  res += value;
  return res;
}

BMString BMString::operator+(const char* value)const
{
  BMString res(*this);
  res += value;
  return res;
}

BMString& BMString::operator+(int offset)
{
  m_value.erase(0,offset);
  return *this;
}

BMString& BMString::operator++()
{
  m_value.erase(0,1);
  return *this;
}

char& BMString::operator[](int offset)
{
  return m_value[offset];
}

const char& BMString::operator[](int offset)const
{
  return m_value[offset];
}

int BMString::find(const char* key,int offset)const
{
  return m_value.find(key,offset);
}

int BMString::rfind(const char* key,int offset)const
{
  if (offset == -1)
    return m_value.rfind(key);

  return m_value.rfind(key,offset);
}

BMString& BMString::mid(int begin,int nb)
{
  if (nb == -1)
    {
    nb = m_value.length() - begin;
    }
  m_value.erase(begin+nb);
  m_value.erase(0,begin);
  return *this;
}

BMString BMString::midCopy(int begin,int nb)const
{
  if (nb == -1)
    {
    nb = m_value.length() - begin;
    }
  return m_value.substr(begin,nb);
}

BMString& BMString::begin(const char* key,int offset)
{
  size_t m_offset =  m_value.find(key,offset);
  if (m_offset != std::string::npos)
    {
    m_value.erase(m_offset);
    }
  return *this;
}

BMString BMString::beginCopy(const char* key,int offset)const
{
  size_t m_offset = m_value.find(key,offset);
  BMString res(m_value.substr(0,m_offset));
  return res;
}

BMString& BMString::rbegin(const char* key,int offset)
{
  size_t m_offset = m_value.rfind(key,offset);
  if (m_offset != std::string::npos)
    {
    m_value.erase(m_offset);
    }
  return *this;
}

BMString BMString::rbeginCopy(const char* key,int offset)const
{
  size_t m_offset = m_value.rfind(key,offset);
  BMString res(m_value.substr(0,m_offset));
  return res;
}


BMString& BMString::after(const char* key,int offset)
{
  size_t m_offset = m_value.find(key,offset);
  if (m_offset == std::string::npos)
    {
    m_value.clear();
    }
  else
    {
    m_value.erase(0,m_offset+1);
    }
  return *this;
}

BMString BMString::afterCopy(const char* key,int offset)const
{
  size_t m_offset = m_value.find(key,offset);
  if (m_offset == std::string::npos)
    {
    return BMString("");
    }
  return BMString( m_value.substr(m_offset+1) );
}

BMString& BMString::rafter(const char* key,int offset)
{
  size_t m_offset = m_value.rfind(key,offset);
  if (m_offset == std::string::npos)
    {
    m_value.clear();
    }
  else
    {
    m_value.erase(0,m_offset+1);
    }
  return *this;
}

BMString BMString::rafterCopy(const char* key,int offset)const
{
  size_t m_offset = m_value.rfind(key,offset);
  if (m_offset == std::string::npos)
    {
    return "";
    }
  return BMString( m_value.substr(m_offset+1) );
}

bool BMString::startWith(char key)const
{
  std::string::const_iterator it = m_value.begin();
  std::string::const_iterator end = m_value.end();
  for ( ; it != end; ++it)
    {
    if ( *it != ' ' && *it != '\t' )
      {
      return *it == key;
      }
    }
  return false;
}

bool BMString::endWith(char key)const
{
  return m_value[ m_value.length() - 1 ] == key;
}

BMString& BMString::removeFirstChar(char key)
{
  size_t pos = m_value.find(key);
  if ( pos != std::string::npos )
    {
    m_value.erase(pos,1);
    }
  return *this;
}

BMString& BMString::removeAllChars(char key)
{
  size_t pos = m_value.find(key);
  while ( pos != std::string::npos )
    {
    m_value.erase(pos,1);
    pos = m_value.find(key);
    }
  return *this;
}

BMString BMString::removeAllCharsCopy(char key)const
{
  BMString res;
  std::string::const_iterator it = m_value.begin();
  std::string::const_iterator end = m_value.end();
  for( ; it != end; ++it)
    {
    if ( *it != key )
      {
      res.m_value += *it;
      }
    }
  return res;
}

BMString& BMString::duplicateChar(char key)
{
  std::string::iterator it = std::find( m_value.begin(), m_value.end(), key);
  while (it != m_value.end())
    {
    it = m_value.insert( it, key );
    it = std::find( it, m_value.end(), key );
    }
  return *this;
}

BMString BMString::duplicateCharCopy(char key)const
{
  BMString res;
  std::string::const_iterator it = m_value.begin();
  std::string::const_iterator end = m_value.end();
  for( ; it != end; ++it)
    {
    res.m_value += *it;
    if( *it == key )
      {
      res.m_value += *it;
      }
    }
  return res;
}

BMString& BMString::replaceAllChars(char key,char key2)
{
  std::replace(m_value.begin(), m_value.end(), key, key2);
  return *this;
}

BMString BMString::replaceAllCharsCopy(char key,char key2)const
{
  // create a string with same size of m_value
  std::string m_newvalue;//(m_value.size(), char());
  replace_copy( m_value.begin(), m_value.end(), 
                std::back_inserter(m_newvalue), key, key2);
  return BMString(m_newvalue);
}

BMString& BMString::replace(const char* key,const char* key2)
{
  size_t length = strlen(key);
  size_t pos = m_value.find(key);
  while(pos != std::string::npos)
    {
    m_value.replace(pos,length,key2);
    pos = m_value.find(key);
    }
  return *this;
}

BMString BMString::replaceCopy(const char* key,const char* key2)const
{
  BMString res;
  size_t old_pos = 0;
  size_t pos = m_value.find(key,old_pos);
  size_t length = strlen(key);
  while(pos != std::string::npos)
    {
    res.m_value += m_value.substr( old_pos, pos - old_pos );
    res.m_value += key2;
    old_pos = pos + length;
    pos = m_value.find(key, old_pos);
    }
  res.m_value += m_value.substr( old_pos );
  return res;
}

BMString& BMString::toLower()
{
  std::transform( m_value.begin(), m_value.end(), 
                  m_value.begin(), static_cast<int(*)(int)>(std::tolower) );
  return *this;
}

BMString& BMString::toUpper()
{
  std::transform( m_value.begin(), m_value.end(), 
                  m_value.begin(), static_cast<int(*)(int)>(std::toupper) );
  return *this;
}

BMString BMString::toLowerCopy()const
{
  std::string res;
  std::transform( m_value.begin(), m_value.end(), 
                  std::back_inserter(res), 
                  static_cast<int(*)(int)>(std::tolower) );
  return BMString(res);
}

BMString BMString::toUpperCopy()const
{
  std::string res;
  std::transform( m_value.begin(), m_value.end(), 
                  std::back_inserter(res), 
                  static_cast<int(*)(int)>(std::toupper) );
  return BMString(res);
}

int BMString::count(char key)const
{
  return std::count(m_value.begin(), m_value.end(), key);
}

bool BMString::isFloat()const
{
  std::istringstream i(m_value);
  float x;
  char c;
  if ( !(i >> x) || (i.get(c)) )
    {
    return false;
    }
  return true;
}

bool BMString::isInt()const
{
  std::istringstream i(m_value);
  int x;
  char c;
  if ( !(i >> x) || (i.get(c)) )
    {
    return false;
    }
  return true;
}

/** Return if the current pos is between chars
 *  And we assume pairing */
bool BMString::isInBetweenChar(char val,long int pos)const
{
  long int pos1 = m_value.find(val);
  while(pos1 != -1)
    {
    long int pos2 = m_value.find(val,pos1+1);
    if(pos2 == -1)
      {
      return false;
      }

    if(pos>=pos1 && pos<=pos2)
      {
      return true;
      }
    pos1 = m_value.find(val,pos2+1);
    }
  return false;
}

bool BMString::isVariable()const
{
  // a variable must start with ''' preceded by 0 or many ' ' (spaces)
  // a variable must end with ''' followed by 0 or many ' ' (spaces)
  // only two ''' can be in the string
  std::string::const_iterator it = m_value.begin();
  std::string::const_iterator end = m_value.end();
  // 0 or many preceding ' '
  for( ; it != end && *it == ' '; ++it)
    {
    ;
    }
  if( it == end || *it != '\'' )
    {//no opening ''' char
    return false;
    }
  ++it;
  it = std::find(it,end,'\'');
  if( it == end )
    {// no closing ''' char
    return false;
    }
  ++it;
  for( ; it != end && *it == ' '; ++it)
    {
    ;
    }
  if( it != end )
    {
    return false;
    }
  return true;
}

bool BMString::isExpandableVariable() const
{
  std::size_t dollarOpenBrace = m_value.find( "${" );
  if( dollarOpenBrace != 0 )
    {
    return false;
    }
  std::size_t closeBrace = m_value.find( '}' );
  if( closeBrace != m_value.length() - 1 )
    {
    return false;
    }
  return true;
}

// Extract the string that is between two ' characters
// if value == "'foo'", returns "foo", if value == "foo", returns "foo"
BMString  BMString::fromVariable()const
{
  if( !this->isVariable() )
    {
    return *this;
    }
  size_t start = m_value.find('\'');
  size_t end = m_value.rfind('\'');
  if ( start == std::string::npos || end == std::string::npos || start == end )
    {
    return *this;
    }
  return this->midCopy(start + 1, end - 1 - start);
}

// Create a string with its content surrounded by two ' characters
// if value == "foo", returns "'foo'"
BMString  BMString::toVariable()const
{
  if ( !this->isVariable() )
    {
    return std::string( "'" ) + m_value + std::string( "'" );
    }
  return *this;
}

BMString BMString::extractExpandableVariable() const
{
  std::string res;
  std::size_t dollarOpenBrace = m_value.find( "${" );
  if( dollarOpenBrace != 0 )
    {
    return res;
    }
  std::size_t closeBrace = m_value.find( '}' );
  if( closeBrace != m_value.length() - 1 )
    {
    return res;
    }
  return m_value.substr( dollarOpenBrace + 2, closeBrace - 2 );
}

bool NotQuoteNotSpace( const char& character )
{
  return character != '\'' && character != ' ';
}

bool QuoteOrSpace( const char& character )
{
  return character == '\'' || character == ' ';
}

bool NotSpace( const char& character )
{
  return character != ' ';
}

std::vector<BMString> BMString::extractVariables( bool keepQuotes )const
{
  std::vector<BMString> variableList;
  bool inQuote = false;
  bool inVariable = false;
  std::string::const_iterator variableStart = m_value.begin();
  std::string::const_iterator afterQuote = m_value.begin();
  std::string::const_iterator variableEnd = m_value.begin();
  std::string::const_iterator end = m_value.end();
  std::string variable;
  while( variableEnd != end )
    {
    if( inVariable )
      {
      if( inQuote)
        {
        afterQuote = variableStart;
        ++afterQuote;
        variableEnd = std::find( afterQuote, end, '\'' );
        if( keepQuotes )
          {
          ++variableEnd;
          }
        else
          {
          ++variableStart;
          }
        }
      else
        {
        variableEnd = std::find_if( variableStart, end, QuoteOrSpace );
        }

      std::copy( variableStart, variableEnd, std::back_inserter(variable) );
      if( !keepQuotes )
        {
        ++variableEnd;
        }
      // we only create the variable if the next character is a space,
      // otherwise we just concatenate
      if( variableEnd == m_value.end() || *variableEnd == ' ' )
        {
        variableList.push_back( variable );
        variable.clear();
        //std::cout << "extracted variable: \"" << variable << "\"" << std::endl;
        }
      inVariable = false;
      inQuote = false;
      }
    //find the next variable first character
    while( !inVariable )
      {
      variableEnd = std::find_if( variableEnd, end, NotSpace );
      if( variableEnd == end )
        {
        break;
        }
      switch( *variableEnd )
        {
        case '\'':
          variableStart = variableEnd;
          inVariable = true;
          inQuote = true;
          break;
        default:
          variableStart = variableEnd++;
          inVariable = true;
          break;
        case ' ':
        // if we found a space we have to make sure the next character is not 
        // a space nor a quote. If spaces are represented by '_', then there
        // is no variable starting after the first '_' in the cases:
        // __wfjliwe...
        // _'wfjliwe...
        // but only in the case:
        // _wfjliwe...
        variableStart = variableEnd++;
        if( variableEnd != end && !QuoteOrSpace(*(variableEnd) ) )
          {
          inVariable = true;
          inQuote = false;
          }
        }
      }
    }
    
  if( variableList.empty() )
    {
    variableList.push_back(m_value);
    }
  return variableList;
}

std::vector<BMString> BMString::tokenize(const std::string& delimiters)const
{
  std::vector<BMString> tokens;
  // Skip delimiters at beginning.
  std::string::size_type lastPos = m_value.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos     = m_value.find_first_of(delimiters, lastPos);

  while( std::string::npos != pos || std::string::npos != lastPos )
    {
    // Found a token, add it to the vector.
    tokens.push_back( m_value.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = m_value.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = m_value.find_first_of(delimiters, lastPos);
    }
  return tokens;
}


BMString BMString::ConvertWildcardToRegEx()const
{
  BMString res;
  res.m_value += '^';
  std::string::const_iterator it = m_value.begin();
  std::string::const_iterator end = m_value.end();
  for ( ; it != end ; ++it) 
    {
    char c = *it;
    switch(c)
      {
      case '*':
        res.m_value += ".*";
        break;
      case '?':
        res.m_value += ".";
        break;
      // escape special regexp-characters
      case '(': case ')': case '[': case ']': case '$':
      case '^': case '.': case '{': case '}': case '|':
      case '\\':
        res.m_value += "\\";
        res.m_value += c;
        break;
      default:
        res.m_value += c;
      break;
      }
    }
  res.m_value += '$';
  return res;
}
