#include "XcedeCatalog.h"
#include <curl.h>

std::ofstream* xcedeOutputFile = NULL;
bool xcede_write_buffer = false;
std::string xcedeHttpReply = "";
CURL* xcedeCurl;


static size_t outputFunction(char *buffer, size_t size, size_t nitems, void *userp)
{
  if(xcedeOutputFile)
    {
    xcedeOutputFile->write(buffer,size*nitems);
    }

  if(xcede_write_buffer)
    {
    xcedeHttpReply += buffer;
    }

  size *= nitems;

  return size;
}


XcedeCatalog::XcedeCatalog()
{
  m_LoginUrl = "http://rigel/midas/mymidas.php";
  m_Url = "";
  m_Catalog = "";
}


void XcedeCatalog::Login(std::string login, std::string password)
{
  std::string loginUrl = m_LoginUrl;

  curl_global_init(CURL_GLOBAL_ALL);
  xcedeCurl = curl_easy_init();
  if(!xcedeCurl) 
    {
    std::cout << "Cannot Initialize Curl!" << std::endl;
    }

  std::string log = "Email=";
  log += login;
  log += "&Password=";
  log += password;
  log += "&Submit=Log%20In";

  // Retrieve cookie session
  curl_easy_setopt(xcedeCurl,CURLOPT_COOKIEJAR,"Cookie.txt");
  curl_easy_setopt(xcedeCurl, CURLOPT_POST, 1);

  curl_easy_setopt(xcedeCurl, CURLOPT_WRITEFUNCTION, outputFunction);
  curl_easy_setopt(xcedeCurl, CURLOPT_WRITEDATA, NULL);
  curl_easy_setopt(xcedeCurl, CURLOPT_POSTFIELDS, log.c_str());
  curl_easy_setopt(xcedeCurl, CURLOPT_URL, loginUrl.c_str());
  curl_easy_perform(xcedeCurl); 
}


xmlDocPtr XcedeCatalog::ParseDoc(std::string docname)
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
  if (xmlStrcmp(cur->name, (const xmlChar *) "XCEDE"))
    {
    std::cerr<<"document of the wrong type, root node != Xcede"<<std::endl;
    xmlFreeDoc(doc);
    return (NULL);
    }
  xmlCleanupParser();

  return(doc);
}


std::vector<std::string> XcedeCatalog::GetUrls(xmlDocPtr doc, std::string catalogID)
{
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;
  std::string nameSpace = "xcede";
  std::string nameSpaceUrl = "http://www.xcede.org/xcede-2";

  std::string Request = "//" + nameSpace;
  if(catalogID != "")
    {
    Request += ":catalog[@ID=\"";
    Request += catalogID;
    Request += "\"]//@uri";
    }
  else
    {
    Request += ":entry/@uri";
    }

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


std::vector<std::string> XcedeCatalog::GetNames(xmlDocPtr doc, std::string catalogID)
{
  xmlXPathObjectPtr result;
  xmlNodeSetPtr nodeset;
  std::string nameSpace = "xcede";
  std::string nameSpaceUrl = "http://www.xcede.org/xcede-2";

  std::string Request = "//" + nameSpace;
  if(catalogID != "")
    {
    Request += ":catalog[@ID=\"";
    Request += catalogID;
    Request += "\"]//@name";
    }
  else
    {
    Request += ":entry/@name";
    }

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


xmlXPathObjectPtr XcedeCatalog::GetNodeSet (xmlDocPtr doc, xmlChar *xpath,
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


std::vector<std::string> XcedeCatalog::GetXcedeDataSets(xmlDocPtr doc, std::string CataloID, 
                                                        std::string login, std::string password)
{
  if(!(login == "" && password == ""))
    {
    xcede_write_buffer = true;
    this->Login(login, password);
    xcede_write_buffer = false;
    }

  //Get all the Urls, and the Names
  std::vector<std::string> urls = this->GetUrls(doc, CataloID);
  std::vector<std::string> filenames = this->GetNames(doc, CataloID);
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



bool XcedeCatalog::DownloadXcedeDatasets(std::string xcedeDataSet, std::string directory,
                                         std::string login, std::string password)
{
  std::string filename = this->GetXcedeFilename(xcedeDataSet);
  std::string url = this->GetXcedeUrl(xcedeDataSet);

  if(!(login == "" && password == ""))
    {
    xcede_write_buffer = true;
    this->Login(login, password);
    xcede_write_buffer = false;
    }
  else
    {
    curl_global_init(CURL_GLOBAL_ALL);
    xcedeCurl = curl_easy_init();
    if(!xcedeCurl) 
      {
      std::cout << "Cannot Initialize Curl!" << std::endl;
      }
    curl_easy_setopt(xcedeCurl, CURLOPT_WRITEFUNCTION, outputFunction);
    curl_easy_setopt(xcedeCurl, CURLOPT_WRITEDATA, NULL);
    }

  xcedeOutputFile = new std::ofstream;
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
  xcedeOutputFile->open(newfilename.c_str(), std::ios::binary);
  if(!xcedeOutputFile->is_open())
    {
    std::cout << "Cannot open fic" << std::endl;
    }

  curl_easy_setopt(xcedeCurl, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(xcedeCurl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(xcedeCurl, CURLOPT_NOPROGRESS, false);
  curl_easy_setopt(xcedeCurl, CURLOPT_FOLLOWLOCATION,true);
  curl_easy_setopt(xcedeCurl, CURLOPT_PROGRESSDATA, this);

  CURLcode res = curl_easy_perform(xcedeCurl);
  curl_easy_cleanup(xcedeCurl);

  if (res != CURLE_OK) 
    {
    fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
    xcedeOutputFile->close();
    xcedeOutputFile = NULL;
    return false;
    }
  xcedeOutputFile->close();
  xcedeOutputFile = NULL;

  return true;
}

std::string XcedeCatalog::GetXcedeFilename(std::string xcedeDataSet)
{
  std::string::size_type loc = xcedeDataSet.rfind(";", xcedeDataSet.size());
  std::string filename = xcedeDataSet.substr(loc+1);
  
  return filename;
}

std::string XcedeCatalog::GetXcedeUrl(std::string xcedeDataSet)
{
  std::string::size_type loc = xcedeDataSet.rfind(";", xcedeDataSet.size());
  std::string url = xcedeDataSet.substr(0,loc);
  
  return url;
}



