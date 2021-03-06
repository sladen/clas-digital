#include "CMetadata.hpp"

CMetadata::CMetadata () {}


/**
* @param[in] jMetadata json with metadata
*/
CMetadata::CMetadata(nlohmann::json jMetadata) {
    m_metadata = jMetadata;
}


/*
* @param[in] jMetadata new metadata
*/
void CMetadata::setMetadata(nlohmann::json jMetadata) {
    m_metadata = jMetadata;
}
    


//************ get metadata *******************//

nlohmann::json CMetadata::getMetadata() {
    return m_metadata;
}

/**
* getter function to return selected metadata
* @parameter string (sSearch: which metadata (f.e. title, date...)
* @return string 
*/
std::string CMetadata::getMetadata(std::string sSearch)
{
    std::string returnSearch = m_metadata.value(sSearch, "");
    return returnSearch;
} 
 
/**
* getter function to return selected metadata
* @parameter string (sSearch: which metadata (f.e. title, date...)
* @parameter string (sFrom: from which json (f.e. title -> data -> title) 
* @return string 
*/
std::string CMetadata::getMetadata(std::string sSearch, std::string sFrom)
{
    nlohmann::json jFrom;           //Create secon json
    jFrom = m_metadata[sFrom];

    if(jFrom.size() == 0)
        return "";

    std::string returnSearch = jFrom.value(sSearch, "");

    return returnSearch; 
}

/**
* getter function to return selected metadata
* @parameter string (sSearch: which metadata (f.e. title, date...)
* @parameter string (sFrom2: from which json (f.e. title -> data -> title) 
* @parameter string (sFrom2: in json from which json (f.e. author -> data creators -> author)
* @return string 
*/
std::string CMetadata::getMetadata(std::string sSearch, std::string sFrom1, std::string sFrom2)
{
    //Create first level
    nlohmann::json jFrom1;           
    jFrom1 = m_metadata[sFrom1];
    
    //Create second level
    nlohmann::json jFrom2;
    jFrom2 = jFrom1[sFrom2];

    if(jFrom2.size() == 0)
        return "";

    //Get data from second level
    std::string returnSearch = jFrom2.value(sSearch, "");

    return returnSearch; 
} 

/**
* getter function to return selected metadata
* @parameter string (sSearch: which metadata (f.e. title, date...)
* @parameter string (sFrom2: from which json (f.e. title -> data -> title) 
* @parameter string (sFrom2: in json from which json (f.e. author -> data creators -> author)
* @parameter int (index: in case of list: which element from list)
* @return string 
*/
std::string CMetadata::getMetadata(std::string sSearch, std::string sFrom1, std::string sFrom2, int in)
{
    //Create first level
    nlohmann::json jFrom1;           
    jFrom1 = m_metadata[sFrom1];

    if(jFrom1.size() == 0)
        return "";

    //Create vector of jsons
    nlohmann::json listJsons = jFrom1[sFrom2];

    //Check whether element could be accessed
    if(listJsons.size() == 0)
        return "";

    //Create second level
    nlohmann::json jFrom2;
    jFrom2 = listJsons[in]; 

    //Get data from second level
    std::string returnSearch = jFrom2.value(sSearch, "");

    return returnSearch; 
}


/**
* @return vector with all collections this book is in
*/
std::vector<std::string> CMetadata::getCollections()
{
    //Create empty vector as default
    std::vector<std::string> vDefault;

    //Get json "data"
    nlohmann::json jData;
    jData = m_metadata["data"];

    if(jData.size() == 0)
        return vDefault;

    //Get collection
    std::vector<std::string> sCollections = jData["collections"];

    return sCollections;
}

/**
* @return lastName, or Name of author
*/
std::string CMetadata::getAuthor()
{
    //Get author. Try to find "lastName"
    std::string sAuthor = getMetadata("lastName", "data", "creators", 0);

    //std::string sAuthor = m_metadata["data"]["creators"][0]["lastName"];


    //If string is empty, try to find "name"
    if(sAuthor.size() == 0)
        sAuthor = getMetadata("name", "data", "creators", 0);

    //Return result: either Name of author or empty string
    return sAuthor;
}

/**
* @return title of book
*/
std::string CMetadata::getTitle() {
    return getMetadata("title", "data");
}

/**
* @return short-title of book (Title if not exists)
*/
std::string CMetadata::getShortTitle() {
    std::string sReturn = getMetadata("shortTitle", "data");
    if(sReturn == "")
        return getTitle();
    else
        return sReturn;
}

/**
* @return date or -1 if date does not exists or is currupted
*/
int CMetadata::getDate()
{
    //Create regex
    std::regex date3(".*(\\d{3}).*");
    std::regex date4(".*(\\d{4}).*");
    std::smatch m;

    //Get date from json
    std::string sDate = getMetadata("date", "data");

    //Check whether regex find match with a 4-digit. 
    if(std::regex_search(sDate, m, date4))
        return std::stoi(m[1]);
    //Check whether regex find match with a 3-digit. 
    if(std::regex_search(sDate, m, date3))
        return std::stoi(m[1]);

    //Return -1 if regex nether matched
    return -1;
}

/**
* @return string with Auhtor + first 6 words 15 words of title + date
*/
std::string CMetadata::getShow()
{
    // *** Add Author *** //
    std::string sResult = getAuthor();

    if(getDate() != -1)
        sResult += ", " + std::to_string(getDate());

    sResult += ".";

    return sResult;
}
/**
* @return string with Auhtor + first 6 words 15 words of title + date
*/
std::string CMetadata::getShow2()
{
    // *** Add Author *** //
    std::string sResult = getAuthor() + ", \"";

    // *** Add first [num] words of title *** //
    std::vector<std::string> vStrs;
    func::split(getShortTitle(), " ", vStrs);
    for(unsigned int i=0; i<10 && i<vStrs.size(); i++)
        sResult += vStrs[i] + " ";

    if(vStrs.size() > 10)
        sResult += "...\"";
    else
        sResult += "\"";

    if(getDate() != -1)
        sResult += ", " + getDate();

    sResult += ".";

    return sResult;
}


