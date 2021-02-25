#include "huffman.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

void algo(std::string &type, std::string &file_in, std::string &file_out) {
	std::ifstream fi(file_in);

	if(fi.fail()) {
		fi.close();
		std::cout << "LOAD: unable to open input file." << "\n";	
		return;
	}

	uint8_t c1 = 0;
	std::vector<uint8_t> st;
	while(fi.read((char*)&c1, 1)) {
		st.push_back(c1);
	}	
	fi.close();
	
	HuffmanArchiver ar(st);
	std::vector<uint8_t> res;
	if (type == "compression")
		res = ar.compression();
	else
		res = ar.decompression();

	std::ofstream fo(file_out);

	if(fo.fail()) {
		fo.close();
		std::cout << "LOAD: unable to open output file." << "\n";	
		return;
	}

	for (size_t i = 0; i < res.size(); i++)
		fo.write((char*)&res[i], 1);

	fo.close();
	
	if (type == "compression")
		std::cout << ar.get_decompressed_size() << '\n' << ar.get_compressed_size() << '\n' << ar.get_table_size() << '\n';
	else
		std::cout << ar.get_compressed_size() << '\n' << ar.get_decompressed_size() << '\n' << ar.get_table_size() << '\n';
}

int main(int argc, char** argv) {
	std::string file_in = "", file_out = "";
	std::string type = "";
	for (int i = 1; i < argc; i++)
		if (strcmp(argv[i], "-c") == 0 && type == "")
			type = "compression";
		else if (strcmp(argv[i], "-u") == 0 && type == "")
			type = "decompression";
		else if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && file_in == "")
			file_in = argv[++i];
		else if ((strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) && file_out == "")
			file_out = argv[++i];
		else {
			std::cout << "Incorect input" << "\n";
			return 0;
		}

	try {
		algo(type, file_in, file_out);
	}
	catch(HuffmanException &e) {
		std::cout << e.what() << "\n";
	}
	catch(...) {
		std::cout << "Sorry, something went wrong...\n";
	}


	return 0;
}
