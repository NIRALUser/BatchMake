/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRegExAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptRegExAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

#include <stdio.h>
#include "itksys/RegularExpression.hxx"

namespace bm {


ScriptRegExAction::ScriptRegExAction()
: ScriptAction()
{
}

ScriptRegExAction::~ScriptRegExAction()
{
}

bool ScriptRegExAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() < 4)
     {
     error->SetError(MString("Not enough parameter for RegEx function !"),linenumber);
     return false;
     }
   
  std::string mode = (m_Manager->Convert(m_Parameters[3])).toChar();
  if (mode == "'REPLACE'")
    {
    if (m_Parameters.size() < 5)
       {
       error->SetError(MString(
         "REPLACE needs the replace string parameter after REPLACE !"),linenumber);
       return false;
       }
    }

   m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  return true;
}

 
MString ScriptRegExAction::Help()
{
  return "RegEx(<newvar> <var> <RegularExpression> <MATCH or REPLACE> [replacevar])";
}


void ScriptRegExAction::Execute()
{
  m_Manager->SetVariable(m_Parameters[0],"");
  itksys::RegularExpression regEx;

  BMString expression = m_Manager->Convert(m_Parameters[2]).fromVariable();

  BMString word = m_Manager->Convert(m_Parameters[1]).fromVariable();

  std::string regex = expression.GetValue();

  if (!regEx.compile(expression.toChar()))
    {
    std::string e = "Failed to compile regular expression ";
    e += expression.toChar();
    this->m_ProgressManager->AddError(e);
    return;
    }

  std::string mode = (m_Manager->Convert(m_Parameters[3])).toChar();
  if (mode == "'MATCH'")
    {
    if(regEx.find(word.toChar()))
      {
      std::string value = "'";
      value += regEx.match(0);
      value += "'";
      m_Manager->SetVariable(m_Parameters[0],value.c_str());
      }
    }
  else if (mode == "'REPLACE'")
    {
    if(regEx.find(word.toChar()))
      {
      BMString replaceS = m_Manager->Convert(m_Parameters[4]);
      std::string replace = replaceS.removeAllChars('\'').GetValue();

      // Pull apart the replace expression to find the escaped [0-9] values.
      std::vector<RegexReplacement> replacement;
      std::string::size_type l = 0;
      while(l < replace.length())
        {
        std::string::size_type r = replace.find("\\", l);
        if(r == std::string::npos)
          {
          r = replace.length();
          replacement.push_back(replace.substr(l, r-l));
          }
        else
          {
          if(r-l > 0)
            {
            replacement.push_back(replace.substr(l, r-l));
            }
          if(r == (replace.length()-1))
            {
            this->m_ProgressManager->AddError(
              "replace-expression ends in a backslash");
            return;
            }
          if((replace[r+1] >= '0') && (replace[r+1] <= '9'))
            {
            replacement.push_back(replace[r+1]-'0');
            }
          else if(replace[r+1] == 'n')
            {
            replacement.push_back("\n");
            }
          else if(replace[r+1] == '\\')
            {
            replacement.push_back("\\");
            }
          else
            {
            std::string e = "mode REPLACE: Unknown escape \"";
              e += replace.substr(r, 2);
              e += "\"in replace-expression.";
            this->m_ProgressManager->AddError(e);
            return;
            }
          r += 2;
          }
        l = r;
        }
      
      std::string input = word.GetValue();
      
      // Scan through the input for all matches.
      std::string output;
      std::string::size_type base = 0;
      while(regEx.find(input.c_str()+base))
        {
        std::string::size_type l2 = regEx.start();
        std::string::size_type r = regEx.end();
        
        // Concatenate the part of the input that was not matched.
        output += input.substr(base, l2);
        
        // Make sure the match had some text.
        if(r-l2 == 0)
          {
          std::string e = "mode REPLACE: regex \"" + regex + "\" matched empty string";
          if (this->m_Manager)
            {
            this->m_Manager->GetError()->SetError(MString(e.c_str()));
            }
          this->m_ProgressManager->AddError(e.c_str());
          return ;
          }
        
        // Concatenate the replacement for the match.
        for(unsigned int i=0; i < replacement.size(); ++i)
          {
          if(replacement[i].number < 0)
            {
            // This is just a plain-text part of the replacement.
            output += replacement[i].value;
            }
          else
            {
            // Replace with part of the match.
            int n = replacement[i].number;
            std::string::size_type start = regEx.start(n);
            std::string::size_type end = regEx.end(n);
            std::string::size_type len = input.length()-base;
            if((start != std::string::npos) && (end != std::string::npos) &&
               (start <= len) && (end <= len))
              {
              output += input.substr(base+start, end-start);
              }
            else
              {
              std::string e = "mode REPLACE: replace expression \""+
                replace+"\" contains an out-of-range escape for regex \""+
                regex+"\".";
              if (this->m_Manager)
                {
                this->m_Manager->GetError()->SetError(MString(e.c_str()));
                }
              this->m_ProgressManager->AddError(e.c_str());
              return;
              }
            }
          }
        
        // Move past the match.
        base += r;
        }
      
      // Concatenate the text after the last match.
      output += input.substr(base, input.length()-base);

      std::string value = "'";
      value += output;
      value += "'";
      m_Manager->SetVariable(m_Parameters[0],value.c_str());
      }
    }
  
}

} // end namespace bm
