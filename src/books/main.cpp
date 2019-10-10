#include <iostream>
#include <list>
#include "json.hpp"
#include "CBookManager.hpp"
#include "func.hpp"
#include "src/console/console.hpp"

int main()
{
    CBookManager manager;

    std::ifstream read("bin/zotero.json");
    nlohmann::json jItems;
    read >> jItems;
    read.close();
    alx::cout.write ("updating zotero.\n");
    manager.updateZotero(jItems);
    bool check = manager.initialize();

    if(check == false)
    {
        alx::cout.write ("Failed to initialize.\n");
        return 0;
    }

    for(;;)
    {
        alx::cout.write ("> ");
        std::string sInput = alx::cout.getCommand();
        func::convertToLower(sInput);

        if(sInput == "q")
            return 0;

        alx::cout.write ("\nFuzzyness: ");
        std::string sFuzzy = alx::cout.getCommand();
        int fuzzy = std::stoi(sFuzzy);

        CSearchOptions* searchOpts = new CSearchOptions(sInput, fuzzy, {"RFWJC42V", "XCFFDRQC", "RBB8DW5B", "WIXP3DS3"}, false, false, "", 0 , 2019, 1, true);
        CSearch* search = new CSearch(searchOpts, 0, 100000);
        manager.addSearch(search);

        /*
        alx::cout.write("\n");
        std::list<std::string>* suggestions = manager.getSuggestions(sInput);
        for(auto it = suggestions->begin(); it !=suggestions->end(); it++)
            alx::cout.write((*it), "\n");
        */

        alx::cout.write ("\nSearching for ", sInput, "... \n");

        std::list<CBook*>* searchResults = manager.search(0);
        unsigned int counter = 0;
        for(auto it=searchResults->begin(); it!=searchResults->end(); it++)
        {
            alx::cout.write (alx::console::green_black, (*it)->getKey(), ": ", (*it)->getMetadata().getShow(), "\n", alx::console::blue_black, (*it)->getPreview(sInput), "\n");

            //alx::cout.write("num results: ", (*it)->getNumMatches(sInput, fuzzy), "\n");

            // *** Print Pages *** //
            alx::cout.write(alx::console::yellow_black, "Pages: ");

            std::map<int, std::vector<std::string>>* mapPages = (*it)->getPages(sInput, fuzzy);
            for(auto jt=mapPages->begin(); jt!=mapPages->end(); jt++)
            {
                alx::cout.write(jt->first);

                //If there are no found words of this page (f.e. full-search) -> continue)
                if(jt->second.size() == 0)
                {
                    alx::cout.write(", ");
                    continue;
                }

                //Print words on this page
                alx::cout.write(" (");
                for(size_t i=0; i<jt->second.size(); i++)
                    alx::cout.write (jt->second[i], ", ");
                alx::cout.write ("\b\b), ");
            }

            //Delete results
            delete mapPages;
            alx::cout.write ("\b\b\n");

            counter++;
        }
        alx::cout.write("Results found: ", (int)counter, "\n");
   }
}
     



