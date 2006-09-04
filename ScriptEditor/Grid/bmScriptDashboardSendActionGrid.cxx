/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDashboardSendActionGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDashboardSendAction.h"

namespace bm {

/** */
void ScriptDashboardSendAction::GenerateGrid()
{
  // We create the bmGridSend application and send it to condor
  ApplicationWrapper app;
  MString appName = "bmGridSend";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it = m_manager->GetApplicationWrapperList()->begin();
  while (it != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*it)->GetName().toChar(),appName.toChar()))
      {
      app = *(*it);
      appFound = true;
      break;
      }
    it++;
  }

  if(!appFound)
    {
    std::cout << "ScriptDashboardSendAction::GenerateCondor : Cannot find bmGridSend " 
              << appName.toChar() << std::endl;
    return;
    }

  typedef ScriptActionManager::Dashboard DashboardType;
  typedef ScriptActionManager::DashboardExperiment DashboardExperimentType;
  typedef ScriptActionManager::DashboardMethod DashboardMethodType;
  typedef ScriptActionManager::DashboardMethodParameter DashboardMethodParameterType;

  // Get the project name
  const DashboardType * dashboard = m_manager->GetDashboard();
  const DashboardExperimentType* exp = NULL;
  const DashboardMethodType* meth = NULL;
  std::vector<DashboardExperimentType>::const_iterator it2 = dashboard->experiments.begin();
  while(it2 != dashboard->experiments.end())
    {
    std::vector<ScriptActionManager::DashboardMethod>::const_iterator itM = (*it2).methods.begin();
    while(itM != (*it2).methods.end())
      {
      if(!strcmp((*itM).variable.c_str(),m_parameters[0].toChar()))
        {
        exp = &(*it2);
        meth = &(*itM);
        break;
        }
      itM++;
      }
    it2++;
    }

   if(!meth)
    {
    m_progressmanager->AddError("BMDashboard: Cannot find method");
    return;
    }

  if(!exp)
    {
    m_progressmanager->AddError("BMDashboard: Cannot find experiment");
    return;
    }

  std::string withslash = "\"";
  withslash += m_manager->GetDashboardUser();
  withslash += "\"";
  app.SetParameterValue("hostname","",m_manager->GetDashboardURL());
  app.SetParameterValue("user","",withslash);
  withslash = "\"";
  withslash += exp->project;
  withslash += "\"";
  app.SetParameterValue("project","",withslash);
  app.SetParameterValue("send","","1");
  withslash = "\"";
  withslash += exp->name;
  withslash += "\"";
  app.SetParameterValue("send.experiment","",withslash);
  withslash = "\"";
  withslash += meth->name;
  withslash += "\"";
  app.SetParameterValue("send.method","",withslash);
  app.SetParameterValue("data","","1");
 
  // Check that the experiment exist
  std::string data = "";
  unsigned int num = 0;
  std::string imagedata = "";
  unsigned int imagenum = 0;

  std::vector<DashboardExperimentType>::const_iterator itE 
                                              = dashboard->experiments.begin();
  while(itE != dashboard->experiments.end())
    {
    std::vector<DashboardMethodType>::const_iterator itMeth = (*itE).methods.begin();
    while(itMeth != (*itE).methods.end())
      {
      if((!strcmp((*itMeth).variable.c_str(),m_parameters[0].toChar())))
        {
        std::vector<DashboardMethodParameterType>::const_iterator itParam 
                                                    = (*itMeth).parameters.begin();
        bool found = false;
        while(itParam != (*itMeth).parameters.end())
          {
          if( !strcmp((*itParam).type.c_str(),"png")
            || !strcmp((*itParam).type.c_str(),"jpg"))
            {
            MString param = "${";
            param += (*itParam).variable.c_str();
            param += "}";
            imagedata += "\"";
            imagedata += (*itParam).name.c_str();
            imagedata += "\"";
            imagedata += " ";
            imagedata += "\"";
            MString value = m_manager->Convert(param).toChar();
            value = value.removeChar('\'');
            imagedata += value.toChar();
            imagedata += "\" ";
            imagenum++;
            }
          else
            {
            MString param = "${";
            param += (*itParam).variable.c_str();
            param += "}";
            data += "\"";
            data += (*itParam).name.c_str();
            data += "\"";
            data += " ";
            data += "\"";
            MString value = m_manager->Convert(param).toChar();
            value = value.removeChar('\'');
            data += value.toChar();
            data += "\" ";
            num++;
            }
          itParam++;
          }
        }
      itMeth++;
      }
    itE++;
    }

  char* numParams = new char[10];
  sprintf(numParams,"%d",num*2);
  std::string numParam = numParams;
  numParam += " ";
  numParam += data;
  delete [] numParams;
  app.SetParameterValue("data.NumberOfValues","",numParam);

  numParams = new char[10];
  sprintf(numParams,"%d",imagenum*2);
  numParam = numParams;
  numParam += " ";
  numParam += imagedata;
  delete [] numParams;
  app.SetParameterValue("imagedata.NumberOfValues","",numParam);

  if(imagenum>0)
    {
    app.SetParameterValue("imagedata","","1");
    }

  app.SetParameterValue("inputFile","","1");
  app.SetParameterValue("inputFile.inputFile","",m_GridModule->GetCurrentScopeFile());

  m_GridModule->AddApplication(&app,m_GridModule->GetOutputDirectory());
}

} // end namespace bm
