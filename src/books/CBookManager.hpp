#include <iostream>
#include <string>
#include <list>
#include <map>
#include <fstream>
#include <dirent.h>
#include <shared_mutex>
#include "CBook.hpp"
#include "CSearch.hpp"
#include "CSearchOptions.hpp"
#include "func.hpp"
#include "src/console/console.hpp"

#pragma once 

class CBookManager
{
private:

    //Map of all books
    std::map<std::string, CBook> m_mapBooks;
    std::map<std::string, std::map<std::string, CBook*>> m_mapWords;
    std::map<std::string, std::map<std::string, CBook*>> m_mapWordsTitle;

    std::map<unsigned long long, CSearch*> m_mapSearchs;

    std::shared_mutex m_searchLock;

public:

    // **** getter **** //

    /**
    * @return map of all book
    */
    std::map<std::string, CBook>& getMapOfBooks();

    /**
    * @brief load all books.
    * @return boolean for successful of not
    */
    bool initialize(); 

    /**
    * @brief parse json of all items. If item exists, change metadata of item, create new book.
    * @param[in] j_items json with all items
    */
    void updateZotero(nlohmann::json j_Items);

    /**
    * @brief add a book, or rather: add ocr to book
    * @param[in] sKey key to book
    */
    void addBook(std::string sKey);

    /**
    * @brief search function calling fitting function from search class
    * @return list of all found books
    */
    std::map<std::string, CBook*>* search(unsigned long long id);

    /**
    * @brief create map of all words (key) and books in which the word occurs (value)
    */
    void createMapWords();

    /**
    * @brief create map of all words (key) and book-titles in which the word occurs (value)
    */
    void createMapWordsTitle();

    /**
    * @brief add a new search
    */
    void addSearch(CSearch* search);

    /**
    * @brief get progress of given search
    */
    float getProgress(unsigned long long id);

    /**
    * @brief delete given search and erase from map
    */
    void deleteSearch(unsigned long long id);
}; 



