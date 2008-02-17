#ifndef __XnatCatalog_h_
#define __XnatCatalog_h_

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


class XnatCatalog
{
public:

  XnatCatalog();
  ~XnatCatalog(){};

  void Login(std::string login, std::string password);

  xmlDocPtr ParseDoc(std::string docname);
  xmlDocPtr ParseDocMemory(std::string buffer);
  xmlXPathObjectPtr GetNodeSet (xmlDocPtr doc, xmlChar *xpath,
                                     std::string nameSpace, std::string nameSpaceUrl);
  xmlDocPtr GetNewCatalog(xmlDocPtr mainDoc, std::string login="",
                          std::string password="");


  std::vector<std::string> GetUrls(xmlDocPtr doc, std::string login="",
                                   std::string password="");
  std::vector<std::string> GetNames(xmlDocPtr doc, std::string login="",
                                    std::string password="");

  std::vector<std::string> GetXnatDataSets(xmlDocPtr mainDoc, std::string login="",
                                          std::string password="");
  std::vector<std::string> GetXnatDataSetNames(xmlDocPtr mainDoc, std::string login="",
                                               std::string password="");
  bool DownloadXnatDatasets(std::string xnatDataSet, std::string directory,
                            std::string login="", std::string password="");

  std::string GetXnatFilename(std::string xnatDataSet);
  std::string GetXnatUrl(std::string xnatDataSet);


private:

  std::string m_LoginUrl;
  std::string m_SessionId;

  std::string m_Catalog;
  std::string m_Directory;
  std::string m_Url;
  std::string m_Filename;
};

#endif
