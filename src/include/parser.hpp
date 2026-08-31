#include <pugixml.hpp>
#include <converter.hpp>
#include <exceptions.h>
#include <string>
#include <thread>
#include<fstream>
#include <filesystem>

/*
    In theory, pre-reserving at least the average amount of string
    space we'll need for a reference should make the program a little more efficient.
    So, one Google and some manual character counting later...
*/
const int AVERAGE_REFERENCE_LEN = 148;  

class Parser {
    public: 
    Parser(bool multithreaded, int threads) {
        shouldUseThreads = multithreaded;
        numThreads = threads;
        stringPool = new std::string[multithreaded ? threads : 1];
    }
    void Ingest(std::string filepath) {
        try {
            pugi::xml_parse_result result = source.load_file(filepath.c_str());
            if (result.status != pugi::xml_parse_status::status_ok) {
                throw std::exception();
            }
        } catch (std::exception e) {
            throw IngestionException(filepath);
        } 
    }


    void Parse() { 
        if (source.empty()) {
            throw ConversionException(EMPTY);
        }
        taskQueue = source.select_nodes(".//*[self::verse]");
        
        if (shouldUseThreads) {
            ParseMultithreaded();
        } else {
            std::string result;
            pugi::xml_node currentNode;
                try {
                    int size = taskQueue.size();
                result.reserve(size * AVERAGE_REFERENCE_LEN);
                for (int i = 0; i < size; i++) {
                    currentNode = taskQueue[i].node();
                    result.append("\"" + 
                    Converter::OSIStoJSONId(currentNode.attribute("osisID").value()) +
                    "\": \"" + currentNode.text().get() + "\"" + 
                    (i == size - 1 ? "" : ",") +
                    '\n');
                }
                stringPool[0] = result;
            } catch (std::exception e) {
                throw ConversionException(NONE);
            }
        }
    }

    void Print(std::string out) {

        using namespace std::filesystem;

        if (!exists(out)) {
            try {
               path p = out;
               path parent = p.parent_path();
               create_directories(parent);
            } catch (std::exception e) {
                throw PrintException(out);
            }
        }else if (!is_regular_file(out)) {
            throw PrintException(out);
        }

        try {
            std::ofstream output(out);
            output << "{\n";
            for (int i = 0; i < (shouldUseThreads ? numThreads : 1); i++ ) {
                 output << stringPool[i];
            }
            output << "}";
            output << std::endl;
        } catch (std::exception e) {
            throw PrintException(out);
        }
    }

    private:
    bool shouldUseThreads;
    int numThreads;
    pugi::xml_document source;
    std::string* stringPool;
    pugi::xpath_node_set taskQueue;

    //Underlying parsing functions!!
    void ParseMultithreaded() {
        taskQueue = source.select_nodes(".//*[self::verse]");
        std::vector<std::thread> threadPool;
        int queueSize = taskQueue.size();
        int workloadSize = queueSize / numThreads;
        //Set up our thread pool
        for (int i = 0; i < numThreads; i++) {
            int start = workloadSize * i;
            int end = i == numThreads - 1 ? queueSize - 1 : (workloadSize * (i + 1)) - 1;
            threadPool.emplace_back(&Parser::ParseWorker, this, start, end, i);
        }
        for (std::thread& t : threadPool) {
            t.join();
        }
    }

    void ParseWorker(const int poolStart, const int poolEnd, int insertionIdx) {
        std::string result;
        result.reserve(AVERAGE_REFERENCE_LEN * ((poolEnd - poolStart) + 1));

        for (int i = poolStart; i <= poolEnd; i++) {

            pugi::xml_node node = taskQueue[i].node();
            result.append("\"" + 
            Converter::OSIStoJSONId(node.attribute("osisID").value()) +
            "\": \"" + node.text().get() + "\"" + 
            (insertionIdx == numThreads-1 && i == poolEnd ? "" : ",") +
            '\n');

        }
        stringPool[insertionIdx] = result;
    } 

};

