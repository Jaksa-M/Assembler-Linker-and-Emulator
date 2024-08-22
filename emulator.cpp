#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <string>

class RegisterFile {
private:
    std::array<int, 16> gprx; // General-purpose registers (0-15)
    std::array<int, 3> csrx;  // Control/Status registers (0-2)

public:
    RegisterFile() {
        gprx.fill(0);
        csrx.fill(0);
    }

    int getGPRX(int index) const {
        if (index < 0 || index >= gprx.size()) {
            throw std::out_of_range("Invalid GPRX register index");
        }
        return gprx[index];
    }

    void setGPRX(int index, int value) {
        if (index < 0 || index >= gprx.size()) {
            throw std::out_of_range("Invalid GPRX register index");
        }
        gprx[index] = value;
    }

    int getCSRX(int index) const {
        if (index < 0 || index >= csrx.size()) {
            throw std::out_of_range("Invalid CSRX register index");
        }
        return csrx[index];
    }

    void setCSRX(int index, int value) {
        if (index < 0 || index >= csrx.size()) {
            throw std::out_of_range("Invalid CSRX register index");
        }
        csrx[index] = value;
    }

    void displayRegisters() const {
        std::cout << "GPRX Registers:" << std::endl;
        for (int i = 0; i < gprx.size(); i++) {
            std::cout << "GPRX[" << i << "]: " << std::hex << gprx[i] << std::endl;
        }
        std::cout << "CSRX Registers:" << std::endl;
        for (int i = 0; i < csrx.size(); i++) {
            std::cout << "CSRX[" << i << "]: " << std::hex << csrx[i] << std::endl;
        }
    }
};

std::map<int, uint8_t> memoryMap;

std::map<int, uint8_t> readMemoryHexFile(const std::string& filename) {
    std::map<int, uint8_t> memoryMap;

    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return memoryMap;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        // Split the line into address and data part
        std::istringstream lineStream(line);
        std::string addressStr, dataStr;
        if (!(lineStream >> addressStr)) continue;  // Read address part

        int address;
        std::stringstream ss;
        ss << std::hex << addressStr;
        ss >> address;

        // Read the data bytes
        while (lineStream >> dataStr) {
            int byte;
            std::stringstream ssData;
            ssData << std::hex << dataStr;
            ssData >> byte;

            memoryMap[address++] = static_cast<uint8_t>(byte);
        }
    }

    inFile.close();
    return memoryMap;
}

void executeInstructions(RegisterFile* regFile){
    regFile->setGPRX(15, 0x40000000); //setting the initial value of pc(r15)
    int counter = 0;
    while (counter < memoryMap.size()) {
        size_t pc = regFile->getGPRX(15);
        uint8_t opcode1 = memoryMap[pc];
        uint8_t opcode2 = memoryMap[pc + 1];
        uint8_t opcode3 = memoryMap[pc + 2];
        uint8_t opcode4 = memoryMap[pc + 3];

        uint8_t upper4Bits1 = (opcode1 >> 4) & 0x0F;
        uint8_t lower4Bits1 = opcode1 & 0x0F;

        uint8_t upper4Bits2 = (opcode2 >> 4) & 0x0F;
        uint8_t lower4Bits2 = opcode2 & 0x0F;

        uint8_t upper4Bits3 = (opcode3 >> 4) & 0x0F;
        uint8_t lower4Bits3 = opcode3 & 0x0F;

        uint8_t upper4Bits4 = (opcode4 >> 4) & 0x0F;
        uint8_t lower4Bits4 = opcode4 & 0x0F;
        regFile->setGPRX(15, regFile->getGPRX(15) + 4); //pc = pc + 4
        counter += 4;

        switch(upper4Bits1) {
            int rA = regFile->getGPRX(upper4Bits2); // gpr[A]
            int rB = regFile->getGPRX(lower4Bits2); // gpr[B]
            int rC = regFile->getGPRX(upper4Bits3); // gpr[C]
            int D = 0;
            D |= (regFile->getGPRX(lower4Bits3) & 0x0F) << 8; // Shift lower4Bits3 to the leftmost 4 bits
            D |= (regFile->getGPRX(upper4Bits4) & 0x0F) << 4; // Shift upper4Bits4 to the middle 4 bits
            D |= (regFile->getGPRX(lower4Bits4) & 0x0F);
            case 0x00: // HALT
                return;
            case 0x01: // INT
                // push status
                int val = regFile->getCSRX(0); //status
                uint8_t byte1 = (val >> 24) & 0xFF; // highest byte
                uint8_t byte2 = (val >> 16) & 0xFF;
                uint8_t byte3 = (val >> 8) & 0xFF;
                uint8_t byte4 = val & 0xFF;
                regFile->setGPRX(14, regFile->getGPRX(14) - 4); //sp = sp - 4
                memoryMap[regFile->getGPRX(14)] = byte1;
                memoryMap[regFile->getGPRX(14) - 1] = byte2;
                memoryMap[regFile->getGPRX(14) - 2] = byte3;
                memoryMap[regFile->getGPRX(14) - 3] = byte4;

                // push pc
                val = regFile->getCSRX(15); //pc
                uint8_t byte1 = (val >> 24) & 0xFF; // highest byte
                uint8_t byte2 = (val >> 16) & 0xFF;
                uint8_t byte3 = (val >> 8) & 0xFF;
                uint8_t byte4 = val & 0xFF;
                regFile->setGPRX(14, regFile->getGPRX(14) - 4); //sp = sp - 4
                memoryMap[regFile->getGPRX(14)] = byte1;
                memoryMap[regFile->getGPRX(14) - 1] = byte2;
                memoryMap[regFile->getGPRX(14) - 2] = byte3;
                memoryMap[regFile->getGPRX(14) - 3] = byte4;

                // cause <= 4
                regFile->setCSRX(2, 4);

                // status <= status & (~0x1)
                regFile->setCSRX(0, regFile->getCSRX(0) & (~0x01));

                // pc <= handle
                regFile->setGPRX(15, regFile->getCSRX(1));
                break;
            case 0x02: // CALL
                // push pc (both instructions have it)
                int val = regFile->getGPRX(15); //pc
                uint8_t byte1 = (val >> 24) & 0xFF;
                uint8_t byte2 = (val >> 16) & 0xFF;
                uint8_t byte3 = (val >> 8) & 0xFF;
                uint8_t byte4 = val & 0xFF;
                regFile->setGPRX(14, regFile->getGPRX(14) - 4); //sp = sp - 4
                memoryMap[regFile->getGPRX(14)] = byte1;
                memoryMap[regFile->getGPRX(14) - 1] = byte2;
                memoryMap[regFile->getGPRX(14) - 2] = byte3;
                memoryMap[regFile->getGPRX(14) - 3] = byte4;
                // gpr[A] + gpr[B] + D
                val = rA + rB + D;
                switch(lower4Bits1){
                    case 0x00: // pc <= gpr[A] + gpr[B] + D
                        regFile->setGPRX(15, val);//temp = 2 * 33
                        break;
                    case 0x01: // pc<=mem32[gpr[A]+gpr[B]+D]
                        int temp = 0;
                        temp |= static_cast<int>(memoryMap[val]) << 0;   // byte1 (lowest byte)
                        temp |= static_cast<int>(memoryMap[val + 1]) << 8; // byte2
                        temp |= static_cast<int>(memoryMap[val + 2]) << 16; // byte3
                        temp |= static_cast<int>(memoryMap[val + 3]) << 24; 
                        regFile->setGPRX(15, temp); // pc = temp
                        break;
                }
                break;
            case 0x03: // JMP
                switch(lower4Bits1){
                    int temp = rA + D; // gpr[A] + D

                    // mem32[gpr[A] + D]
                    int val = 0;
                    val |= static_cast<int>(memoryMap[temp]) << 0;   // byte1 (lowest byte)
                    val |= static_cast<int>(memoryMap[temp + 1]) << 8; // byte2
                    val |= static_cast<int>(memoryMap[temp + 2]) << 16; // byte3
                    val |= static_cast<int>(memoryMap[temp + 3]) << 24; // byte4
                    case 0x00: // pc <= gpr[A] + D
                        regFile->setGPRX(15, temp);
                        break;
                    case 0x01: // if (gpr[B] == gpr[C]) pc <= gpr[A] + D
                        if (rB == rC) regFile->setGPRX(15, temp);
                        break;
                    case 0x02: // if (gpr[B] != gpr[C]) pc <= gpr[A] + D
                        if (rB != rC) regFile->setGPRX(15, temp);
                        break;
                    case 0x03: // if (gpr[B] signed> gpr[C]) pc <= gpr[A] + D
                        if (rB > rC) regFile->setGPRX(15, temp);
                        break;
                    case 0x08: // pc <= mem32[gpr[A] + D]
                        regFile->setGPRX(15, val);
                        break;
                    case 0x09: // if (gpr[B] == gpr[C]) pc <= mem32[gpr[A] + D]
                        if (rB == rC) regFile->setGPRX(15, val);
                        break;
                    case 0x0a: // if (gpr[B] != gpr[C]) pc <= mem32[gpr[A] + D]
                        if (rB != rC) regFile->setGPRX(15, val);
                        break;
                    case 0x0b: // if (gpr[B] signed> gpr[C]) pc <= mem32[gpr[A] + D]
                        if (rB > rC) regFile->setGPRX(15, val);
                        break;
                }
                break;
            case 0x04: // XCHG
                int temp = rB; // temp <= gpr[B];
                regFile->setGPRX(lower4Bits2, rC); // gpr[B] <= gpr[C];
                regFile->setGPRX(upper4Bits3, temp); // gpr[C] <= temp
                break;
            case 0x05:
                switch(lower4Bits1){
                    case 0x00: // ADD, gpr[A] <= gpr[B] + gpr[C]
                        regFile->setGPRX(upper4Bits2, rB + rC);
                        break;
                    case 0x01: // SUB, gpr[A] <= gpr[B] - gpr[C]
                        regFile->setGPRX(upper4Bits2, rB - rC);
                        break;
                    case 0x02: // MUL, gpr[A] <= gpr[B] * gpr[C]
                        regFile->setGPRX(upper4Bits2, rB * rC);
                        break;
                    case 0x03: // DIV, gpr[A] <= gpr[B] / gpr[C]
                        regFile->setGPRX(upper4Bits2, rB / rC);
                        break;
                }
                break;
            case 0x06:
                switch(lower4Bits1){
                    case 0x00: // NOT,  gpr[A]<=~gpr[B]
                        regFile->setGPRX(upper4Bits2, ~rB);
                        break;
                    case 0x01: // AND, gpr[A]<=gpr[B] & gpr[C]
                        regFile->setGPRX(upper4Bits2, rB & rC);
                        break;
                    case 0x02: // OR, gpr[A]<=gpr[B] | gpr[C]
                        regFile->setGPRX(upper4Bits2, rB | rC);
                        break;
                    case 0x03: // XOR, gpr[A]<=gpr[B] ^ gpr[C]
                        regFile->setGPRX(upper4Bits2, rB ^ rC);
                        break;
                }
                break;
            case 0x07:
                switch(lower4Bits1){
                    case 0x00: // SHL, gpr[A] <= gpr[B] << gpr[C]
                        regFile->setGPRX(upper4Bits2, rB << rC);
                        break;
                    case 0x01: // SHR, gpr[A] <= gpr[B] >> gpr[C]
                        regFile->setGPRX(upper4Bits2, rB >> rC);
                        break;
                }
                break;
            case 0x08:
                int val = rA + rB + D; // gpr[A] + gpr[B] + D
                switch(lower4Bits1){ // order is 2, than 1 in project file so i did the same
                    case 0x00: // mem32[gpr[A] + gpr[B] + D] <= gpr[C]
                        memoryMap[val] = rC & 0xFF; // Lower 8 bits
                        memoryMap[val + 1] = (rC >> 8) & 0xFF;  // Next 8 bits
                        memoryMap[val + 2] = (rC >> 16) & 0xFF; // Next 8 bits
                        memoryMap[val + 3] = (rC >> 24) & 0xFF; // Highest 8 bits
                        break;
                    case 0x02: // mem32[mem32[gpr[A] + gpr[B] + D]] <= gpr[C]
                        uint32_t temp = 0;
                        temp |= memoryMap[val]; // Lowest 8 bits
                        temp |= (memoryMap[val + 1] << 8);
                        temp |= (memoryMap[val + 2] << 16);
                        temp |= (memoryMap[val + 3] << 24);

                        memoryMap[temp] = rC & 0xFF; // Lower 8 bits
                        memoryMap[temp + 1] = (rC >> 8) & 0xFF;
                        memoryMap[temp + 2] = (rC >> 16) & 0xFF;
                        memoryMap[temp + 3] = (rC >> 24) & 0xFF;
                        break;
                    case 0x01: // gpr[A] <= gpr[A] + D; mem32[gpr[A]] <= gpr[C]
                        regFile->setGPRX(upper4Bits2, rA + D);
                        int rA1 = regFile->getGPRX(upper4Bits2);
                        memoryMap[rA1] = rC & 0xFF; // Lower 8 bits
                        memoryMap[rA1 + 1] = (rC >> 8) & 0xFF;  // Next 8 bits
                        memoryMap[rA1 + 2] = (rC >> 16) & 0xFF; // Next 8 bits
                        memoryMap[rA1 + 3] = (rC >> 24) & 0xFF; // Highest 8 bits
                        break;
                }
                break;
            case 0x09:
                switch(lower4Bits1){
                    case 0x00: // gpr[A] <= csr[B]
                        regFile->setGPRX(upper4Bits2, regFile->getCSRX(lower4Bits2));
                        break;
                    case 0x01: // gpr[A] <= gpr[B] + D
                        regFile->setGPRX(upper4Bits2, rB + D);
                        break;
                    case 0x02: // gpr[A] <= mem32[gpr[B] + gpr[C] + D]
                        int temp = rB + rC + D;
                        int val = 0;
                        val |= static_cast<int>(memoryMap[temp]) << 0;   // byte1 (lowest byte)
                        val |= static_cast<int>(memoryMap[temp + 1]) << 8; // byte2
                        val |= static_cast<int>(memoryMap[temp + 2]) << 16; // byte3
                        val |= static_cast<int>(memoryMap[temp + 3]) << 24; // byte4
                        regFile->setGPRX(upper4Bits2, val);
                        break;
                    case 0x03: // gpr[A] <= mem32[gpr[B]]; gpr[B] <= gpr[B] + D
                        int val = 0;
                        val |= static_cast<int>(memoryMap[rB]) << 0;   // byte1 (lowest byte)
                        val |= static_cast<int>(memoryMap[rB + 1]) << 8; // byte2
                        val |= static_cast<int>(memoryMap[rB + 2]) << 16; // byte3
                        val |= static_cast<int>(memoryMap[rB + 3]) << 24; // byte4
                        regFile->setGPRX(upper4Bits2, val);
                        regFile->setGPRX(lower4Bits2, rB + D);
                        break;
                    case 0x04: // csr[A] <= gpr[B]
                        regFile->setCSRX(upper4Bits2, rB);
                        break;
                    case 0x05: // csr[A] <= csr[B] | D
                        regFile->setCSRX(upper4Bits2, regFile->getCSRX(lower4Bits2) | D);
                        break;
                    case 0x06: // csr[A] <= mem32[gpr[B] + gpr[C] + D]
                        int temp = rB + rC + D;
                        int val = 0;
                        val |= static_cast<int>(memoryMap[temp]) << 0;   // byte1 (lowest byte)
                        val |= static_cast<int>(memoryMap[temp + 1]) << 8; // byte2
                        val |= static_cast<int>(memoryMap[temp + 2]) << 16; // byte3
                        val |= static_cast<int>(memoryMap[temp + 3]) << 24; // byte4
                        regFile->setCSRX(upper4Bits2, val);
                        break;
                    case 0x07: // csr[A] <= mem32[gpr[B]]; gpr[B] <= gpr[B] + D
                        int val = 0;
                        val |= static_cast<int>(memoryMap[rB]) << 0;   // byte1 (lowest byte)
                        val |= static_cast<int>(memoryMap[rB + 1]) << 8; // byte2
                        val |= static_cast<int>(memoryMap[rB + 2]) << 16; // byte3
                        val |= static_cast<int>(memoryMap[rB + 3]) << 24; // byte4
                        regFile->setCSRX(upper4Bits2, val);

                        regFile->setGPRX(lower4Bits2, rB + D);
                        break;
                }
                break;
            default:
                throw std::runtime_error("Unknown opcode encountered");
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Hex file not specified" << std::endl;
        return 1;
    }
    std::string fileName = argv[1];
    memoryMap = readMemoryHexFile(fileName);

    // Print the content of the memory map to verify
    for (const auto& entry : memoryMap) {
        std::cout << std::hex << std::setw(4) << std::setfill('0') << entry.first << ": "
                  << std::setw(2) << std::setfill('0') << static_cast<int>(entry.second) << std::endl;
    }

    RegisterFile* regFile = new RegisterFile();
    //executeInstructions(regFile);

    return 0;
}
