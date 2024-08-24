//#include "definitions.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <iomanip>
#include <elf.h>
#include <algorithm>

std::vector<std::string> symbol_list;

extern "C" void addToSymbolList(char* symbol) {
  symbol_list.push_back(std::string(symbol));
}

extern "C" void printSymbolList() {
  for (size_t i = 0; i < symbol_list.size(); ++i) {
    printf("%s", symbol_list[i].c_str());
    if (i < symbol_list.size() - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }
}

extern "C" void clearSymbolList() {
  symbol_list.clear();
}


enum WordType {
  SYMBOL_TYPE,
  LITERAL_TYPE
};

// Define a struct with a union to store symbols and literals
struct Word {
  WordType type;
  union {
    char* symbol;
    int literal;
  } value;
};

std::vector<Word> word_list;

extern "C" void addSymbolToWordList(char* symbol) {
  Word word;
  word.type = SYMBOL_TYPE;
  word.value.symbol = symbol;
  word_list.push_back(word);
}

extern "C" void addLiteralToWordList(int literal) {
  Word word;
  word.type = LITERAL_TYPE;
  word.value.literal = literal;
  word_list.push_back(word);
}

extern "C" void printWordList() {
  for (size_t i = 0; i < word_list.size(); ++i) {
    if (word_list[i].type == SYMBOL_TYPE) {
      printf("%s", word_list[i].value.symbol);
    } else {
      printf("%d", word_list[i].value.literal);
    }
    if (i < word_list.size() - 1) {
      printf(", ");
    } else {
      printf("\n");
    }
  }
}

extern "C" void clearWordList() {
  word_list.clear();
}

//-------------------------------------------------------------------
//Flink table and relocations
struct AddressSignAddend {
  int address;
  char sign;
  int addend;

  AddressSignAddend(int addr, char sgn, int add) : address(addr), sign(sgn), addend(add) {}
};

struct FlinkTableEntry {
  std::string symbol;
  int symbol_value = 0;
  std::vector<AddressSignAddend> address_sign_addend;

  FlinkTableEntry(const std::string& sym): symbol(sym) {}
};
std::map<int, std::vector<FlinkTableEntry>> flinkTableMap;

void addFlinkTableEntry(int section, const char* symbol, int symbol_value, int address, char sign, int addend) {
  FlinkTableEntry* entry = nullptr;
  for (auto& e : flinkTableMap[section]) {
    if (e.symbol == symbol) {
      entry = &e;
      break;
    }
  }

  if (entry == nullptr) {
    FlinkTableEntry newEntry(symbol);
    newEntry.address_sign_addend.emplace_back(address, sign, addend);
    flinkTableMap[section].push_back(newEntry);
  } else {
    entry->address_sign_addend.emplace_back(address, sign, addend); //emplace_back does the same as push_back but is more efficient.
  }
}

void changeValFlinkTableEntry(int section, const char* symbol, int symbol_value) { //change the symbol value
  if (flinkTableMap.count(section)) {
    auto& entries = flinkTableMap[section];
    for (auto& entry : entries) {
      if (entry.symbol == symbol) {
        entry.symbol_value = symbol_value;
        return;
      }
    }
  }
}

extern "C" void printFlinkTable() {
    std::cout << std::setw(10) << "SECTION" << std::setw(20) << "SYMBOL" 
              << std::setw(15) << "SYMBOL_VALUE" << std::setw(10) << "ADDRESS" 
              << std::setw(10) << "SIGN" <<  std::setw(10) << "ADDEND" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    // Print each entry in the flink table
    for (const auto& section : flinkTableMap) {
        bool firstEntryInSection = true;
        for (const auto& entry : section.second) {
            for (const auto& pair : entry.address_sign_addend) {
                if (firstEntryInSection) {
                    std::cout << std::setw(10) << section.first;
                    firstEntryInSection = false;
                } else {
                    std::cout << std::setw(10) << "";
                }
                std::cout << std::setw(20) << entry.symbol << std::setw(15) << entry.symbol_value
                          << std::setw(10) << pair.address << std::setw(10) << pair.sign << std::setw(10) << pair.addend << std::endl;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------
std::vector<uint8_t> memory;
std::map <int, std::vector<uint8_t>> memoryMap; //key is section number, and value is memory for that section
std::map<int, int> sectionLocationCounter;

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
int SECTION_INDEX = 1;      // when new section arrives (doesnt already exist), initialize it with this number.
int CURR_SECTION_INDEX = 0; //information in which section are we currently in.
std::vector<SymbolTableEntry> symbolTable;
bool symbolTableInitialized = false;

// Function to initialize the symbol table with the initial entry
void initializeSymbolTable() {
  if (symbolTableInitialized == false) {
    SymbolTableEntry initialEntry(0, "NOTYP", "LOC", 0, "", "defined");
    symbolTable.push_back(initialEntry);
    symbolTableInitialized = true;
  }
}

void addSymbolToSymTable(int value, std::string type, std::string bind, int section_index, std::string name, std::string defined) {
  initializeSymbolTable();
  SymbolTableEntry entry(value, type, bind, section_index, name, defined);
  symbolTable.push_back(entry);
}

extern "C" void printSymbolTable() {
  // Print the headers
  std::cout << std::setw(5) << "NUM" << std::setw(10) << "VALUE" << std::setw(10) << "TYPE"
            << std::setw(10) << "BIND" << std::setw(15) << "SECTION INDEX" << std::setw(20) << "NAME" << std::setw(15) << "DEFINED" << std::endl;
  std::cout << std::string(85, '-') << std::endl;

  // Print each entry in the symbol table
  for (const auto& entry : symbolTable) {
      std::cout << std::setw(5) << entry.num << std::setw(10) << entry.value << std::setw(10) << entry.type
                << std::setw(10) << entry.bind << std::setw(15) << entry.section_index << std::setw(20) << entry.name << std::setw(15) << entry.defined
                << std::endl;
  }
}

SymbolTableEntry* symbolExist(std::string name){
  for(int i = 0; i < symbolTable.size(); i++){
    if(symbolTable[i].name == name) return &symbolTable[i];
  }
  return nullptr;
}

int getSymbolNum(std::string symbol) {
  for(int i = 0; i < symbolTable.size(); i++){
    if(symbolTable[i].name == symbol){
      if(symbolTable[i].bind == "LOC") {
        //we have section index now we have to find what is the num of that section
        for(int j = 0; j < symbolTable.size(); j++){
          if(symbolTable[j].type == "SCTN" && symbolTable[j].section_index == symbolTable[i].section_index){
            return symbolTable[j].num;
          }
        }
      }
      else return symbolTable[i].num; 
    }
  }
  return -2; //ERROR CODE (wont happen)
}

int formAddend(std::string symbol, int flink_addend, int offset){
  for(int i = 0; i < symbolTable.size(); i++){
    if(symbolTable[i].name == symbol){
      if(symbolTable[i].bind == "LOC") {
        return symbolTable[i].value; //+ flink_addend - offset;
      }
      else {
        return 0;//flink_addend - offset;
      }
    }
  }
  return -1; //ERROR code (wont happen)
}

bool isDefined(std::string symbol){ // if symbol exists in symbol table and is defined it will return true
  SymbolTableEntry* entry = symbolExist(symbol);
  if(entry == nullptr) return false;
  else{
    if(entry->defined == "defined") return true;
    else return false;
  }
}

extern "C" void printMemoryMap() {
  std::cout << "Memory Map:" << std::endl;
  for (auto it = memoryMap.begin(); it != memoryMap.end(); ++it) {
    int address = it->first;
    const std::vector<uint8_t>& data = it->second;
    std::cout << "Address: 0x" << std::hex << std::setw(4) << std::setfill('0') << address << std::dec << std::endl;
    std::cout << "Data: ";
    for (size_t i = 0; i < data.size(); ++i) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << ' ';
      if ((i + 1) % 16 == 0) {
          std::cout << std::endl << "      ";
      }
    }
    std::cout << std::dec << std::endl;
  }
}

void insert_to_memory(uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t addr4, uint8_t addr5, uint8_t addr6, uint8_t addr7, uint8_t addr8){
  uint8_t combined1 = (addr1 << 4) | (addr2 & 0x0F);
  uint8_t combined2 = (addr3 << 4) | (addr4 & 0x0F);
  uint8_t combined3 = (addr5 << 4) | (addr6 & 0x0F);
  uint8_t combined4 = (addr7 << 4) | (addr8 & 0x0F);
  
  memoryMap[CURR_SECTION_INDEX].push_back(combined4);
  memoryMap[CURR_SECTION_INDEX].push_back(combined3);
  memoryMap[CURR_SECTION_INDEX].push_back(combined2);
  memoryMap[CURR_SECTION_INDEX].push_back(combined1);

  sectionLocationCounter[CURR_SECTION_INDEX] += 4; // Update the location counter for the current section
}

void splitAndInsertLiteralToMem(int literal) {
  uint8_t parts[8];
  
  // Split the integer into 8 parts of 4 bits each
  for (int i = 0; i < 8; ++i) {
    parts[i] = (literal >> (28 - 4 * i)) & 0xF; // Extract 4 bits
  }
  insert_to_memory(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], parts[7]);
}

//--------------------------------------------------------------------------------------------------
struct RelocationEntry {
  int offset; // Offset within the section
  std::string type = "R_X86_64_32S"; // Type of relocation
  int symbol; // Symbol index in symbol table
  int addend; // Addend value

  RelocationEntry(int off, int sym, int add)
    : offset(off), symbol(sym), addend(add) {}
};
std::map<int, std::vector<RelocationEntry>> relocationTables; // key is section number

void addToRelocationTable(int offset, int section, std::string symbol, int addend){
  int symbol_num = getSymbolNum(symbol);
  if(offset == -1) offset = sectionLocationCounter[CURR_SECTION_INDEX];
  if(section == -1) section = CURR_SECTION_INDEX;
  
  RelocationEntry rentry(offset, symbol_num, formAddend(symbol, addend, offset));
  relocationTables[section].push_back(rentry);
}

extern "C" void printRelocationTables() {
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

//--------------------------------------------------------------------------------------------------
//instructions
extern "C" void instruction_add_sub_mul_div_xchg(char* instr, int gprS, int gprD) {
  if (strcmp(instr, "add") == 0) {
    insert_to_memory(5,0,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "sub") == 0) {
    insert_to_memory(5,1,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "mul") == 0) {
    insert_to_memory(5,2,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "div") == 0) {
    insert_to_memory(5,3,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "xchg") == 0){
    insert_to_memory(4,0,0,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  }
}

extern "C" void instruction_and_or_xor_shl_shr(char* instr, int gprS, int gprD){
  if (strcmp(instr, "and") == 0) {
    insert_to_memory(6,1,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "or") == 0) {
    insert_to_memory(6,2,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "xor") == 0) {
    insert_to_memory(6,3,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "shl") == 0) {
    insert_to_memory(7,0,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  } else if (strcmp(instr, "shr") == 0) {
    insert_to_memory(7,1,static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprD),static_cast<uint8_t>(gprS),0,0,0);
  }
}

extern "C" void instruction_not_push_pop(char* instr, int gpr){
  if (strcmp(instr, "not") == 0) {
    insert_to_memory(6,0,static_cast<uint8_t>(gpr),static_cast<uint8_t>(gpr),0,0,0,0);
  } else if (strcmp(instr, "push") == 0) {
    insert_to_memory(8,1,14,0,static_cast<uint8_t>(gpr),15,15,12); //sp = sp - 4, mem32[sp] = gpr, FFC(15,15,12) predstavlja -4
  } else if (strcmp(instr, "pop") == 0) {
    insert_to_memory(9,3,static_cast<uint8_t>(gpr),14,0,0,0,4); //gpr <= mem32[sp]; sp <= sp + 4; 
  }
}

extern "C" void instruction_halt_int_iret_ret(char* instr){
  if (strcmp(instr, "halt") == 0) {
    insert_to_memory(0,0,0,0,0,0,0,0);
  } else if (strcmp(instr, "int") == 0) {
    insert_to_memory(1,0,0,0,0,0,0,0);
  } else if (strcmp(instr, "iret") == 0) {
    //NE MOZE OVAKO JER KAD SE POPUJE PC, ONDA SE SAMO SKACE NA TU ADRESU A NE IZVRSAVA SE POP STATUS
    //insert_to_memory(9,3,15,14,0,0,0,4); //pc <= mem32[sp]; sp <= sp + 4;
    //insert_to_memory(9,7,0,14,0,0,0,4);  //status <= mem32[sp]; sp <= sp + 4;
    //OVAKO TREBA
    insert_to_memory(9,1,14,14,0,0,0,8); // sp = sp + 8
    insert_to_memory(9,6,0,14,0,15,15,12); // status = mem32[sp-4]
    insert_to_memory(9,2,15,14,0,15,15,8); // pc = mem32[sp - 8]
  } else if (strcmp(instr, "ret") == 0) {
    insert_to_memory(9,3,15,14,0,0,0,4); //pc <= mem32[sp]; sp <= sp + 4;
  }
}

extern "C" void instruction_beq_bne_bgt(char* instr, int gpr1, int gpr2, char* what_op, int literal, char* symbol){
  if (strcmp(instr, "beq") == 0) {
    if(strcmp(what_op, "symbol") == 0){
      insert_to_memory(3,9,15,static_cast<uint8_t>(gpr1),static_cast<uint8_t>(gpr2),0,0,4); //BEQ GPR1, GPR2, [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); //JMP pc+4
      if(isDefined(std::string(symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 4);
      }
      else{
        addToRelocationTable(-1, -1, std::string(symbol), 4);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "literal") == 0){
      insert_to_memory(3,9,15,static_cast<uint8_t>(gpr1),static_cast<uint8_t>(gpr2),0,0,4); //BEQ GPR1, GPR2, [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); //JMP pc+4
      splitAndInsertLiteralToMem(literal); //.neki literal na 32bita
    }
  } else if (strcmp(instr, "bne") == 0) {
    if(strcmp(what_op, "symbol") == 0){
      insert_to_memory(3,10,15,static_cast<uint8_t>(gpr1),static_cast<uint8_t>(gpr2),0,0,4); //BNE GPR1, GPR2, [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); //JMP pc+4
      if(isDefined(std::string(symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 4);
      }
      else{
        addToRelocationTable(-1, -1, std::string(symbol), 4);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "literal") == 0){
      insert_to_memory(3,10,15,static_cast<uint8_t>(gpr1),static_cast<uint8_t>(gpr2),0,0,4); //BNE GPR1, GPR2, [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); //JMP pc+4
      splitAndInsertLiteralToMem(literal); //.neki literal na 32bita
    }
  } else if (strcmp(instr, "bgt") == 0) {
    if(strcmp(what_op, "symbol") == 0){
      insert_to_memory(3,11,15,static_cast<uint8_t>(gpr1),static_cast<uint8_t>(gpr2),0,0,4); //BGT GPR1, GPR2, [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); //JMP pc+4
      if(isDefined(std::string(symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 4);
      }
      else{
        addToRelocationTable(-1, -1, std::string(symbol), 4);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "literal") == 0){
      insert_to_memory(3,11,15,static_cast<uint8_t>(gpr1),static_cast<uint8_t>(gpr2),0,0,4); //BGT GPR1, GPR2, [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); //JMP pc+4
      splitAndInsertLiteralToMem(literal); //.neki literal na 32bita
    }
  }
}

extern "C" void instruction_call_jmp(char* instr, char* what_op, int literal, char* symbol){
  if (strcmp(instr, "call") == 0) {
    if(strcmp(what_op, "symbol") == 0){
      SymbolTableEntry* entry = symbolExist(std::string(symbol));
      insert_to_memory(2,1,15,0,0,0,0,4); // CALL [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4
      if(isDefined(std::string(symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 4);
      }
      else{
        addToRelocationTable(-1, -1, std::string(symbol), 4);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "literal") == 0){
      // it must be done that way with literal pool in order to have 32bit addend.
      insert_to_memory(2,1,15,0,0,0,0,4); // CALL [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4
      splitAndInsertLiteralToMem(literal);// .neki literal na 32bita
    }
  } else if (strcmp(instr, "jmp") == 0) {
    if(strcmp(what_op, "symbol") == 0){
      insert_to_memory(3,8,15,0,0,0,0,0); // JMP [pc]
      SymbolTableEntry* entry = symbolExist(std::string(symbol));
      if(isDefined(std::string(symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 0);
      }
      else{
        addToRelocationTable(-1, -1, std::string(symbol), 0);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "literal") == 0){
      insert_to_memory(3,8,15,0,0,0,0,0); // JMP [pc]
      splitAndInsertLiteralToMem(literal);// .neki literal na 32bita
    }
  }
}

extern "C" void instruction_csrrd_csrrw(char* instr, int csr, int gpr){
  if (strcmp(instr, "csrrd") == 0) {
    insert_to_memory(9,0,static_cast<uint8_t>(gpr),static_cast<uint8_t>(csr),0,0,0,0);
  } else if (strcmp(instr, "csrrw") == 0) {
    insert_to_memory(9,4,static_cast<uint8_t>(csr),static_cast<uint8_t>(gpr),0,0,0,0);
  }
}

extern "C" void instruction_ld_st(char* instr, char* what_op, int gpr, int literal, char* symbol, int dollar_literal, char* dollar_symbol,
                                  int reg, int mem_reg, int mem_reg_literal, int mem_reg_symbol){
  if (strcmp(instr, "ld") == 0) {
    if(strcmp(what_op, "symbol") == 0){
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),15,0,0,0,8); // gpr[A] <= mem32[pc+8]
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),static_cast<uint8_t>(gpr),0,0,0,0); // gpr[A] = mem[gprA]
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4
      if(isDefined(std::string(symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 0);
      }
      else{
        addToRelocationTable(-1, -1, std::string(symbol), 0);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "literal") == 0){
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),15,0,0,0,8); // gpr = mem[pc+8]
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),static_cast<uint8_t>(gpr),0,0,0,0); // LD [gpr]
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4
      splitAndInsertLiteralToMem(literal); // .neki literal na 32bita
    }
    else if(strcmp(what_op, "dollar_literal") == 0){
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),15,0,0,0,4); // LD [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4

      //convert int to hex string
      // std::stringstream ss;
      // ss << std::hex << dollar_literal;
      // std::string hexString = ss.str();
      // // Convert the hexadecimal string back to an integer
      // int hexValue;
      // std::stringstream ss2;
      // ss2 << std::hex << hexString;
      // ss2 >> hexValue;
      // dollar_literal = hexValue;
      splitAndInsertLiteralToMem(dollar_literal); // .neki literal na 32bita
    }
    else if(strcmp(what_op, "dollar_symbol") == 0){
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),15,0,0,0,4); // gprx <= [pc+4]
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4
      if(isDefined(std::string(dollar_symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, dollar_symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 0);
      }
      else{
        addToRelocationTable(-1, -1, std::string(dollar_symbol), 0);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "reg") == 0){
      insert_to_memory(9,1,static_cast<uint8_t>(gpr),static_cast<uint8_t>(reg),0,0,0,0);
    }
    else if(strcmp(what_op, "mem_reg") == 0){
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),static_cast<uint8_t>(reg),0,0,0,0); //gprx = mem[reg]
    }
    else if(strcmp(what_op, "mem_reg_literal") == 0){
      if ((literal & 0xFFFFF000) != 0){} //ERROR
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),static_cast<uint8_t>(reg),0,static_cast<uint8_t>((literal >> 8) & 0x0F),
        static_cast<uint8_t>((literal >> 4) & 0x0F),static_cast<uint8_t>(literal & 0x0F)); //gprx = mem[reg + literal]
    }
    else if(strcmp(what_op, "mem_reg_symbol") == 0){
      if(isDefined(std::string(symbol)) == false){} //ERROR
      insert_to_memory(9,2,static_cast<uint8_t>(gpr),static_cast<uint8_t>(reg),0,0,0,0); //gprx = mem[reg + symbol], symbol has value 0 right now...
    }
  } else if (strcmp(instr, "st") == 0) {
    if(strcmp(what_op, "symbol") == 0){
      insert_to_memory(8,2,15,0,static_cast<uint8_t>(gpr),0,0,4); // mem[mem[pc+4]] = gprx
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4
      if(isDefined(std::string(symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 0);
      }
      else{
        addToRelocationTable(-1, -1, std::string(symbol), 0);
      }
      splitAndInsertLiteralToMem(0); // .neki symbol na 32bita, dodeljene sve 0le posto ne znamo vrednost simbola
    }
    else if(strcmp(what_op, "literal") == 0){
      insert_to_memory(8,2,15,0,static_cast<uint8_t>(gpr),0,0,4); // mem[mem[pc+4]] = gprx
      insert_to_memory(3,0,15,0,0,0,0,4); // JMP pc+4
      splitAndInsertLiteralToMem(literal); // .neki literal na 32bita
    }
    else if(strcmp(what_op, "dollar_literal") == 0){
      //ERROR
    }
    else if(strcmp(what_op, "dollar_symbol") == 0){
      //ERROR
    }
    else if(strcmp(what_op, "reg") == 0){
      insert_to_memory(9,1,static_cast<uint8_t>(reg),static_cast<uint8_t>(gpr),0,0,0,0);
    }
    else if(strcmp(what_op, "mem_reg") == 0){
      insert_to_memory(8,0,static_cast<uint8_t>(reg),0,static_cast<uint8_t>(gpr),0,0,0); //mem[reg] = gprx
    }
    else if(strcmp(what_op, "mem_reg_literal") == 0){
      if ((literal & 0xFFFFF000) != 0){} //ERROR
      insert_to_memory(8,0,static_cast<uint8_t>(reg),0,static_cast<uint8_t>(gpr),static_cast<uint8_t>((literal >> 8) & 0x0F),
                        static_cast<uint8_t>((literal >> 4) & 0x0F),static_cast<uint8_t>(literal & 0x0F)); //mem[reg + literal] = gprx
    }
    else if(strcmp(what_op, "mem_reg_symbol") == 0){
      if(isDefined(std::string(symbol)) == false){} //ERROR
      insert_to_memory(8,0,static_cast<uint8_t>(reg),0,static_cast<uint8_t>(gpr),0,0,0); //mem[reg + symbol] = gprx
    }
  }
}

//-------------------------------------------------------------------------------------------
extern "C" void directive_global(){
  for(int i = 0; i < symbol_list.size(); i++){
    SymbolTableEntry* entry = symbolExist(symbol_list[i]);
    if(entry == nullptr){
      addSymbolToSymTable(0, "NOTYP", "GLOB", 0, symbol_list[i], "undefined"); //value and section_index will be inserted when lable with its name is found.
    }
    else{
      entry->bind = "GLOB"; //nisam siguran dal ovo ovako treba, pogledati jos sta se desava ako prvo bude labela pa tek kasnije naidje global
    }
  }
}

extern "C" void directive_extern(){
  for(int i = 0; i < symbol_list.size(); i++){
    SymbolTableEntry* entry = symbolExist(symbol_list[i]);
    if(entry == nullptr){
      addSymbolToSymTable(0, "NOTYP", "GLOB", 0, symbol_list[i], "undefined");
    }
    else{
      entry->bind = "GLOB"; //nisam siguran dal ovo ovako treba, pogledati jos sta se desava ako prvo bude labela pa tek kasnije naidje extern
    }
  }
}

extern "C" void directive_section(char* name){
  SymbolTableEntry* entry = symbolExist(std::string(name));
  if(entry == nullptr){
    addSymbolToSymTable(0, "SCTN", "LOC", SECTION_INDEX++, std::string(name), "defined");
    CURR_SECTION_INDEX = SECTION_INDEX-1;
    sectionLocationCounter[CURR_SECTION_INDEX] = 0; // Initialize the location counter for the new section
    relocationTables[CURR_SECTION_INDEX];
  }
  else{
    CURR_SECTION_INDEX = entry->section_index;
  }
}

extern "C" void directive_word(){
  for (size_t i = 0; i < word_list.size(); i++) {
    if (word_list[i].type == SYMBOL_TYPE) {
      if(isDefined(std::string(word_list[i].value.symbol)) == false){
        addFlinkTableEntry(CURR_SECTION_INDEX, word_list[i].value.symbol, 0, sectionLocationCounter[CURR_SECTION_INDEX], '+', 0);
      }
      else{
        addToRelocationTable(-1, -1, std::string(word_list[i].value.symbol), 0);
      }
      
      splitAndInsertLiteralToMem(0);
    }
    else {
      splitAndInsertLiteralToMem(word_list[i].value.literal);
    }
  }
}

extern "C" void directive_skip(int literal){
  for(int i = 0; i < literal; i++){
    memoryMap[CURR_SECTION_INDEX].push_back(0);
  }
  sectionLocationCounter[CURR_SECTION_INDEX] += literal;
}

extern "C" void directive_ascii(const char* str){
  if (str == nullptr) return;
  std::string s = std::string(str);
  for (size_t i = 1; i < s.length()-1; i++) { //1 and length()-1 is to skip "
    memoryMap[CURR_SECTION_INDEX].push_back(static_cast<unsigned char>(s[i]));
  }
  sectionLocationCounter[CURR_SECTION_INDEX] += s.length();
}

// extern "C" void directive_ascii(const char* str) { //little endian version
//     if (str == nullptr) return;
//     std::string s = std::string(str);

//     // Reverse the string to simulate little-endian storage
//     std::reverse(s.begin(), s.end());

//     for (size_t i = 0; i < s.length(); ++i) {
//         memoryMap[CURR_SECTION_INDEX].push_back(static_cast<unsigned char>(s[i]));
//     }
//     sectionLocationCounter[CURR_SECTION_INDEX] += s.length();
// }


extern "C" void directive_equ(){

}

extern "C" void process_label(char* label){
  SymbolTableEntry* entry = symbolExist(std::string(label));
  if(entry == nullptr){
    addSymbolToSymTable(sectionLocationCounter[CURR_SECTION_INDEX], "NOTYP", "LOC", CURR_SECTION_INDEX, std::string(label), "defined");
    changeValFlinkTableEntry(CURR_SECTION_INDEX, label, sectionLocationCounter[CURR_SECTION_INDEX]); //we change symbol value of correspodning symbol in flink table 
  }
  else{
    if(entry->bind == "LOC" || (entry->bind == "GLOB" && entry->defined == "defined")){ // check if that symbol already exists (either LOC or GLOB) in symbol table
      std::cout<<"ERROR: Multiple symbol definitions inside 1 file, symbol name: "<<entry->name<<std::endl;
      exit(-2); //ERROR
    }
    entry->section_index = CURR_SECTION_INDEX;
    entry->value = sectionLocationCounter[CURR_SECTION_INDEX];
    entry->defined = "defined";
  }
}

//---------------------------------------------------------------------------------------

extern "C" void processFlinkTable(){
  //fill out missing symbol values in flink table
  for (auto& section : flinkTableMap) {
    for (auto& entry : section.second) { // Iterate over each entry in the current section
      SymbolTableEntry* symEntry = symbolExist(entry.symbol); // Check if the symbol exists in the symbol table
      if (symEntry != nullptr) {
        entry.symbol_value = symEntry->value; // Assign the value from the symbol table to the flink table entry
      } else {} // Symbol not found, ERROR
    }
  }

  //fill out relocation table, we iterate over flink table and put every entry in relocation table
  for (const auto& section : flinkTableMap) {
    for (const auto& entry : section.second) {
      for (const auto& pair : entry.address_sign_addend) {
        addToRelocationTable(pair.address, section.first, entry.symbol, pair.addend);
      }
    }
  }
}

//-----------------------------------------------------------------------------------------
//ELF FORMAT
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
void printShstrtab(const std::string& shstrtab) {
    for (size_t i = 0; i < shstrtab.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(shstrtab[i]);
        if (ch == '\0') {
            std::cout << "\\0";  // Print a visible representation for null characters
        } else if (std::isprint(ch)) {
            std::cout << ch;     // Print printable characters
        } else {
            std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
        }
    }
    std::cout << std::endl;
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
    size_t offset1 = sizes["shstrtab"];
    int counter = 0;
    for(int j = 0; j < symbolTable.size(); j++){
      if(symbolTable[j].type  == "SCTN"){
        shdr[i].sh_name = offsets_sections[counter++];
        shdr[i].sh_type = SHT_PROGBITS;
        shdr[i].sh_flags = 0;
        shdr[i].sh_addr = 0;
        shdr[i].sh_offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + offset1;
        shdr[i].sh_size = memoryMap[symbolTable[j].section_index].size();
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
      shdr[i].sh_size = relocationTables[j+1].size() * sizeof(Elf64_Rela); // Size of relocation table
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
    shdr[i].sh_link = 0;  // No link section
    shdr[i].sh_info = 0;  // No info
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
  for(int i = 1; i < memoryMap.size() + 1; i++){
    const std::vector<uint8_t>& sectionData = memoryMap[i];
    memcpy(elfFile + sectionOffset, sectionData.data(), sectionData.size());
    sectionOffset += memoryMap[i].size();
  }
}

void serializeRelocationTable(char* elfFile) {
  size_t offset = sizeof(Elf64_Ehdr) + sizes["headerSections"] + sizes["shstrtab"] + sizes["sectionData"];
  
  // Calculate total size needed for all relocation tables
  size_t totalSize = 0;
  std::vector<size_t> tableOffsets;
  for (int i = 1; i < relocationTables.size() + 1; i++) {
    tableOffsets.push_back(totalSize);
    totalSize += relocationTables[i].size() * sizeof(Elf64_Rela); 
  }
  
  // Write all relocation tables to ELF file
  for (size_t i = 0; i < tableOffsets.size(); i++) {
    const std::vector<RelocationEntry>& entries = relocationTables[i+1];
    Elf64_Rela* rela = reinterpret_cast<Elf64_Rela*>(elfFile + offset + tableOffsets[i]);
    
    for (size_t j = 0; j < entries.size(); j++) {
      const RelocationEntry& entry = entries[j];
      rela[j].r_offset = entry.offset;
      rela[j].r_info = ELF64_R_INFO(entry.symbol, R_X86_64_32S);
      rela[j].r_addend = entry.addend;
    }
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

std::string outputFileName;
extern "C" void setOutputFileName(char* fileName){
  outputFileName = std::string(fileName);
}

extern "C" void createELF() {
  sizes["symbolTable"] = symbolTable.size() * sizeof(Elf64_Sym);
  sizes["headerSections"] = (4 + 2 * relocationTables.size()) * sizeof(Elf64_Shdr);

  size_t relocationTablesSize = 0;
  for (int i = 1; i < relocationTables.size() + 1; i++) {
    relocationTablesSize += relocationTables[i].size() * sizeof(Elf64_Rela);
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
  for(int i = 1; i < memoryMap.size() + 1; i++){
    sectionDatasize += memoryMap[i].size();
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
  FILE* file = fopen(outputFileName.c_str(), "wb");
  if (file) {
    fwrite(elfFile, 1, fileSize, file);
    fclose(file);
  }

  delete[] elfFile;
}
