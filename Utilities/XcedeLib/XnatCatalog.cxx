#include "XnatCatalog.h"
#include <curl.h>

std::ofstream* xnatOutputFile = NULL;
bool xnat_write_buffer = false;
std::string xnatHttpReply = "";
CURL* xnatCurl;

static size_t outputFunction(char *buffer, size_t size, size_t nitems, void *userp)
{
  if(xnatOutputFile)
    {
    xnatOutputFile->write(buffer,size*nitems);
    }

  if(xnat_write_buffer)
    {
    std::string buf = buffer;
    buf.resize(size*nitems);
    xnatHttpReply += buf;
    }

  size *= nitems;

  return size;
}


XnatCatalog::XnatCatalog()
{
  m_LoginUrl = "http://central.xnat.org/app/action/XDATLoginUser"; 
  m_Url = "";
  m_Catalog = "";
}


void XnatCatalog::Login(std::string login, std::string password)
{
  std::string loginUrl = m_LoginUrl;

  curl_global_init(CURL_GLOBAL_ALL);
  xnatCurl = curl_easy_init();
  if(!xnatCurl) 
    {
    std::cout << "Cannot Initialize Curl!" << std::endl;
    }
  curl_easy_setopt(xnatCurl, CURLOPT_WRITEFUNCTION, outputFunction);
  curl_easy_setopt(xnatCurl, CURLOPT_WRITEDATA, NULL);

  loginUrl += "?username=";
  loginUrl += login;
  loginUrl += "&password=";
  loginUrl += password;
  curl_easy_setopt(xnatCurl, CURLOPT_URL,loginUrl.c_str());

  xnatHttpReply = "";
  CURLcode res = curl_easy_perform(xnatCurl);

  //Get The SessionID
  std::string::size_type loc1 = xnatHttpReply.find("jsessionid=", 0);
  std::string sessionId = xnatHttpReply.substr(loc1+11, 32);
  m_SessionId = sessionId;
}


xmlDocPtr XnatCatalog::ParseDoc(std::string docname)
{
  xmlDocPtr doc;
  xmlNodePtr cur;
  doc = xmlParseFile(docname.c_str());

  if (doc == NULL )
    {
    std::cout<<"Document not parsed successfully"<<std::endl;
    return (NULL);
    }

  cur = xmlDocGetRootElement(doc);

  if (cur == NULL)
    {
    std::cerr<<"empty document"<<std::endl;
    xmlFreeDoc(doc);
    return (NULL);
    }
  if (xmlStrcmp(cur->name, (const xmlChar *) "Catalog"))
    {
    std::cerr<<"document of the wrong type, root node != cat:Catalog"<<std::endl;
    xmlFreeDoc(doc);
    return (NULL);
    }
  xmlCleanupParser();

  return(doc);
}


xmlDocPtr XnatCatalog::ParseDocMemory(std::string buffer)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseMemory(buffer.c_str(), strlen(buffer.c_str()));

  if (doc == NULL )
    {
    std::cout<<"Document not parsed successfully"<<std::endl;
    return (NULL);
    }

  cur = xmlDocGetRootElement(doc);

  if (cur == NULL)
    {
    std::cerr<<"empty document"<<std::endl;
    xmlFreeDoc(doc);
    return (NULL);
    }
  if (xmlStrcmp(cur->name, (const xmlChar *) "Catalog"))
    {
    std::cerr<<"document of the wrong type, root node != cat:Catalog"<<std::endl;
    xmlFreeDoc(doc);
    return (NULL);
    }
  xmlCleanupParser();

  return(doc);
}


std::vector<std::string> XnatCatalog::GetUrls(xmlDocPtr doc, std::string login, std::string password)
{
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;
  std::string nameSpace = "cat";
  std::string nameSpaceUrl = "http://nrg.wustl.edu/catalog";

  std::string Request = "//";
  Request += nameSpace;
  Request += ":entry";
  Request += "/@URI";

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


std::vector<std::string> XnatCatalog::GetNames(xmlDocPtr doc, std::string login, std::string password)
{
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;
  std::string nameSpace = "cat";
  std::string nameSpaceUrl = "http://nrg.wustl.edu/catalog";

  std::string Request = "//";
  Request += nameSpace;
  Request += ":entry";
  Request += "/@cachePath";

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


xmlXPathObjectPtr XnatCatalog::GetNodeSet (xmlDocPtr doc, xmlChar *xpath,
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


xmlDocPtr XnatCatalog::GetNewCatalog(xmlDocPtr mainDoc, std::string login, std::string password)
{
  xnat_write_buffer = true;
  if(!(login == "" && password == ""))
    {
    this->Login(login, password);
    }
  else
    {
    curl_global_init(CURL_GLOBAL_ALL);
    xnatCurl = curl_easy_init();
    if(!xnatCurl) 
      {
      std::cout << "Cannot Initialize Curl!" << std::endl;
      }
    curl_easy_setopt(xnatCurl, CURLOPT_WRITEFUNCTION, outputFunction);
    curl_easy_setopt(xnatCurl, CURLOPT_WRITEDATA, NULL);
    }


  //Get the main Url
  std::vector<std::string> urls = this->GetUrls(mainDoc, login, password);
  std::string mainUrl = urls[0];
  if(!(login == "" && password == ""))
    {
    mainUrl += ";jsessionid=";
    mainUrl += m_SessionId;
    }
  curl_easy_setopt(xnatCurl, CURLOPT_URL, mainUrl.c_str());
  xnatHttpReply = "";
  CURLcode res = curl_easy_perform(xnatCurl);
  curl_easy_cleanup(xnatCurl);

  xmlDocPtr doc = this->ParseDocMemory(xnatHttpReply);

  return doc;
}

std::vector<std::string> XnatCatalog::GetXnatDataSets(xmlDocPtr mainDoc, std::string login, std::string password)
{
  xmlDocPtr doc = this->GetNewCatalog(mainDoc, login, password);

  // Get all the Urls, and the Names
  std::vector<std::string> urls = this->GetUrls(doc, login, password);
  std::vector<std::string> filenames = this->GetNames(doc, login, password);
  std::vector<std::string> dataSet;

  for(int i=0; i<(int)urls.size() ; i++)
    {
    std::string newData = urls[i];
    newData += ";";
    newData += filenames[i];
    dataSet.push_back(newData);
    }
  return dataSet;
}


std::vector<std::string> XnatCatalog::GetXnatDataSetNames(xmlDocPtr mainDoc, std::string login, std::string password)
{
  xmlDocPtr doc = this->GetNewCatalog(mainDoc, login, password);
  
  std::vector<std::string> names = this->GetNames(doc, login, password);

  return names;
}


bool XnatCatalog::DownloadXnatDatasets(std::string xnatDataSet, std::string directory,
                                       std::string login, std::string password)
{
  std::string filename = this->GetXnatFilename(xnatDataSet);
  std::string url = this->GetXnatUrl(xnatDataSet);

  if(!(login == "" && password == ""))
    {
    xnat_write_buffer = true;
    this->Login(login, password);
    xnat_write_buffer = false;
    url += ";jsessionid=";
    url += m_SessionId;
    }
  else
    {
    curl_global_init(CURL_GLOBAL_ALL);
    xnatCurl = curl_easy_init();
    if(!xnatCurl) 
      {
      std::cout << "Cannot Initialize Curl!" << std::endl;
      }
    curl_easy_setopt(xnatCurl, CURLOPT_WRITEFUNCTION, outputFunction);
    curl_easy_setopt(xnatCurl, CURLOPT_WRITEDATA, NULL);
    }

  xnatOutputFile = new std::ofstream;
  std::string::size_type loc = filename.find( "/", 0);

  while(loc != std::string::npos )
    {
    std::string newDir = filename;
    newDir.erase(loc, (filename.length())-loc);
    directory += "/" + newDir;
    filename.erase(0, loc+1);
    loc = filename.find( "/", 0);
    }
  itksys::SystemTools::MakeDirectory(directory.c_str());

  std::string newfilename = directory + "/" + filename;
  xnatOutputFile->open(newfilename.c_str(), std::ios::binary);
  if(!xnatOutputFile->is_open())
    {
    std::cout << "Cannot open fic" << std::endl;
    }

  curl_easy_setopt(xnatCurl, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(xnatCurl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(xnatCurl, CURLOPT_NOPROGRESS, false);
  curl_easy_setopt(xnatCurl, CURLOPT_FOLLOWLOCATION,true);
  curl_easy_setopt(xnatCurl, CURLOPT_PROGRESSDATA, this);
  CURLcode res = curl_easy_perform(xnatCurl);
  curl_easy_cleanup(xnatCurl);

  if (res != CURLE_OK) 
    {
    fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
    xnatOutputFile->close();
    xnatOutputFile = NULL;
    return false;
    }
  xnatOutputFile->close();
  xnatOutputFile = NULL;

  return true;
}

std::string XnatCatalog::GetXnatFilename(std::string xnatDataSet)
{
  std::string::size_type loc = xnatDataSet.rfind(";", xnatDataSet.size());
  std::string filename = xnatDataSet.substr(loc+1);
  
  return filename;
}

std::string XnatCatalog::GetXnatUrl(std::string xnatDataSet)
{
  std::string::size_type loc = xnatDataSet.rfind(";", xnatDataSet.size());
  std::string url = xnatDataSet.substr(0,loc);
  
  return url;
}
