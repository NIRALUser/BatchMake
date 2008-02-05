#include "Xcede.h"

xmlDocPtr Xcede::ParseDoc(std::string docname, std::string schema, bool validation)
{
  xmlDocPtr doc;
  xmlNodePtr cur;
  doc = xmlParseFile(docname.c_str());

  if (doc == NULL )
    {
    std::cerr<<"Document not parsed successfully"<<std::endl;
    return (NULL);
    }

  cur = xmlDocGetRootElement(doc);

  if (cur == NULL)
    {
    std::cerr<<"empty document"<<std::endl;
    xmlFreeDoc(doc);
    return (NULL);
    }
  if (xmlStrcmp(cur->name, (const xmlChar *) "XCEDE"))
    {
    std::cerr<<"document of the wrong type, root node != XCEDE"<<std::endl;
    xmlFreeDoc(doc);
    return (NULL);
    }
  xmlCleanupParser();

  if(validation)
    {
    if(SchemaValidation(doc, schema.c_str(),true))
      {
      return(doc);
      }
    else
      {
      return (NULL);
      }
    }
  else
    {
    return(doc);
    }
}


xmlNodePtr Xcede::CreateNewResourceNode(Resource* res, xmlDocPtr doc)
{
  xmlNodePtr resourceNode;
  xmlNodePtr root = xmlDocGetRootElement(doc);

  // Create a new "resource" node
  if ((resourceNode = xmlNewNode(NULL, BAD_CAST "resource")) == NULL)
    {
    return NULL;
    }

  // add attributes
  if (xmlSetProp(resourceNode, BAD_CAST "ID", BAD_CAST res->GetResourceID().c_str()) == NULL ||
      xmlSetProp(resourceNode, BAD_CAST "format", BAD_CAST res->GetResourceInfo().format.c_str()) == NULL ||
      xmlSetProp(resourceNode, BAD_CAST "subjectGroupID", BAD_CAST res->GetResourceInfo().subjectGroupID.c_str()) == NULL ||
      xmlSetProp(resourceNode, BAD_CAST "subjectID", BAD_CAST res->GetResourceInfo().subjectID.c_str()) == NULL ||
      xmlSetProp(resourceNode, BAD_CAST "projectID", BAD_CAST res->GetResourceInfo().projectID.c_str()) == NULL ||
      xmlSetProp(resourceNode, BAD_CAST "xsi:type", BAD_CAST "moreresource_t") == NULL)
    {
    xmlFreeNode(resourceNode);
    return NULL;
    }

  // Create the new nodes
  for(int i=0 ; i<(int)res->GetNbTagsResource() ; i++)
    {
    if (xmlNewTextChild(resourceNode, NULL, BAD_CAST res->GetTagName(i).c_str(), BAD_CAST res->GetTagNameValue(i).c_str()) == NULL)
      {
      xmlFreeNode(resourceNode);
      return NULL;
      }
    xmlAddChild(root, resourceNode);
    }

  return resourceNode;
}


bool Xcede::SchemaValidation(xmlDocPtr doc, const char *xml_schema, bool printErrors)
{
  xmlSchemaPtr schema;
  xmlSchemaValidCtxtPtr vctxt;
  xmlSchemaParserCtxtPtr pctxt;

  // Opening and loading the XML Schema
  if ((pctxt = xmlSchemaNewParserCtxt(xml_schema)) == NULL)
    {
    return -1;
    }
  schema = xmlSchemaParse(pctxt);
  xmlSchemaFreeParserCtxt(pctxt);
  if (schema == NULL)
    {
    return -1;
    }
  if ((vctxt = xmlSchemaNewValidCtxt(schema)) == NULL)
    {
    xmlSchemaFree(schema);
    return -1;
    }

  // Processing errors
  if (printErrors)
    {
    xmlSchemaSetValidErrors(vctxt, (xmlSchemaValidityErrorFunc)fprintf,
                           (xmlSchemaValidityWarningFunc) fprintf, stderr);
    }

  // Validation
  if(xmlSchemaValidateDoc(vctxt, doc) == 0)
    {
    xmlSchemaFree(schema);
    xmlSchemaFreeValidCtxt(vctxt);
    return true;
    }
  else
    {
    xmlSchemaFree(schema);
    xmlSchemaFreeValidCtxt(vctxt);
    return false;
    }
}


Project Xcede::LoadXcede(xmlDocPtr doc, std::string pathImages)
{
  std::string nameSpace = "xcede";
  std::string nameSpaceUrl = "http://www.xcede.org/xcede-2";

  std::vector<std::string> result, patients;

  //Get the main information from the file
  result = this->Xpath(doc, "project", "ID", nameSpace, nameSpaceUrl);
  m_Project.SetProjectName(result[0]);
  result.clear();
  result = this->Xpath(doc, "subjectGroup", "ID", nameSpace, nameSpaceUrl);
  m_Project.SetGroupName(result[0]);

  //Get the information of each subject
  patients = this->Xpath(doc, "subject", "ID", nameSpace, nameSpaceUrl);
  for(int i=0 ; i<(int)patients.size() ;i++)
    {
    m_Project.AddPatientToProject();
    m_Project.GetPatient(i)->SetSubjectID(patients[i]);
    this->XpathSubjectInfo(doc, i, m_Project.GetPatient(i), nameSpace, nameSpaceUrl);
    }

  this->LoadResources(doc, pathImages);

  xmlFreeDoc(doc);
  return m_Project;
}


void Xcede::LoadResources(xmlDocPtr doc, std::string pathImages)
{
  std::string nameSpace = "xcede";
  std::string nameSpaceUrl = "http://www.xcede.org/xcede-2";
  itksys::Glob glob;
  glob.RecurseOn();
  glob.FindFiles(pathImages.c_str());
  std::vector<std::string> xcedeResourcesFiles = glob.GetFiles();

  for(int j=0 ; j<m_Project.GetNbPatients() ; j++)
    {
    int cur_resource = 0;
    std::vector<std::string> resources;
    for(int i=0; i<(int)xcedeResourcesFiles.size() ; i++)
      {
      xmlDocPtr docResource;
      docResource = xmlParseFile(xcedeResourcesFiles[i].c_str());

      if(strcmp(this->GetResourceID(docResource, m_Project.GetPatient(j)->GetSubjectID(),
         nameSpace, nameSpaceUrl).c_str(), "") != 0)
        {
        resources.push_back(this->GetResourceID(docResource, m_Project.GetPatient(j)->GetSubjectID(),
                            nameSpace, nameSpaceUrl));
        m_Project.GetPatient(j)->AddResource();
        m_Project.GetPatient(j)->GetResource(cur_resource)->SetResourceID(resources.back());
        this->XpathResourceInfo(docResource, j, cur_resource,
                                m_Project.GetPatient(j)->GetResource(cur_resource), nameSpace, nameSpaceUrl);
        this->CreateNewResourceNode(m_Project.GetPatient(j)->GetResource(cur_resource), doc);
        cur_resource++;
        }
        xmlFreeDoc(docResource);
      }
    }
}


std::vector<std::string> Xcede::Xpath(xmlDocPtr doc, std::string xpath, std::string attribute,
                                      std::string nameSpace, std::string nameSpaceUrl)
{
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;

  std::string Request = "//";
  Request += nameSpace;
  Request += ":";
  Request += xpath;
  Request += "/@";
  Request += attribute;


  std::vector<std::string> listOfValues;

  xmlChar *xpathRequest =(xmlChar*) Request.c_str();
  result = this->GetNodeSet(doc, xpathRequest, nameSpace, nameSpaceUrl);
  if (result)
    {
    nodeset = result->nodesetval;
    for (int i=0; i < nodeset->nodeNr; i++)
      {
      xmlChar *value = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode,1);
      listOfValues.push_back((char*) value);
      xmlFree(value);
      }
    xmlXPathFreeObject (result);
    }

  return listOfValues;
}


std::string Xcede::GetResourceID(xmlDocPtr doc, std::string subjectID,
                                 std::string nameSpace, std::string nameSpaceUrl)
{
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;

  std::string Request = "//";
  Request += nameSpace;
  Request += ":resource[@subjectID=\"";
  Request += subjectID;
  Request += "\"]/@ID";

  std::string resourceId;
   
  xmlChar *xpathRequest =(xmlChar*) Request.c_str();
  result = this->GetNodeSet(doc, xpathRequest, nameSpace, nameSpaceUrl);
  if (result)
    {
    nodeset = result->nodesetval;
    xmlChar *value = xmlNodeListGetString(doc, nodeset->nodeTab[0]->xmlChildrenNode,1);
    resourceId = (char*) value;
    xmlFree(value);
    xmlXPathFreeObject (result);
    return resourceId;
    }
  else 
    {
    return "";
    }
}


void Xcede::XpathSubjectInfo(xmlDocPtr doc, int patient, Subject* sub,
                             std::string nameSpace, std::string nameSpaceUrl)
{  
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;

  std::string Request = "//";
  Request += nameSpace;
  Request += ":subject[@ID=\"";
  Request += sub->GetSubjectID();
  Request += "\"]/";
  Request += nameSpace;
  Request += ":subjectInfo/*";

  xmlChar *xpathRequest =(xmlChar*) Request.c_str();
  result = this->GetNodeSet(doc, xpathRequest, nameSpace, nameSpaceUrl);
  if (result)
    {
    nodeset = result->nodesetval;
    m_Project.GetPatient(patient)->SetNbTagsInfo(nodeset->nodeNr);
    for (int i=0; i < nodeset->nodeNr; i++)
      {
      xmlChar *value = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode,1);
      if(value != NULL)
        {
        m_Project.GetPatient(patient)->SetTagName((char*) nodeset->nodeTab[i]->name);
        m_Project.GetPatient(patient)->SetTagValue((char*) value,i);
        }
      else 
        {
        m_Project.GetPatient(patient)->SetTagName((char*) nodeset->nodeTab[i]->name);
        m_Project.GetPatient(patient)->SetTagValue("",i);
        }
      xmlFree(value);
      }
    xmlXPathFreeObject (result);
    }
}


void Xcede::XpathResourceInfo(xmlDocPtr doc, int patient, int resource, Resource* res,
                              std::string nameSpace, std::string nameSpaceUrl)
{
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;

  std::string Request = "//";
  Request += nameSpace;
  Request += ":resource[@ID=\"";
  Request += res->GetResourceID();
  Request += "\"]/*";

  xmlChar *xpathRequest =(xmlChar*) Request.c_str();
  result = this->GetNodeSet(doc, xpathRequest, nameSpace, nameSpaceUrl);
  if (result)
    {
    nodeset = result->nodesetval;
    for (int i=0; i < nodeset->nodeNr; i++)
      {
      xmlChar *value = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode,1);
      m_Project.GetPatient(patient)->GetResource(resource)->SetTagName((char*) nodeset->nodeTab[i]->name);
      m_Project.GetPatient(patient)->GetResource(resource)->SetTagValue((char*) value,i);
      xmlFree(value);
      }
    xmlXPathFreeObject (result);
    }


  std::string format, subjectGroupID, subjectID, projectID;
  std::string requestFormat, requestSubjectGroupID, requestSubjectID, requestProjectID;
  xmlXPathObjectPtr xpathFormat, xpathSubjectGroupID, xpathSubjectID, xpathProjectID;
  xmlNodeSetPtr nodesetFormat, nodesetSubjectGroupID, nodesetSubjectID, nodesetProjectID;
  requestFormat = "//";
  requestFormat += nameSpace;
  requestFormat += ":resource[@ID=\"";
  requestFormat += res->GetResourceID();
  requestSubjectGroupID = requestFormat;
  requestSubjectID = requestFormat;
  requestProjectID = requestFormat;
  requestFormat += "\"]/@format";
  requestSubjectGroupID += "\"]/@subjectGroupID";
  requestSubjectID += "\"]/@subjectID";
  requestProjectID += "\"]/@projectID";

  xmlChar *xpathRequestFormat =(xmlChar*) requestFormat.c_str();
  xmlChar *xpathRequestSubjectGroupID =(xmlChar*) requestSubjectGroupID.c_str();
  xmlChar *xpathRequestSubjectID =(xmlChar*) requestSubjectID.c_str();
  xmlChar *xpathRequestProjectID =(xmlChar*) requestProjectID.c_str();
  xpathFormat = this->GetNodeSet(doc, xpathRequestFormat, nameSpace, nameSpaceUrl);
  xpathSubjectGroupID = this->GetNodeSet(doc, xpathRequestSubjectGroupID, nameSpace, nameSpaceUrl);
  xpathSubjectID = this->GetNodeSet(doc, xpathRequestSubjectID, nameSpace, nameSpaceUrl);
  xpathProjectID = this->GetNodeSet(doc, xpathRequestProjectID, nameSpace, nameSpaceUrl);
  if (xpathFormat && xpathSubjectGroupID && xpathSubjectID && xpathProjectID)
    {
    nodesetFormat = xpathFormat->nodesetval;
    nodesetSubjectGroupID = xpathSubjectGroupID->nodesetval;
    nodesetSubjectID = xpathSubjectID->nodesetval;
    nodesetProjectID = xpathProjectID->nodesetval;
    xmlChar *formatValue = xmlNodeListGetString(doc, nodesetFormat->nodeTab[0]->xmlChildrenNode,1);
    xmlChar *subjectGroupIDValue = xmlNodeListGetString(doc, nodesetSubjectGroupID->nodeTab[0]->xmlChildrenNode,1);
    xmlChar *subjectIDValue = xmlNodeListGetString(doc, nodesetSubjectID->nodeTab[0]->xmlChildrenNode,1);
    xmlChar *projectIDValue = xmlNodeListGetString(doc, nodesetProjectID->nodeTab[0]->xmlChildrenNode,1);
    format = (char*) formatValue;
    subjectGroupID = (char*) subjectGroupIDValue;
    subjectID = (char*) subjectIDValue;
    projectID = (char*) projectIDValue;
    xmlFree(formatValue);
    xmlFree(subjectGroupIDValue);
    xmlFree(subjectIDValue);
    xmlFree(projectIDValue);
    xmlXPathFreeObject (xpathFormat);
    xmlXPathFreeObject (xpathSubjectGroupID);
    xmlXPathFreeObject (xpathSubjectID);
    xmlXPathFreeObject (xpathProjectID);
    }

  m_Project.GetPatient(patient)->GetResource(resource)->SetInfo(format, subjectGroupID, subjectID, projectID);
}


xmlXPathObjectPtr Xcede::GetNodeSet (xmlDocPtr doc, xmlChar *xpath,
                                     std::string nameSpace, std::string nameSpaceUrl)
{
  xmlXPathInit();
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  context = xmlXPathNewContext(doc);
  xmlXPathRegisterNs(context, BAD_CAST nameSpace.c_str(), BAD_CAST nameSpaceUrl.c_str());


  if (context == NULL)
    {
    std::cerr<<"Error in xmlXPathNewContext"<<std::endl;
    return NULL;
    }

  result = xmlXPathEvalExpression(xpath, context);
  xmlXPathFreeContext(context);

  if (result == NULL)
    {
    std::cerr<<"Error in xmlXPathEvalExpression"<<std::endl;
    return NULL;
    }
 
  if(xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
    xmlXPathFreeObject(result);
    return NULL;
    }
  return result;
}


/** Create a new xcede file and write the basic XCEDE stuff*/
void Xcede::CreateNewXcedeFile(std::string filename, std::string schemaLocation)
{
  std::ofstream file(filename.c_str(),std::ios::out | std::ios::trunc);
  file<<"<?xml version=\"1.0\"?>\n";
  file<<"<XCEDE xmlns=\"http://www.xcede.org/xcede-2\" ";
  file<<"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
  file<<"xmlns:fbirn=\"http://www.xcede.org/xcede-2/extensions/fbirn\" ";
  file<<"xmlns:xcede=\"http://www.xcede.org/xcede-2\" ";
  file<<"xsi:schemaLocation=\"http://www.xcede.org/xcede-2 "<<schemaLocation<<"\">\n";

  file<<"</XCEDE>";
  file.close();
}

/** Write the new xcede file with all the new information
 *  about each patient, the resources etc...*/
void Xcede::WriteNewFile(std::string filename, xmlDocPtr doc, Project project,
                         std::string listOfArguments, std::string directoryResources,
                         std::string schemaLocation)
{
  std::vector<std::string> tagValues, tagNames;
  std::string pathT2, resourceID;
  xmlNodePtr cur;
  cur = xmlDocGetRootElement(doc);

  while (cur != NULL)
    {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"XCEDE")))
      {
      for(int i=0 ; i<project.GetNbPatients() ; i++)
        {
        this->AddPatient(doc, project.GetPatient(i), true);
        }

      xmlNodePtr projectNode = xmlNewTextChild (cur, NULL, BAD_CAST "project",NULL);
      xmlNewProp (projectNode, BAD_CAST "ID", BAD_CAST project.GetProjectName().c_str());
      xmlNodePtr projectInfoNode = xmlNewTextChild (projectNode, NULL, BAD_CAST "projectInfo",NULL);
      xmlNodePtr GroupListNode = xmlNewTextChild (projectInfoNode, NULL, BAD_CAST "subjectGroupList", NULL);
      xmlNodePtr subjectGroupNode = xmlNewTextChild (GroupListNode, NULL, BAD_CAST "subjectGroup", NULL);
      xmlNewProp (subjectGroupNode, BAD_CAST "ID", BAD_CAST project.GetGroupName().c_str());

      for(int i=0 ; i<project.GetNbPatients() ; i++)
        {
        xmlNewTextChild (subjectGroupNode, NULL, BAD_CAST "subjectID",
                         BAD_CAST project.GetPatient(i)->GetSubjectID().c_str());
        }
      }
    cur = cur->next;
    }

  if (doc != NULL)
    {
    xmlSaveFormatFile (filename.c_str(), doc, 2);
    xmlFreeDoc(doc);
    }


  // Wrte the new resources xcede files
  for(int i=0 ; i<project.GetNbPatients() ; i++)
    {
    for(int j=0 ; j<project.GetPatient(i)->GetNbResource() ; j++)
      {
      for(int k=0 ; k<project.GetPatient(i)->GetResource(j)->GetNbTagsResource() ; k++)
        {
        if(strcasecmp(project.GetPatient(i)->GetResource(j)->GetTagNameValue(k).c_str(), "T2") == 0)
          {
          std::string xcedeResource = directoryResources;
          xcedeResource += "/";
          for(int l=0 ; l<project.GetPatient(i)->GetResource(j)->GetNbTagsResource() ; l++)
            {
            if(strcasecmp(project.GetPatient(i)->GetResource(j)->GetTagName(l).c_str(), "uri") == 0)
              {
              std::string buffer = project.GetPatient(i)->GetResource(j)->GetTagNameValue(l);
              std::string::size_type loc = buffer.find_last_of ("/", buffer.length());
              if( loc != std::string::npos )
                {     
                xcedeResource += buffer.erase(0, loc+1);
                } 
              else 
                {    
                xcedeResource += buffer;    
                }
              }
            }

          xcedeResource += "_Query.xcede";
          project.GetPatient(i)->GetResource(j)->SetProcessStep("XcedeQuery", listOfArguments);
          this->CreateNewXcedeFile(xcedeResource, schemaLocation);
          xmlDocPtr docResource = xmlParseFile(xcedeResource.c_str());
          this->WriteNewResourceFile(xcedeResource, docResource,  project.GetPatient(i)->GetResource(j));
          }
        }
      }
    }
}


void Xcede::WriteNewResourceFile(std::string filename, xmlDocPtr doc, Resource* res)
{
  std::vector<std::string> tagValues, tagNames;
  std::string pathT2, resourceID;
  xmlNodePtr cur;
  cur = xmlDocGetRootElement(doc);
  while (cur != NULL)
    {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"XCEDE")))
      {
      std::string id = res->GetResourceID();
      id += "-Query";
      xmlNodePtr resourceNode = xmlNewTextChild (cur, NULL, BAD_CAST "resource",NULL);
      xmlNewProp (resourceNode, BAD_CAST "ID", BAD_CAST id.c_str());
      xmlNewProp (resourceNode, BAD_CAST "format", BAD_CAST res->GetResourceInfo().format.c_str());
      xmlNewProp (resourceNode, BAD_CAST "subjectGroupID", BAD_CAST res->GetResourceInfo().subjectGroupID.c_str());
      xmlNewProp (resourceNode, BAD_CAST "subjectID", BAD_CAST res->GetResourceInfo().subjectID.c_str());
      xmlNewProp (resourceNode, BAD_CAST "projectID", BAD_CAST res->GetResourceInfo().projectID.c_str());
      xmlNewProp (resourceNode, BAD_CAST "xsi:type", BAD_CAST "moreresource_t");
      
      xmlNewTextChild (resourceNode, NULL, BAD_CAST res->GetTagName(0).c_str(),
                       BAD_CAST res->GetTagNameValue(0).c_str());
      AddProcessStep(doc, resourceNode, res);
      xmlNewTextChild (resourceNode, NULL, BAD_CAST res->GetTagName(1).c_str(),
                       BAD_CAST res->GetTagNameValue(1).c_str());

      }
    cur = cur->next;
    }

  if (doc != NULL)
    {
    xmlSaveFormatFile (filename.c_str(), doc, 2);
    xmlFreeDoc(doc);
    }
}


void Xcede::AddPatient (xmlDocPtr doc, Subject* sub, bool subjectInfo)
{
  xmlNodePtr cur;
  cur = xmlDocGetRootElement(doc);

  if(subjectInfo)
    {
    while (cur != NULL)
      {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"XCEDE")))
        {
        xmlNodePtr patientNode = xmlNewTextChild (cur, NULL, BAD_CAST "subject",NULL);
        xmlNewProp (patientNode, BAD_CAST "ID", BAD_CAST sub->GetSubjectID().c_str());
        xmlNodePtr subjectInfoNode = xmlNewTextChild (patientNode, NULL, BAD_CAST "subjectInfo", NULL);
        xmlNewProp (subjectInfoNode, BAD_CAST "xsi:type", BAD_CAST "moresubjectInfo_t");

        for(int i=0 ; i<sub->GetNbTagsInfo() ; i++)
          {
          xmlNewTextChild (subjectInfoNode, NULL, BAD_CAST sub->GetTagName(i).c_str(),
                           BAD_CAST sub->GetTagNameValue(i).c_str());
          }
        }
      cur = cur->next;
      }
    }
  else
    {
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
      {
      if ((!xmlStrcmp(cur->name, (const xmlChar *)"project")))
        {
        xmlNewTextChild (cur, NULL, BAD_CAST "subjectID", BAD_CAST sub->GetSubjectID().c_str());
        }
      cur = cur->next;
      }
    }
}


void Xcede::AddProcessStep (xmlDocPtr doc, xmlNodePtr cur, Resource *res)
{
  xmlNodePtr ProvenanceNode = xmlNewTextChild (cur, NULL, BAD_CAST "provenance",NULL);
  xmlNodePtr ProcessNode = xmlNewTextChild (ProvenanceNode, NULL, BAD_CAST "processStep",NULL);
  res->GenerateProcessStepInfo();
  xmlNewTextChild (ProcessNode, NULL, BAD_CAST "program", BAD_CAST res->GetProcessStep().program.c_str());
  xmlNewTextChild (ProcessNode, NULL, BAD_CAST "programArguments", BAD_CAST res->GetProcessStep().programArguments.c_str());
  xmlNewTextChild (ProcessNode, NULL, BAD_CAST "timeStamp", BAD_CAST res->GetProcessStep().timeStamp.c_str());
  xmlNewTextChild (ProcessNode, NULL, BAD_CAST "user", BAD_CAST res->GetProcessStep().user.c_str());
  xmlNewTextChild (ProcessNode, NULL, BAD_CAST "hostName", BAD_CAST res->GetProcessStep().hostname.c_str());
  xmlNodePtr platformNode = xmlNewTextChild (ProcessNode, NULL, BAD_CAST "platform", BAD_CAST res->GetProcessStep().platformName.c_str());
  xmlNewProp (platformNode, BAD_CAST "version", BAD_CAST res->GetProcessStep().platformVersion.c_str());
}
