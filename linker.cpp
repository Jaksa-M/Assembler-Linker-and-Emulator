#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <elf.h>

struct RelocationEntry {
  int offset; // Offset within the section
  std::string type = "R_X86_64_32S"; // Type of relocation
  int symbol; // Symbol index in symbol table
  int addend; // Addend value

  RelocationEntry(int off, int sym, int add)
    : offset(off), symbol(sym), addend(add) {}
};
std::map<std::string, std::map<int, std::vector<RelocationEntry>>> filesRelocationTables; // key is name of the input file
std::map<std::string, std::vector<std::string>> relocationTablesNames; // key is file name, value is vector of section names for that file

struct SymbolTableEntry {
  static int currentNum;
  int num;
  int value;
  std::string type;
  std::string bind;
  int section_index;
  std::string name;
  std::string defined; //used to check if value is valid or not, so decides to put it in flink table
  SymbolTableEntry(int val, const std::string& typ, const std::string& bin, int sec_index, const std::string& nam, const std::string& def)
      : num(currentNum++), value(val), type(typ), bind(bin), section_index(sec_index), name(nam), defined(def) {}
};
int SymbolTableEntry::currentNum = 0;
std::map<std::string, std::vector<SymbolTableEntry>> symbolTables; // key is name of the input file


std::map<std::string, std::map<std::string, std::vector<uint8_t>>> filesSectionsMemory; // Map that holds sections data (like memory from assembler)


void readELF(std::vector<std::string> inputFiles){
    for(int i = 0; i < inputFiles.size(); i++){
        FILE* file = fopen(inputFiles[i].c_str(), "rb");
        if (!file) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        // Get the file size
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        rewind(file);

        // Read the file into a buffer
        char* elfFile = new char[fileSize];
        fread(elfFile, 1, fileSize, file);
        fclose(file);

        //read the ELF header
        Elf64_Ehdr* ehdr = reinterpret_cast<Elf64_Ehdr*>(elfFile);


        //read the section headers
        Elf64_Shdr* shdr = reinterpret_cast<Elf64_Shdr*>(elfFile + ehdr->e_shoff);

        // Locate the shstrtab
        char* shstrtab = elfFile + shdr[ehdr->e_shstrndx].sh_offset;

        // Find the .symtab, .strtab, and .rela sections
        Elf64_Shdr* symtabShdr = nullptr;
        Elf64_Shdr* strtabShdr = nullptr;
        std::vector<Elf64_Shdr*> relaShdrs;

        for (int i = 0; i < ehdr->e_shnum; i++) {
            std::string sectionName = shstrtab + shdr[i].sh_name;
            if (sectionName == ".symtab") {
                symtabShdr = &shdr[i];
            } else if (sectionName == ".strtab") {
                strtabShdr = &shdr[i];
            } else if (sectionName.find(".rela") == 0) {
                relaShdrs.push_back(&shdr[i]);
            }
        }

        std::map<std::string, std::vector<uint8_t>> sectionData;
        // Loop through each section to store data
        for (int j = 0; j < ehdr->e_shnum; j++) {
            std::string sectionName = shstrtab + shdr[j].sh_name;
            std::vector<uint8_t> data;

            if (shdr[j].sh_type == SHT_PROGBITS) {
                uint8_t* sectionStart = reinterpret_cast<uint8_t*>(elfFile + shdr[j].sh_offset);
                data.assign(sectionStart, sectionStart + shdr[j].sh_size);
                sectionData[sectionName] = data;
            }
        }
        filesSectionsMemory[inputFiles[i]] = sectionData;

        //parsing the symbol table
        std::vector<SymbolTableEntry> symbolTable;
        if (symtabShdr && strtabShdr) {
            Elf64_Sym* symtab = reinterpret_cast<Elf64_Sym*>(elfFile + symtabShdr->sh_offset);
            char* strtab = elfFile + strtabShdr->sh_offset;
            int numSymbols = symtabShdr->sh_size / sizeof(Elf64_Sym);

            for (int i = 0; i < numSymbols; ++i) {
                int value = symtab[i].st_value;
                int sectionIndex = symtab[i].st_shndx;
                std::string name = strtab + symtab[i].st_name;

                // Determine bind and type
                std::string bind = (ELF64_ST_BIND(symtab[i].st_info) == STB_LOCAL) ? "LOC" : "GLOB";
                std::string type = (ELF64_ST_TYPE(symtab[i].st_info) == STT_SECTION) ? "SCTN" : "NOTYP";

                symbolTable.emplace_back(value, type, bind, sectionIndex, name, "defined");
            }
        }
        symbolTables[inputFiles[i]] = symbolTable;
        
        //parsing relocations tables
        std::vector<std::string> names; //using this temporary variable to store names of sections for current file 
        std::map<int, std::vector<RelocationEntry>> relocationTables; // key is section number
        for (auto& relaShdr : relaShdrs) {
            Elf64_Rela* rela = reinterpret_cast<Elf64_Rela*>(elfFile + relaShdr->sh_offset);
            int numRelocs = relaShdr->sh_size / sizeof(Elf64_Rela);
            int sectionIndex = relaShdr->sh_info - 1; // Added "-1" cause without it will start from 2 like it is in section header. It doesnt matter

            if(numRelocs == 0) relocationTables[sectionIndex];
            for (int i = 0; i < numRelocs; i++) {
                int offset = rela[i].r_offset;
                int symbol = ELF64_R_SYM(rela[i].r_info);
                int addend = rela[i].r_addend;

                relocationTables[sectionIndex].emplace_back(offset, symbol, addend);
            }
            names.push_back(shstrtab + shdr[relaShdr->sh_info].sh_name);
        }
        filesRelocationTables[inputFiles[i]] = relocationTables;
        relocationTablesNames[inputFiles[i]] = names;

        delete[] elfFile;
    }
}

struct LinkerOptions {
    std::string outputFile;
    std::map<std::string, std::string> sectionPlacements;
    bool isHex = false;
    bool isRelocatable = false;
    std::vector<std::string> inputFiles;
};

void printUsage() {
    std::cerr << "Usage: ./linker [-o output_file] [-place=section@address] [-hex | -relocatable] input_files...\n";
    exit(EXIT_FAILURE);
}

LinkerOptions parseArguments(int argc, char* argv[]) {
    LinkerOptions options;
    bool output_file_check = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if(output_file_check == true){
            output_file_check = false;
            options.outputFile = arg;
            continue;
        }

        if (arg == "-hex") {
            options.isHex = true;
        } else if (arg == "-relocatable") {
            options.isRelocatable = true;
        } else if (arg.rfind("-o", 0) == 0) {
            output_file_check = true;
            
        } else if (arg.rfind("-place=", 0) == 0) {
            std::string place = arg.substr(7);
            size_t atPos = place.find('@');
            if (atPos == std::string::npos) {
                printUsage();
            }
            std::string section = place.substr(0, atPos);
            std::string address = place.substr(atPos + 1);
            options.sectionPlacements[section] = address;
        } else {
            options.inputFiles.push_back(arg);
        }
    }

    if (!(options.isHex || options.isRelocatable)) {
        std::cerr << "Error: Either -hex or -relocatable must be specified.\n";
        printUsage();
    }

    if (options.inputFiles.empty()) {
        std::cerr << "Error: No input files specified.\n";
        printUsage();
    }

    return options;
}

struct MemorySection {
    std::string name;
    std::vector<uint8_t> data; // The actual data in the section
    uint address; // Starting address of the section
};

std::map<std::string, MemorySection> memoryMap; // key is section name
std::map<std::string, std::map<std::string, uint>> fileSectionStartAddresses; // Map to store the starting address of each section for each input file
// key is section name, value is its starting address (differs from the map above because this tracks only beggining addresses of each section)
std::map<std::string, uint> startAddressEachSection; 
std::map<std::string, SymbolTableEntry> unifiedSymbolTable; // Unified symbol table

// void addSectionToMemory(const std::string& name, const std::vector<uint8_t>& data, const LinkerOptions& options, std::string fileName) {
//     int address = 0;
//     if (memoryMap.find(name) != memoryMap.end()) { // If section already exists, append data
//         auto& section = memoryMap[name];
//         section.data.insert(section.data.end(), data.begin(), data.end());

//         // Check if the updated section data extends beyond its original end
//         int oldEndAddress = section.address + section.data.size() - data.size();
//         if (section.address + section.data.size() > oldEndAddress) {
//             std::vector<std::string> sectionsToUpdate;
//             for (const auto& entry : memoryMap) {
//                 if (entry.second.address >= oldEndAddress && entry.first != name) {
//                     sectionsToUpdate.push_back(entry.first);
//                 }
//             }
//             for (const auto& sectionName : sectionsToUpdate) {
//                 auto& section = memoryMap[sectionName];
//                 section.address += data.size(); //this may have to be changed, not + data.size(), maybe less???
//             }
//         }
//     } else {
//         // Check if the section has a placement specified via -place argument
//         if (options.isRelocatable == false && options.sectionPlacements.find(name) != options.sectionPlacements.end()) {
//             address = std::stoi(options.sectionPlacements.at(name), nullptr, 16); // Convert the placement address from string to int
//             //std::cout<<"Section: "<<name<<", address: "<<std::hex<<address<<std::endl;
//         } else {
//             // If no placement is specified, place it at the end of memory
//             if (memoryMap.empty()) {
//                 address = 0x0000;
//                 //address = 0x0100; // Start at 0x0100 if no sections are present (fix this)
//             } else {
//                 const auto& lastSection = memoryMap.rbegin()->second;
//                 address = lastSection.address + lastSection.data.size();
//             }
//         }
//         memoryMap[name] = {name, data, address}; // Insert the new section into memory
//         std::cout<<"name: "<<name<<", address: "<<std::hex<<address<<std::endl;
//     }
// }

void addSectionToMemory(const std::string& name, const std::vector<uint8_t>& data, const LinkerOptions& options, std::string fileName) {
    uint address = 0;

    // Process sections specified by -place first, but only if it isn't yet in memory
    if (options.sectionPlacements.find(name) != options.sectionPlacements.end() && memoryMap.find(name) == memoryMap.end()) {
        address = std::stoul(options.sectionPlacements.at(name), nullptr, 16); // Convert the placement address from string to int

        // Check for overlaps with existing placed sections
        for (const auto& entry : memoryMap) {
            const auto& section = entry.second;
            if (section.address <= address && address < section.address + section.data.size()) {
                std::cerr << "Error: Section " << name << " overlaps with section " << section.name << " at address " << std::hex << address << std::endl;
                exit(-3); //ERROR
            }
        }
    }
    else if(options.sectionPlacements.find(name) == options.sectionPlacements.end()) {
        // If no placement is specified, place it after the highest section address end
        if (memoryMap.empty() == false) {
            const auto& lastSection = std::max_element(memoryMap.begin(), memoryMap.end(),
                [](const auto& a, const auto& b) {
                    return (a.second.address + a.second.data.size()) < (b.second.address + b.second.data.size());
                })->second;
            address = lastSection.address + lastSection.data.size();
        } else {
            address = 0x0000; // Start at 0x0000 if no sections are present
        }
    }
    
    // If the section already exists, check if it overlaps with any other placed sections
    if (memoryMap.find(name) != memoryMap.end()) {
        auto& section = memoryMap[name];
        section.data.insert(section.data.end(), data.begin(), data.end());

        // Check if the updated section data extends beyond its original end
        int newEndAddress = section.address + section.data.size();
        for (const auto& entry : memoryMap) {
            if (entry.first != name && entry.second.address >= section.address && entry.second.address < newEndAddress) {
                if (options.sectionPlacements.find(entry.first) != options.sectionPlacements.end()) {
                    std::cerr << "Error: Section " << name << " overlaps with placed section " << entry.first << std::endl;
                    return;
                } else {
                    auto& overlappingSection = memoryMap[entry.first];
                    overlappingSection.address = newEndAddress; // Move the overlapping section
                }
            }
        }
    } else {
        memoryMap[name] = {name, data, address}; // Insert the new section into memory
    }
}


std::string findSection(std::string fileName, int section_index){
    for(int i = 0; i < symbolTables[fileName].size(); i++){
        if(symbolTables[fileName][i].type == "SCTN" && symbolTables[fileName][i].section_index == section_index){
            return symbolTables[fileName][i].name;
        }
    }
    return ""; //ERROR (won't happen)
}

int findInUnifiedSymbolTable(std::string name){ //returns index of that row in unified symbol table
    for(auto& entry: unifiedSymbolTable){
        if(entry.second.name == name) {
            return entry.second.num;
        }
    }
    return -1; //ERROR (won't happen)
}

void setFileSectionStartAddresses(std::vector<std::string> inputFiles, bool isRelocatable) {
    std::map<std::string, int> occurrences; // key is section name, value is how many times that section occurred
    std::map<std::string, std::map<std::string, int>> sectionSizesFiles; // key is file name, value key is section name
    std::map<std::string, std::map<std::string, int>> sectionAddressesFiles; // key is file name, value key is section name
    std::map<std::string, int> addressPlusSize;
    int cnt = 0;
    for(int i = 0; i < inputFiles.size(); i++){
        std::map<std::string, int> sectionSizes; // key is section name, value is its size
        std::map<std::string, std::vector<uint8_t>> sectionData;
        sectionData = filesSectionsMemory[inputFiles[i]];
        for (auto& entry : sectionData){ //calculated section sizes for current file
            sectionSizes[entry.first] = entry.second.size();
        }
        sectionSizesFiles[inputFiles[i]] = sectionSizes;

        for (auto& entry : sectionData){ // calculating what are offsets of the sections in each file
            auto sect = addressPlusSize.find(entry.first);
            if(sect == addressPlusSize.end()){
                addressPlusSize[entry.first] = memoryMap[entry.first].address;
                startAddressEachSection[entry.first] = memoryMap[entry.first].address; //will be used when changing rela tables
            }
            else{
                addressPlusSize[entry.first] += sectionSizesFiles[inputFiles[i-1]][entry.first];
            }
        }
        for (auto& entry : sectionData){ // assigning those offsets
            fileSectionStartAddresses[inputFiles[i]][entry.first] = addressPlusSize[entry.first];
        }
    }

    // changing the symbol offsets, addends and symbol numbers(changing to point to unifiedSymTab, won't be changed in this for loop) in relocation tables
    // note: additional offset is calculated by looking in which section does current relocation table belong
    //       additional addend for SCTNs is calculated by looking in which section does current symbol belong
    for(int i = 0; i < inputFiles.size(); i++){ // iterating through input files
        for(int j = 1; j < filesRelocationTables[inputFiles[i]].size()+1; j++){ // iterating through relocation tables inside one file
            for(int z = 0; z < filesRelocationTables[inputFiles[i]][j].size(); z++){ // iterating through entries inside relocation tables
                if(symbolTables[inputFiles[i]][filesRelocationTables[inputFiles[i]][j][z].symbol].type == "SCTN"){
                    // std::string section_name = symbolTables[inputFiles[i]][filesRelocationTables[inputFiles[i]][j][z].symbol].name;
                    // filesRelocationTables[inputFiles[i]][j][z].offset += 
                    //     fileSectionStartAddresses[inputFiles[i]][section_name] - startAddressEachSection[section_name];
                    std::string section_name = relocationTablesNames[inputFiles[i]][j-1];
                    filesRelocationTables[inputFiles[i]][j][z].offset += 
                        fileSectionStartAddresses[inputFiles[i]][section_name] - startAddressEachSection[section_name];

                    // addend will be changed also because it was LOCAL when put in relocation table and depends on symbol value
                    section_name = symbolTables[inputFiles[i]][filesRelocationTables[inputFiles[i]][j][z].symbol].name;
                    filesRelocationTables[inputFiles[i]][j][z].addend +=
                        fileSectionStartAddresses[inputFiles[i]][section_name] - startAddressEachSection[section_name];
                }
                else{ // the symbol inside relocatable table was global or extern (not using SCTN)
                    std::string section_name = relocationTablesNames[inputFiles[i]][j-1];
                    filesRelocationTables[inputFiles[i]][j][z].offset += fileSectionStartAddresses[inputFiles[i]][section_name]
                                        - startAddressEachSection[section_name];
                }
            }
        }
    }

    if(isRelocatable == false){
        //updating unifiedSymbolTable SCTNs (now sections value will have its addresses, not 0 anymore)
        for (auto& entry : unifiedSymbolTable) {
            if(entry.second.type == "SCTN"){
                // auto it = unifiedSymbolTable.find(entry.second.name);
                // if (it != unifiedSymbolTable.end()) { // If the symbol exists, update its value
                //     it->second.value = startAddressEachSection[entry.second.name];
                // }
                entry.second.value = startAddressEachSection[entry.second.name];
            }
        }
        // updating values of symbols (adding SCTN values), because when file is not relocatable SCTN values aren't 0
        for (auto& entry : unifiedSymbolTable) {
            if(entry.second.type != "SCTN" && entry.second.section_index != 0){
                for(auto& entry2 : unifiedSymbolTable){
                    if(entry2.second.type == "SCTN" && entry2.second.section_index == entry.second.section_index){
                        entry.second.value += entry2.second.value;
                        break;
                    }
                }
            }
        }
    }

    //updating rela table symbol indexes
    for(int i = 0; i < inputFiles.size(); i++){ // iterating through input files
        for(int j = 1; j < filesRelocationTables[inputFiles[i]].size()+1; j++){ // iterating through relocation tables inside one file
            for(int z = 0; z < filesRelocationTables[inputFiles[i]][j].size(); z++){ // iterating through entries inside relocation tables
                filesRelocationTables[inputFiles[i]][j][z].symbol = 
                    findInUnifiedSymbolTable(symbolTables[inputFiles[i]][filesRelocationTables[inputFiles[i]][j][z].symbol].name);
            }
        }
    }
}

void resolveRelocationTables(std::vector<std::string> inputFiles){
    for(int i = 0; i < inputFiles.size(); i++){ // i contains file index
        for(int j = 1; j < filesRelocationTables[inputFiles[i]].size()+1; j++){ // j contains relocation table index
            for(int z = 0; z < filesRelocationTables[inputFiles[i]][j].size(); z++){ //z contains index of entry inside relocation table
                RelocationEntry entry = filesRelocationTables[inputFiles[i]][j][z];
                // because it is R_X86_64_32S type of relocation, formula is S + A (S = symbol value , A = addend)
                int value = 0;
                for(auto& entry2: unifiedSymbolTable){
                    if(entry2.second.num == entry.symbol){
                        value = entry2.second.value;
                        break;
                    }
                }
                int finalValue = value + entry.addend;

                // Split the 32-bit finalValue into bytes in little-endian format
                memoryMap[relocationTablesNames[inputFiles[i]][j-1]].data[entry.offset + 0] = (finalValue >> 0) & 0xFF;
                memoryMap[relocationTablesNames[inputFiles[i]][j-1]].data[entry.offset + 1] = (finalValue >> 8) & 0xFF;
                memoryMap[relocationTablesNames[inputFiles[i]][j-1]].data[entry.offset + 2] = (finalValue >> 16) & 0xFF;
                memoryMap[relocationTablesNames[inputFiles[i]][j-1]].data[entry.offset + 3] = (finalValue >> 24) & 0xFF;
            }
        }
    }
}

void mergeSymbolTables(const std::vector<std::string>& inputFiles) {
    int SECTION_INDEX = 1;
    std::map<std::string, int> sectionSizeAdjustments;

    for (const auto& file : inputFiles) {
        const auto& symbolTable = symbolTables[file];

        for (const auto& entry : symbolTable) {
            if (entry.type == "SCTN") {
                unifiedSymbolTable.emplace(entry.name, SymbolTableEntry(entry.value, entry.type, entry.bind, SECTION_INDEX++, entry.name, entry.defined));
            } 
            else { // Adjust the value based on section size adjustments
                int adjustedValue = entry.value;
                
                std::string section_name;
                for(const auto& entry2 : symbolTable){ // find the name of the section in which symbol is
                    if(entry2.type == "SCTN" && entry2.section_index == entry.section_index){
                        section_name = entry2.name;
                        break;
                    }
                }
                if (sectionSizeAdjustments.find(section_name) != sectionSizeAdjustments.end()) {
                    adjustedValue += sectionSizeAdjustments[section_name];
                }

                // Create a new symbol table entry with the adjusted value
                SymbolTableEntry adjustedEntry = entry;
                adjustedEntry.value = adjustedValue;

                // check if there are multiple GLOB symbols with same name
                auto it = unifiedSymbolTable.find(adjustedEntry.name);
                if (it != unifiedSymbolTable.end() && adjustedEntry.bind == "GLOB" && it->second.bind == "GLOB" && 
                        (it->second.section_index != 0 && adjustedEntry.section_index != 0)){
                    std::cout<<"ERROR: Multiple global symbol definitions, symbol name: "<<adjustedEntry.name<<std::endl;
                    exit(-1); //ERROR
                }
                // Add the adjusted entry, emplace works like this: if entry with that name already exists it does nothing
                unifiedSymbolTable.emplace(adjustedEntry.name, adjustedEntry);

                // value adjusting for extern symbols
                for(const auto& sym: unifiedSymbolTable){
                    if(sym.second.name == adjustedEntry.name){
                        auto it = unifiedSymbolTable.find(sym.second.name);
                        if (adjustedEntry.section_index != 0 && it != unifiedSymbolTable.end() && sym.second.section_index == 0 && sym.second.name != "") {
                            unifiedSymbolTable.erase(it); // Erase the existing entry
                        }
                        unifiedSymbolTable.emplace(sym.second.name, adjustedEntry); // Insert the new entry
                        break;
                    }
                }
            }
        }
        // update section indexes, to point to section numbers inside unified symbol table
        for (const auto& entry : symbolTable) {
            if(entry.type != "SCTN"){
                std::string section_name = findSection(file, entry.section_index);
                for (auto& entry2 : unifiedSymbolTable){ // find index of that section inside unified sym table
                    if(entry2.second.type == "SCTN" && entry2.second.name == section_name){
                        for(auto& entry3: unifiedSymbolTable) { // find that symbol inside unified sym table and change its section index
                            if(entry3.second.name == entry.name) {
                                entry3.second.section_index = entry2.second.section_index;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }

        // Update the section size adjustments for the next file
        for (const auto& section : filesSectionsMemory[file]) {
            std::string sectionName = section.first;
            int newSize = section.second.size();
            sectionSizeAdjustments[sectionName] += newSize;
        }
    }

    // remove every symbol with LOCAL binding
    for (auto it = unifiedSymbolTable.begin(); it != unifiedSymbolTable.end(); ) {
        if (it->second.bind == "LOC" && it->second.type != "SCTN" && it->second.name != "") {
            it = unifiedSymbolTable.erase(it); // Erase and move to the next element
        } else {
            ++it; // Move to the next element
        }
    }

    //adjust numbering, to go from 0 till size()
    int i = 0;
    for (auto& entry: unifiedSymbolTable) {
        entry.second.num = i++;
    }
}


// Function to write the memory map to a hex file
void writeMemoryToHexFile(std::string fileName) {
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    std::map<uint, uint8_t> addressToData; // key is address, value is data byte
    for (const auto& pair : memoryMap) {
        const MemorySection& section = pair.second;
        int sectionEnd = section.address + section.data.size();
        for (int i = section.address; i < sectionEnd; ++i) {
            addressToData[i] = section.data[i - section.address]; // Store the byte at the address
        }
    }

    // Write to the file in the desired format
    auto it = addressToData.begin();
    while (it != addressToData.end()) {
        uint address = it->first;
        outFile << std::hex << std::setw(8) << std::setfill('0') << address << ": ";

        // Write up to 8 bytes per line, but only if addresses are consecutive
        for (int i = 0; i < 8 && it != addressToData.end(); i++) {
            if (i > 0 && std::prev(it)->first + 1 != it->first) {
                break; // Stop if the address is not consecutive
            }
            outFile << std::setw(2) << std::setfill('0') << static_cast<int>(it->second) << " ";
            ++it;
        }

        outFile << std::endl;
    }

    // Close the file
    outFile.close();
}


//------------------------------------------------------------------------------------------------
//PRINTS:
void printRelocationTablesNames() {
    for (const auto& pair : relocationTablesNames) {
        const std::string& fileName = pair.first;
        const std::vector<std::string>& sectionNames = pair.second;
        
        std::cout << "File: " << fileName << "\n";
        std::cout << "Sections:\n";
        for (const auto& sectionName : sectionNames) {
            std::cout << "  - " << sectionName << "\n";
        }
        std::cout << std::endl;
    }
}
extern "C" void printSymbolTable(std::vector<SymbolTableEntry> symbolTable) {
  // Print the headers
  std::cout << std::setfill(' ') << std::setw(5) << "NUM" << std::setw(10) << "VALUE" << std::setw(10) << "TYPE"
            << std::setw(10) << "BIND" << std::setw(15) << "SECTION INDEX" << std::setw(20) << "NAME" << std::endl;
  std::cout << std::string(85, '-') << std::endl;

  // Print each entry in the symbol table
  for (const auto& entry : symbolTable) {
      std::cout << std::setw(5) << entry.num << std::setw(10) << entry.value << std::setw(10) << entry.type
                << std::setw(10) << entry.bind << std::setw(15) << entry.section_index << std::setw(20) << entry.name << std::setw(15)
                << std::endl;
  }
}
extern "C" void printRelocationTables(std::map<int, std::vector<RelocationEntry>> relocationTables) {
  for (const auto& section : relocationTables) {
    std::cout << "\nSECTION: " << section.first << std::endl;
    std::cout << std::setw(10) << "OFFSET" 
            << std::setw(15) << "TYPE" << std::setw(20) << "SYMBOL" 
            << std::setw(10) << "ADDEND" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (const auto& entry : section.second) {
      std::cout << std::setw(10) << entry.offset
                << std::setw(15) << entry.type << std::setw(20) << entry.symbol
                << std::setw(10) << entry.addend << std::endl;
    }
  }
}
void printMemoryBySections() {
    std::cout << "Memory Layout by Sections:\n";
    for (const auto& section : memoryMap) {
        std::cout << "Section: " << section.first << "\n";
        std::cout << "Start Address: 0x" << std::hex << section.second.address << "\n";
        std::cout << "Data: ";
        
        // Print the data in the section
        for (size_t i = 0; i < section.second.data.size(); ++i) {
            // Print each byte in hexadecimal, formatted to 2 digits
            std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(section.second.data[i]) << " ";
            
            // Break line for every 16 bytes for readability
            if ((i + 1) % 16 == 0) {
                std::cout << "\n      "; // Align with "Data: " for better readability
            }
        }
        std::cout << "\n\n";
    }
}
void printUnifiedSymbolTable() {
    // Print header with proper spacing
    std::cout << std::left <<std::setfill(' ')
              << std::setw(10) << "Num"
              << std::setw(20) << "Symbol Name"
              << std::setw(10) << "Value"
              << std::setw(10) << "Type"
              << std::setw(10) << "Bind"
              << std::setw(15) << "Section Index"
              << std::setw(15) << "Defined"
              << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    // Print each symbol entry
    for (const auto& entry : unifiedSymbolTable) {
        uint val;
        if(entry.second.type == "SCTN") val = static_cast<uint>(entry.second.value);
        else val = entry.second.value;
        std::cout << std::left 
                  << std::setw(10) << entry.second.num
                  << std::setw(20) << entry.second.name
                  << std::setw(10) << std::hex << val // Convert value to string
                  << std::setw(10) << entry.second.type
                  << std::setw(10) << entry.second.bind
                  << std::setw(15) << std::to_string(entry.second.section_index) // Convert section index to string
                  << std::setw(15) << entry.second.defined
                  << std::endl;
    }
}
void printSectionStartAddresses() {
    for (const auto& fileEntry : fileSectionStartAddresses) {
        const std::string& fileName = fileEntry.first;
        const auto& sections = fileEntry.second;
        std::cout << "File: " << fileName << std::endl;
        for (const auto& sectionEntry : sections) {
            const std::string& sectionName = sectionEntry.first;
            uint address = sectionEntry.second;
            std::cout << "  Section: " << sectionName << ", Starting Address: 0x" << std::hex << address << std::dec << std::endl;
        }
    }
}
void printRelocationTablesNEW(const std::map<std::string, std::vector<RelocationEntry>>& relocationTables) {
    for (const auto& section : relocationTables) {
        std::cout << "\nSECTION: " << section.first << std::endl;
        std::cout << std::setw(10) << "OFFSET" 
                << std::setw(15) << "TYPE" << std::setw(20) << "SYMBOL" 
                << std::setw(10) << "ADDEND" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        for (const auto& entry : section.second) {
        std::cout << std::setw(10) << entry.offset
                    << std::setw(15) << entry.type << std::setw(20) << entry.symbol
                    << std::setw(10) << entry.addend << std::endl;
        }
    }
}
std::map<std::string, std::vector<RelocationEntry>> relocationTables; // key is section name, value is relocation table for that section
void mergeRelocationTables(std::vector<std::string> inputFiles){
    for(int i = 0; i < inputFiles.size(); i++){ // iterating through input files
        for(int j = 1; j < filesRelocationTables[inputFiles[i]].size()+1; j++){ // iterating through relocation tables inside one file
            relocationTables[relocationTablesNames[inputFiles[i]][j-1]]; // this creates an entry even though that relocation table for that section is empty
            //std::cout<<"file: "<<inputFiles[i]<<", rela name: "<<relocationTablesNames[inputFiles[i]][j-1]<<std::endl;
            for(int z = 0; z < filesRelocationTables[inputFiles[i]][j].size(); z++){ // iterating through entries inside relocation tables
                relocationTables[relocationTablesNames[inputFiles[i]][j-1]].push_back(filesRelocationTables[inputFiles[i]][j][z]);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------
//Everything related to creating ELF file:

std::vector<SymbolTableEntry> symbolTable;
void mapToVectorSymTab(){
    for (const auto& entry : unifiedSymbolTable) {
        symbolTable.push_back(entry.second);
    }
}


std::map<std::string, size_t> sizes;
std::string strtab;
void createElfHeader(char* elfFile) {
    Elf64_Ehdr ehdr;
    memset(&ehdr, 0, sizeof(ehdr));

    // ELF Header
    ehdr.e_ident[0] = ELFMAG0;
    ehdr.e_ident[1] = ELFMAG1;
    ehdr.e_ident[2] = ELFMAG2;
    ehdr.e_ident[3] = ELFMAG3;
    ehdr.e_ident[4] = ELFCLASS64;     // 64-bit architecture
    ehdr.e_ident[5] = ELFDATA2LSB;    // Little-endian
    ehdr.e_ident[6] = EV_CURRENT;     // ELF version
    ehdr.e_ident[7] = ELFOSABI_NONE;
    ehdr.e_type = ET_REL;             // Relocatable file
    ehdr.e_machine = EM_X86_64;       // Machine architecture
    ehdr.e_version = EV_CURRENT;      // ELF version
    ehdr.e_entry = 0;                 // Entry point (not used in relocatable files)
    ehdr.e_phoff = 0;                 // Program header table offset
    ehdr.e_shoff = sizeof(Elf64_Ehdr); // Section header table offset (after ELF header)
    ehdr.e_flags = 0;                 // Processor-specific flags
    ehdr.e_ehsize = sizeof(Elf64_Ehdr); // ELF header size
    ehdr.e_phentsize = 0;             // Size of program header entry
    ehdr.e_phnum = 0;                 // Number of program header entries
    ehdr.e_shentsize = sizeof(Elf64_Shdr); // Size of section header entry
    ehdr.e_shnum = 4 + 2*relocationTables.size(); // Number of section headers
    ehdr.e_shstrndx = 1;      // Section header string table index

    memcpy(elfFile, &ehdr, sizeof(ehdr));
}

void createSectionHeaders(char* elfFile) {
    Elf64_Shdr shdr[sizes["headerSections"] / sizeof(Elf64_Shdr)];
    memset(shdr, 0, sizeof(shdr));
    int i = 0;

    //calculating offsets and filling names for shstrtab
    std::string shstrtab;
    shstrtab.append("\0", 1);
    size_t offset_shstrtab = shstrtab.size();
    shstrtab.append(".shstrtab\0", 10);
    size_t offset_symtab = shstrtab.size();
    shstrtab.append(".symtab\0", 8);
    size_t offset_strtab = shstrtab.size();
    shstrtab.append(".strtab\0", 8);
    std::vector<size_t> offsets_sections;
    std::vector<size_t> offsets_relocations;
    for(int j = 0; j < symbolTable.size(); j++){
      if(symbolTable[j].type  == "SCTN"){
        offsets_sections.push_back(shstrtab.size());
        shstrtab.append(symbolTable[j].name.c_str(), symbolTable[j].name.size() + 1);

        offsets_relocations.push_back(shstrtab.size());
        shstrtab.append(".rela",5);
        shstrtab.append(symbolTable[j].name.c_str(), symbolTable[j].name.size() + 1);
      }
    }
    
    //inserting 0th section
    shdr[i].sh_name = 0;
    shdr[i].sh_type = SHT_NULL;
    shdr[i].sh_flags = 0;
    shdr[i].sh_addr = 0;
    shdr[i].sh_offset = 0;
    shdr[i].sh_size = 0;
    shdr[i].sh_link = 0;
    shdr[i].sh_info = 0;
    shdr[i].sh_addralign = 0;
    shdr[i++].sh_entsize = 0;
  
    //inserting shstrtab
    shdr[i].sh_name = offset_shstrtab;
    shdr[i].sh_type = SHT_STRTAB;
    shdr[i].sh_flags = 0;
    shdr[i].sh_addr = 0;
    shdr[i].sh_offset = sizeof(Elf64_Ehdr) + sizes["headerSections"];
    shdr[i].sh_size = shstrtab.size();
    shdr[i].sh_link = 0;
    shdr[i].sh_info = 0;
    shdr[i].sh_addralign = 0;
    shdr[i++].sh_entsize = 0;

    //inserting sections
    std::vector<std::string> order; // keeps track what section are first inserted, it may happen that order of sections inside
                                    // relocationTable isn't the same as the one that is in shstrtab.
    size_t offset1 = sizes["shstrtab"];
    int counter = 0;
    for(int j = 0; j < symbolTable.size(); j++){
      if(symbolTable[j].type  == "SCTN"){
        shdr[i].sh_name = offsets_sections[counter++];
        shdr[i].sh_type = SHT_PROGBITS;
        shdr[i].sh_flags = 0;
        shdr[i].sh_addr = 0;
        shdr[i].sh_offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + offset1;
        shdr[i].sh_size = memoryMap[symbolTable[j].name].data.size();
        order.push_back(symbolTable[j].name);
        offset1 += shdr[i].sh_size; // adding size of added section to the offset
        shdr[i].sh_link = 0;
        shdr[i].sh_info = 0;
        shdr[i].sh_addralign = 0;
        shdr[i++].sh_entsize = 0;
      }
    }
    //inserting relocation sections
    size_t offset2 = 0;
    for(int j = 0; j < relocationTables.size(); j++){
        shdr[i].sh_name = offsets_relocations[j];
        shdr[i].sh_type = SHT_RELA;
        shdr[i].sh_flags = 0;
        shdr[i].sh_addr = 0;
        shdr[i].sh_offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + offset1 + offset2;
        // std::string section_name; //have to find name of the section because map is <std::string, RelaEntry>
        // for(int z = 0; z < symbolTable.size(); z++){
        //     if(symbolTable[z].type == "SCTN" && symbolTable[z].section_index == j+1){
        //         section_name = symbolTable[z].name;
        //     }
        // }
        // shdr[i].sh_size = relocationTables[section_name].size() * sizeof(Elf64_Rela); // Size of relocation table
        shdr[i].sh_size = relocationTables[order[j]].size() * sizeof(Elf64_Rela); // Size of relocation table
        offset2 += shdr[i].sh_size;
        shdr[i].sh_link = i + relocationTables.size() - j; //connection with symbol table
        shdr[i].sh_info = i - relocationTables.size();
        shdr[i].sh_addralign = 8;
        shdr[i++].sh_entsize = sizeof(Elf64_Rela);
    }
    
    // Section Header for .symtab
    shdr[i].sh_name = offset_symtab;  // Index to section header string table (assuming it's at offset 0)
    shdr[i].sh_type = SHT_SYMTAB;
    shdr[i].sh_flags = 0;
    shdr[i].sh_addr = 0;
    shdr[i].sh_offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + offset1 + offset2;
    shdr[i].sh_size = symbolTable.size() * sizeof(Elf64_Sym); // Size of symbol table
    shdr[i].sh_link = i+1;  // Index of section header string table
    shdr[i].sh_info = symbolTable.size();
    shdr[i].sh_addralign = 8;
    shdr[i++].sh_entsize = sizeof(Elf64_Sym);
    
    // Section Header for .strtab
    shdr[i].sh_name = offset_strtab;
    shdr[i].sh_type = SHT_STRTAB;
    shdr[i].sh_flags = 0;
    shdr[i].sh_addr = 0;
    shdr[i].sh_offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + offset1 + offset2 + shdr[i-1].sh_size;
    shdr[i].sh_size = strtab.size();  // all the names length from symbol table added together
    shdr[i].sh_link = 0;
    shdr[i].sh_info = 0;
    shdr[i].sh_addralign = 0;
    shdr[i++].sh_entsize = 0;
    
    memcpy(elfFile + sizeof(Elf64_Ehdr), shdr, sizeof(shdr));
    memcpy(elfFile + sizeof(Elf64_Ehdr) + sizes["headerSections"], shstrtab.c_str(), shstrtab.size());
    memcpy(elfFile + sizeof(Elf64_Ehdr) + sizes["headerSections"] + sizes["sectionData"] + sizes["shstrtab"] 
            + sizes["relocationTable"] + sizes["symbolTable"], strtab.c_str(), strtab.size());
}

int getBind(SymbolTableEntry entry){
    if(entry.bind == "LOC") return STB_LOCAL;
    else if(entry.bind == "GLOB") return STB_GLOBAL;
    return -1; //ERROR
}

int getType(SymbolTableEntry entry){
    if(entry.type == "NOTYP") return STT_NOTYPE;
    else if(entry.type == "SCTN") return STT_SECTION; 
    return -1; //ERROR
}

void serializeSectionData(char* elfFile){
    size_t offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + sizes["shstrtab"];
    size_t sectionOffset = offset;
    for(auto& entry: memoryMap){
        const std::vector<uint8_t>& sectionData = entry.second.data;
        memcpy(elfFile + sectionOffset, sectionData.data(), sectionData.size());
        sectionOffset += entry.second.data.size();
    }
}

void serializeRelocationTable(char* elfFile) {
    size_t offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + sizes["shstrtab"] + sizes["sectionData"];

    // Calculate total size needed for all relocation tables
    size_t totalSize = 0;
    std::vector<size_t> tableOffsets;
    for (auto& entry: relocationTables) {
        tableOffsets.push_back(totalSize);
        totalSize += entry.second.size() * sizeof(Elf64_Rela);
    }

    // Write all relocation tables to ELF file
    int i = 0;
    for (auto& entry: relocationTables) {
        const std::vector<RelocationEntry>& entries = entry.second;
        Elf64_Rela* rela = reinterpret_cast<Elf64_Rela*>(elfFile + offset + tableOffsets[i]);

        for (size_t j = 0; j < entries.size(); j++) {
            const RelocationEntry& entry2 = entries[j];
            rela[j].r_offset = entry2.offset;
            rela[j].r_info = ELF64_R_INFO(entry2.symbol, R_X86_64_32S);
            rela[j].r_addend = entry2.addend;
        }
        i++;
    }
}

void serializeSymbolTable(char* elfFile, std::vector<int> strtab_offsets) {
    size_t offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + sizes["shstrtab"] + sizes["sectionData"] + sizes["relocationTable"];
    Elf64_Sym* symtab = reinterpret_cast<Elf64_Sym*>(elfFile + offset);

    for (size_t i = 0; i < symbolTable.size(); ++i) {
        const SymbolTableEntry& entry = symbolTable[i];
        symtab[i].st_name = strtab_offsets[i]; // Index into string table
        symtab[i].st_info = ELF64_ST_INFO(getBind(entry), getType(entry));
        symtab[i].st_other = STV_DEFAULT;
        symtab[i].st_shndx = entry.section_index;
        symtab[i].st_value = entry.value;
        symtab[i].st_size = 0; // Size of the symbol (set to 0 for now, linker will set it later)
    }
}

extern "C" void createELF(std::vector<std::string> inputFiles, std::string outputFile) {
    sizes["symbolTable"] = symbolTable.size() * sizeof(Elf64_Sym);
    sizes["headerSections"] = (4 + 2 * relocationTables.size()) * sizeof(Elf64_Shdr);

    size_t relocationTablesSize = 0;
    for (auto& entry : relocationTables) {
        relocationTablesSize += entry.second.size() * sizeof(Elf64_Rela);
    }
    sizes["relocationTable"] = relocationTablesSize;

    std::vector<int> strtab_offsets;
    strtab.append("\0",1);
    for(int j = 0; j < symbolTable.size(); j++){
        strtab_offsets.push_back(strtab.size());
        strtab.append(symbolTable[j].name.c_str(), symbolTable[j].name.size() +1);
    }
    sizes["strtab"] = strtab.size();

    size_t sectionDatasize = 0;
    for (auto& entry : memoryMap) {
        sectionDatasize += entry.second.data.size();
    }
    sizes["sectionData"] = sectionDatasize;

    std::string shstrtab;
    shstrtab.append("\0", 1);
    shstrtab.append(".shstrtab\0", 10);
    shstrtab.append(".symtab\0", 8);
    shstrtab.append(".strtab\0", 8);
    for(int j = 0; j < symbolTable.size(); j++){
        if(symbolTable[j].type  == "SCTN"){
            shstrtab.append(symbolTable[j].name.c_str(), symbolTable[j].name.size() + 1);
            shstrtab.append(".rela",5);
            shstrtab.append(symbolTable[j].name.c_str(), symbolTable[j].name.size() + 1);
        }
    }
    sizes["shstrtab"] = shstrtab.size();

    size_t fileSize = sizeof(Elf64_Ehdr) + 
                    sizes["headerSections"] +
                    sizes["sectionData"] +
                    sizes["shstrtab"] +
                    sizes["relocationTable"] +
                    sizes["symbolTable"] +
                    sizes["strtab"];

    char* elfFile = new char[fileSize];
    memset(elfFile, 0, fileSize);

    createElfHeader(elfFile);
    std::cout<<"createElfHeader"<<std::endl;
    createSectionHeaders(elfFile);
    std::cout<<"createSectionHeaders"<<std::endl;
    serializeSectionData(elfFile);
    std::cout<<"serializeSectionData"<<std::endl;
    serializeRelocationTable(elfFile);
    std::cout<<"serializeRelocationTable"<<std::endl;
    serializeSymbolTable(elfFile, strtab_offsets);
    std::cout<<"serializeSymbolTable"<<std::endl;

    // Write to file
    FILE* file = fopen(outputFile.c_str(), "wb");
    if (file) {
        fwrite(elfFile, 1, fileSize, file);
        fclose(file);
    }

    delete[] elfFile;
}

int main(int argc, char* argv[]) {
    LinkerOptions options = parseArguments(argc, argv);

    // Now you can use the options structure to control the rest of your linker logic
    // std::cout << "Output file: " << options.outputFile << "\n";
    // for (const auto& placement : options.sectionPlacements) {
    //     std::cout << "Place section " << placement.first << " at address " << placement.second << "\n";
    // }
    // std::cout << (options.isHex ? "Hex output enabled\n" : "Relocatable output enabled\n");
    // for (const auto& inputFile : options.inputFiles) {
    //     std::cout << "Input file: " << inputFile << "\n";
    // }

    // Read the ELF file and populate the symbol table and relocation tables
    readELF(options.inputFiles);

    if(options.isRelocatable == false){
        // First, add every section with a -place argument to memory
        for (const auto& file : options.inputFiles) {
            const auto& sections = filesSectionsMemory[file];
            for (const auto& section : sections) {
                if (options.sectionPlacements.find(section.first) != options.sectionPlacements.end()) {
                    addSectionToMemory(section.first, section.second, options, file);
                }
            }
        }
        // Then, add every other section to memory
        for (const auto& file : options.inputFiles) {
            const auto& sections = filesSectionsMemory[file];
            for (const auto& section : sections) {
                if (options.sectionPlacements.find(section.first) == options.sectionPlacements.end()) {
                    addSectionToMemory(section.first, section.second, options, file);
                }
            }
        }
    }
    else{
        options.sectionPlacements.clear(); //-place parameter should be ignored
        for (const auto& file : options.inputFiles) {
            const auto& sections = filesSectionsMemory[file];
            for (const auto& section : sections) {
                addSectionToMemory(section.first, section.second, options, file);
            }
        }
    }

    mergeSymbolTables(options.inputFiles);
    setFileSectionStartAddresses(options.inputFiles, options.isRelocatable);

    //printing symbol and relocation tables
    // for(int i = 0; i < options.inputFiles.size(); i++){
    //     std::cout<<"FILE: "<<options.inputFiles[i]<<std::endl;
    //     printSymbolTable(symbolTables[options.inputFiles[i]]);
    //     printRelocationTables(filesRelocationTables[options.inputFiles[i]]);
    // }
    
    //printMemoryBySections();
    //printSectionStartAddresses();

    //printUnifiedSymbolTable();

    if(options.isRelocatable == true){
        mergeRelocationTables(options.inputFiles);
        //printRelocationTablesNEW(relocationTables);
        mapToVectorSymTab(); //changing unified symbol table from std::map to std::vector for easier work while creating ELF
        createELF(options.inputFiles, options.outputFile);
    }
    else if(options.isHex == true){
        resolveRelocationTables(options.inputFiles);
        writeMemoryToHexFile(options.outputFile);
    }

    return 0;
}