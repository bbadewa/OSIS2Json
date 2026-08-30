#include <string>
#include <vector>
#include <algorithm>
enum SelectionOption {
				Verse = 1,
				Chapter = 1000,
				Book = 1000000
			};
class Converter {
    private: 
        inline static const std::vector<std::string> defaultOsisIds = {
            "Gen",
            "Exod",
            "Lev",
            "Num",
            "Deut",
            "Josh",
            "Judg",
            "Ruth",
            "1Sam",
            "2Sam",
            "1Kgs",
            "2Kgs",
            "1Chr",
            "2Chr",
            "Ezra",
            "Neh",
            "Esth",
            "Job",
            "Ps",
            "Prov",
            "Eccl",
            "Song",
            "Isa",
            "Jer",
            "Lam",
            "Ezek",
            "Dan",
            "Hos",
            "Joel",
            "Amos",
            "Obad",
            "Jonah",
            "Mic",
            "Nah",
            "Hab",
            "Zeph",
            "Hag",
            "Zech",
            "Mal",
            "Matt",
            "Mark",
            "Luke",
            "John",
            "Acts",
            "Rom",
            "1Cor",
            "2Cor",
            "Gal",
            "Eph",
            "Phil",
            "Col",
            "1Thess",
            "2Thess",
            "1Tim",
            "2Tim",
            "Titus",
            "Phlm",
            "Heb",
            "Jas",
            "1Pet",
            "2Pet",
            "1John",
            "2John",
            "3John",
            "Jude",
            "Rev",
        };
        static std::vector<std::string> split(std::string string, char splitter) { //Yoinked directly from KuwagataDLL-C++->util.cpp!
		    std::vector<std::string> ret;
		    int caret = string.find(splitter);
		    while (caret != std::string::npos) {
		    	ret.push_back(string.substr(0, caret));
		    	string = string.substr(caret+1);
		    	caret = string.find(splitter);
		    }
		    ret.push_back(string);
		    return ret;
	    }   
    public:
        static std::string OSIStoJSONId(std::string OSISId) {
            int JSONid = 0;
            std::vector<std::string> elements = split(OSISId, '.');
            JSONid += std::distance(defaultOsisIds.begin(), std::find(defaultOsisIds.begin(), defaultOsisIds.end(), elements[0])) + 1;
            JSONid *= Book;
            JSONid += atoi(elements[1].c_str()) * Chapter;
            JSONid += atoi(elements[2].c_str());

            return std::to_string(JSONid);
        };
};