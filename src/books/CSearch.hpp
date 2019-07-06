#include <iostream>
#include <string>
#include <list>
#include <map>
#include "CBook.hpp"
#include "CSearchOptions.hpp"
#include "func.hpp"
#include "fuzzy.hpp"
#include "src/console/console.hpp"

#pragma once 

class CSearch
{
private:
    unsigned long long m_ID;
    std::string m_sWord;
    CSearchOptions* m_sOpts;
    float m_fProgress;

public:
    
    /**
    * @brief constructor
    */
    CSearch(CSearchOptions* searchOpts, unsigned long long sID);

    // *** GETTER *** //

    /**
    * @return id of search
    */
    unsigned long long getID();

    /**
    * @return searched word from searchOptions
    */
    std::string getSearchedWord();

    /**
    * @return progress
    */
    float getProgress();

    // *** SETTER *** //

    /**
    * @brief set searched word.
    * param[in] searchedWord set searched word
    */
    void setWord(std::string sWord);


    std::map<std::string, CBook*>* search(std::map<std::string, std::map<std::string, CBook*>>& mWs, std::map<std::string, std::map<std::string, CBook*>>& mWsTitle, std::map<std::string, double>& matches);
    /**
    * @brief search full-match
    * @param[in] mapWords map of all words with a list of books in which this where accures
    * @param[in, out] mapSR map of search results
    */
    void normalSearch(std::map<std::string, std::map<std::string, CBook*>>& mapWords,
                                                            std::map<std::string, CBook*>* mapSR);
    /**
    * @brief search contains
    * @param[in] mapWords map of all words with a list of books in which this where accures
    * @param[in, out] mapSR map of search results
    */
    void containsSearch(std::map<std::string, std::map<std::string, CBook*>>& mapWords,
                     std::map<std::string, CBook*>* mapSR, std::map<std::string, double>& matches);

    /**
    * @brief search fuzzy 
    * @param[in] mapWords map of all words with a list of books in which this word accures
    * @param[in, out] mapSR searchresults
    */
    void fuzzySearch(std::map<std::string, std::map<std::string, CBook*>>& mapWords, 
                     std::map<std::string, CBook*>* mapSR, std::map<std::string, double>& matches);

    /**
    * @brief check whether searched word matches with author of a book.
    */
    std::map<std::string, CBook*>* checkAuthor(std::map<std::string, CBook>& mapBooks);

    /**
    * @brief remove all books that do not match with searchoptions
    * @param[in, out] mapSR map of search results
    */
    void removeBooks(std::map<std::string, CBook*>* mapSR);

    /**
    * @brief check whether book-metadata matches with searchoptions
    * @param[in] book to be checked
    * return Boolean
    */
    bool checkSearchOptions(CBook* book);

    /*
    * @brief inserts searchResults into map of searchresults and assigns value of match
    * @param[out] mapSR
    * @param[in] found
    * @param[out] matches
    * @param[in] value
    */
    void myInsert(std::map<std::string, CBook*>* mapSR, std::map<std::string, CBook*>& found,
                            std::map<std::string, double>& matches, double value);

    /**
    * @brief delete searchOptions
    */
    void deleteSearchOptions();

};
