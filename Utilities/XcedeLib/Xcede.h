#ifndef __Xcede_h_
#define __Xcede_h_

#include "Subject.h"
#include "Project.h"
#include "Resource.h"

#include <fstream>

#include "itksys/Glob.hxx"
#include <libxml/xmlwriter.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>
#include "libxml/xmlschemas.h"


class Xcede
{

public:
  Xcede(){;}
  ~Xcede(){;}

  xmlDocPtr ParseDoc(std::string docname, std::string schema, bool validation);
  xmlNodePtr CreateNewResourceNode(Resource* res, xmlDocPtr doc);
  bool SchemaValidation(xmlDocPtr doc, const char *xml_schema, bool printErrors);
  xmlXPathObjectPtr GetNodeSet(xmlDocPtr doc, xmlChar *xpath,
                                std::string nameSpace, std::string nameSpaceUrl);


  Project LoadXcede(xmlDocPtr doc, std::string pathImages);
  void LoadResources(xmlDocPtr doc, std::string pathImages);

  std::vector<std::string> Xpath(xmlDocPtr doc, std::string xpath, std::string attribute,
                                 std::string nameSpace, std::string nameSpaceUrl);
  std::string GetResourceID(xmlDocPtr doc, std::string subjectID,
                            std::string nameSpace, std::string nameSpaceUrl);
  void XpathSubjectInfo(xmlDocPtr doc, int patient, Subject* sub,
                        std::string nameSpace, std::string nameSpaceUrl);
  void XpathResourceInfo(xmlDocPtr doc, int patient, int resource, Resource* res,
                         std::string nameSpace, std::string nameSpaceUrl);


  void CreateNewXcedeFile(std::string filename, std::string schemaLocation);
  void WriteNewFile(std::string filename, xmlDocPtr doc, Project project,
                    std::string listOfArguments, std::string directoryResources,
                    std::string schemaLocation);
  void WriteNewResourceFile(std::string filename, xmlDocPtr doc, Resource* res);
  
  void SetPatient(std::string id, std::string sex, std::string birthdate,
                  std::string age,std::string handed, std::string race);
  void SetResource(std::string id, std::string format, std::string group, std::string subject,
                   std::string project, std::string uri, ProcessStep proc);

  void AddPatient (xmlDocPtr doc, Subject* sub,  bool subjectInfo);
  void AddProcessStep (xmlDocPtr doc, xmlNodePtr cur, Resource* res);


private:

  Project m_Project;
 };

#endif
