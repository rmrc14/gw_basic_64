//// ProgramMemory.cpp
//
//// ProgramMemory.cpp
//
//#include "ProgramMemory.h"
//#include <string>
//#include <map>
//#include <vector>
//#include <sstream>  // for std::to_string
//#include "ConsoleIO.h"  // adjust path if needed
//
//namespace gw_basic {
//    namespace storage {
//
//        ProgramMemory::ProgramMemory() {
//            // Optional: initialize anything if needed
//        }
//
//         ProgramMemory::storeLine(int lineNumber, const std::string& code) {
//            if (code.empty()) {
//                programLines.erase(lineNumber);
//            }
//            else {
//                programLines[lineNumber] = code;
//            }
//        }
//
//        std::vector<std::string> ProgramMemory::getAllLines() const {
//            std::vector<std::string> lines;
//            for (const auto& entry : programLines) {
//                int num = entry.first;
//                const std::string& code = entry.second;
//                lines.push_back(std::to_string(num) + " " + code);
//            }
//            return lines;
//        }
//
//        void ProgramMemory::list(gw_basic::IO::ConsoleIO& console) const {
//            for (const auto& entry : programLines) {
//                int num = entry.first;
//                const std::string& code = entry.second;
//                console.printLine(std::to_string(num) + " " + code);
//            }
//        }
//
//    } // namespace storage
//} // namespace gw_basic
// gw_basic/Storage/ProgramMemory.cpp
#include "ProgramMemory.h"
#include "IO/ConsoleIO.h"
#include <algorithm>
#include <sstream>
#include <vector>

//namespace gw_basic {
  //  namespace storage {

        ProgramMemory::ProgramMemory() {
            // Initialize empty memory
        }

        void ProgramMemory::storeLine(int lineNumber, const std::string& code) {
            if (lineNumber < 0) {
                throw std::invalid_argument("Line number cannot be negative");
            }

            if (code.empty()) {
                memory.erase(lineNumber);  // Remove line if code is empty
            }
            else {
                memory[lineNumber] = code;  // Add/update line
            }
        }

      //  std::map<int, std::string> ProgramMemory::getAllLines() const {
            std::map<int, std::string> ProgramMemory::getAllLines() const {
            return memory;  // Return copy of the memory map
        }

       // void ProgramMemory::list(gw_basic::IO::ConsoleIO& console) const {
            void ProgramMemory::list(ConsoleIO & console) const {
            for (const auto& entry: memory) {
                int lineNumber = entry.first;
                const std::string& code = entry.second;
                std::ostringstream oss;
                oss << lineNumber << " " << code;                                                                                       
                console.printLine(oss.str().c_str());
            }
        }

     
            std::vector<int> ProgramMemory::getLineNumbers() const {
            std::vector<int> numbers;
            for (const auto& entry : memory) {
                numbers.push_back(entry.first);
            }
            return numbers;
        }

       void ProgramMemory::clear() {
            memory.clear();
        }

 //   } // namespace storage
//} // namespace gw_basic
