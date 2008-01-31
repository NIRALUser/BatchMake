#include "Subject.h"

Subject::Subject()
{
  m_NbResources = 0;
}


void Subject::SetTagName(std::string tagname)
{
  TagNameSubject newTag;
  newTag.name = tagname;
  m_TagName.push_back(newTag);
}


void Subject::AddResource()
{
  Resource* resource = new Resource;
  m_Resources.push_back(resource);
  m_NbResources++;
}