void Assembler::elfWrite() {
  ofstream ofs(outputFileName, ios::binary);
  if(!ofs) {
    cerr << "Failed to open file for writing." << endl;
    return;
  }

  // ELF Header
  Elf64_Ehdr ehdr;
  std::memset(&ehdr, 0, sizeof(ehdr));
  ehdr.e_ident[EI_MAG0] = ELFMAG0;
  ehdr.e_ident[EI_MAG1] = ELFMAG1;
  ehdr.e_ident[EI_MAG2] = ELFMAG2;
  ehdr.e_ident[EI_MAG3] = ELFMAG3;
  ehdr.e_ident[EI_CLASS] = ELFCLASS64;
  ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
  ehdr.e_ident[EI_VERSION] = EV_CURRENT;
  ehdr.e_ident[EI_OSABI] = ELFOSABI_NONE;
  ehdr.e_type = ET_REL;
  ehdr.e_machine = EM_X86_64;
  ehdr.e_version = EV_CURRENT;
  ehdr.e_phoff = 0;
  ehdr.e_shoff = sizeof(Elf64_Ehdr);
  ehdr.e_flags = 0;
  ehdr.e_ehsize = sizeof(Elf64_Ehdr);
  ehdr.e_phentsize = 0;
  ehdr.e_phnum = 0;
  ehdr.e_shentsize = sizeof(Elf64_Shdr);
  ehdr.e_shnum = sections.size() * 2 + 4;
  ehdr.e_shstrndx = 1;
  
  ofs.write(reinterpret_cast<char*>(&ehdr), sizeof(ehdr));

  // Section Headers
  vector<Elf64_Shdr> shdrs(ehdr.e_shnum);

  // Null section
  memset(&shdrs[0], 0, sizeof(Elf64_Shdr));

  // Section names (shstrtab)
  string shstrtab;
  shstrtab.append("\0", 1);
  vector<size_t> section_name_offsets;
  vector<size_t> rela_name_offsets;

  for(const auto& section : sections) {
    section_name_offsets.push_back(shstrtab.size());
    shstrtab.append(section.sectionName.c_str(), section.sectionName.size() + 1);

    rela_name_offsets.push_back(shstrtab.size());
    shstrtab.append((".rela" + section.sectionName + "\0").c_str(), section.sectionName.size() + 6);
  }

  size_t shstrtab_offset = shstrtab.size();
  shstrtab.append(".shstrtab\0", 10);
  size_t symtab_offset = shstrtab.size();
  shstrtab.append(".symtab\0", 8);
  size_t strtab_offset = shstrtab.size();
  shstrtab.append(".strtab\0", 8);

  // shstrtab section header
  shdrs[1].sh_name = shstrtab_offset;
  shdrs[1].sh_type = SHT_STRTAB;
  shdrs[1].sh_offset = sizeof(Elf64_Ehdr) + shdrs.size() * sizeof(Elf64_Shdr);
  shdrs[1].sh_size = shstrtab.size();
  size_t offset = shdrs[1].sh_offset + shdrs[1].sh_size;

  // Section data header
  for(size_t i = 0; i < sections.size(); i++) {
    shdrs[i + 2].sh_name = section_name_offsets[i];
    shdrs[i + 2].sh_type = SHT_PROGBITS;
    shdrs[i + 2].sh_offset = offset;
    shdrs[i + 2].sh_size = sections[i].sectionData8bitValues.size();
    offset += shdrs[i + 2].sh_size;
  }

  // Relocation table headers
  for(size_t i = 0; i < sections.size(); ++i) {
    shdrs[sections.size() + i + 2].sh_name = rela_name_offsets[i];
    shdrs[sections.size() + i + 2].sh_type = SHT_RELA;
    shdrs[sections.size() + i + 2].sh_offset = offset;
    shdrs[sections.size() + i + 2].sh_size = sections[i].relaTable.size() * sizeof(Elf64_Rela);
    shdrs[sections.size() + i + 2].sh_link = sections.size() * 2 + 2; // Link to .symtab
    shdrs[sections.size() + i + 2].sh_info = i + 2; // Index of the associated section
    shdrs[sections.size() + i + 2].sh_addralign = 8;
    shdrs[sections.size() + i + 2].sh_entsize = sizeof(Elf64_Rela);
    offset += shdrs[sections.size() + i + 2].sh_size;
  }

  // Symtab header
  shdrs[sections.size() * 2 + 2].sh_name = symtab_offset;
  shdrs[sections.size() * 2 + 2].sh_type = SHT_SYMTAB;
  shdrs[sections.size() * 2 + 2].sh_offset = offset;
  shdrs[sections.size() * 2 + 2].sh_size = symbolTable.size() * sizeof(Elf64_Sym);
  shdrs[sections.size() * 2 + 2].sh_link = sections.size() * 2 + 3; // Link to .strtab
  shdrs[sections.size() * 2 + 2].sh_info = symbolTable.size();
  shdrs[sections.size() * 2 + 2].sh_addralign = 8;
  shdrs[sections.size() * 2 + 2].sh_entsize = sizeof(Elf64_Sym);
  offset += shdrs[sections.size() * 2 + 2].sh_size;

  // Strtab header
  shdrs[sections.size() * 2 + 3].sh_name = strtab_offset;
  shdrs[sections.size() * 2 + 3].sh_type = SHT_STRTAB;
  shdrs[sections.size() * 2 + 3].sh_offset = offset;
  shdrs[sections.size() * 2 + 3].sh_size = 1; // initially only null byte
  for(const auto& sym : symbolTable) {
    shdrs[sections.size() * 2 + 3].sh_size += sym.name.size() + 1;
  }

  ofs.write(reinterpret_cast<char*>(shdrs.data()), shdrs.size() * sizeof(Elf64_Shdr));

  // Write shstrtab
  ofs.write(shstrtab.c_str(), shstrtab.size());

  // Write section data
  for(const auto& section : sections) {
    ofs.write(reinterpret_cast<const char*>(section.sectionData8bitValues.data()), section.sectionData8bitValues.size());
  }

  // Write relocation tables
  for(const auto& section : sections) {
    for(const auto& rela : section.relaTable) {
      Elf64_Rela elf_rela;
      elf_rela.r_offset = rela.offset;
      elf_rela.r_info = ELF64_R_INFO(rela.symbol, (rela.type == MY_R_X86_64_32S) ? R_X86_64_32S : 0);
      elf_rela.r_addend = rela.addend;
      ofs.write(reinterpret_cast<const char*>(&elf_rela), sizeof(elf_rela));
    }
  }

  // Write symtab
  string strtab;
  strtab.append("\0", 1); // NULL byte
  for(const auto& sym : symbolTable) {
    Elf64_Sym symbol;
    memset(&symbol, 0, sizeof(symbol));
    symbol.st_name = strtab.size(); // Offset in strtab
    symbol.st_info = ELF64_ST_INFO((sym.bind == GLOB) ? STB_GLOBAL : STB_LOCAL, (sym.type == SCTN) ? STT_SECTION : STT_NOTYPE);
    symbol.st_other = 0;
    symbol.st_shndx = (sym.sectionIndex == -1 ? SHN_ABS : sym.sectionIndex);
    symbol.st_value = sym.value;
    symbol.st_size = 0; // Size is usually set by the linker
    ofs.write(reinterpret_cast<char*>(&symbol), sizeof(symbol));
    strtab.append(sym.name.c_str(), sym.name.size() + 1);
  }

  // Write strtab
  ofs.write(strtab.c_str(), strtab.size());

  ofs.close();
}