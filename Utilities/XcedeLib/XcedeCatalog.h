#ifndef __XcedeCatalog_h_
#define __XcedeCatalog_h_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>

#include "itksys/Glob.hxx"
#include "itksys/SystemTools.hxx"

#include <libxml/xmlwriter.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

class XcedeCatalog
{
public:

  XcedeCatalog();
  ~XcedeCatalog(){};

  void Login(std::string login, std::string password);

  xmlDocPtr ParseDoc(std::string docname);
  xmlXPathObjectPtr GetNodeSet (xmlDocPtr doc, xmlChar *xpath,
                                std::string nameSpace, std::string nameSpaceUrl);

  std::vector<std::string> GetUrls(xmlDocPtr doc, std::string catalogID="");
  std::vector<std::string> GetNames(xmlDocPtr doc, std::string catalogID="");

  std::vector<std::string> GetXcedeDataSets(xmlDocPtr doc, std::string catalogID="", 
                                            std::string login="", std::string password="");
  bool DownloadXcedeDatasets(std::string xcedeDataSet, std::string directory,
                             std::string login="", std::string password="");

  std::string GetXcedeFilename(std::string xcedeDataSet);
  std::string GetXcedeUrl(std::string xcedeDataSet);


private:

  std::string m_LoginUrl;
  std::string m_SessionId;

  std::string m_Catalog;
  std::string m_Directory;
  std::string m_Url;
  std::string m_Filename;
};

#endif