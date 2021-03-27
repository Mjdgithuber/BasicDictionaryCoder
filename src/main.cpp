#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void encode(const std::string& in, const std::string& out) {
	std::vector<std::string> dict;
	std::unordered_map<std::string, unsigned int> hmap;

	std::string line;
	std::ifstream inf(in.c_str(), std::ifstream::in);
	std::ofstream outf(out.c_str(), std::ofstream::out | std::ofstream::binary);

	unsigned long long d_off = 12345, d_size = 9;
	outf.write((const char*)(&d_off), sizeof(d_off));
	outf.write((const char*)(&d_size), sizeof(d_size));
	while(inf >> line) {
		// check if current line is in dict
		auto itr = hmap.find(line);
		if(itr == hmap.end()) {
			itr = hmap.insert({line, (unsigned)dict.size()}).first;
			dict.push_back(line);
		}

		// get word offset & write to file
		unsigned w_off = itr->second;
		outf.write((const char*)(&w_off), sizeof(w_off));
	}

	d_size = dict.size();
	//outf.write((const char*)(&d_size), sizeof(d_size));

	//for(unsigned i = 0; i < dict.size(); i++)
		//outf << dict[i] << "\n";
	

	inf.close();
	outf.close();
}

void decode(const std::string& in, const std::string& out) {
	std::vector<std::string> dict;
	
	std::ifstream inf(in.c_str(), std::ifstream::in | std::ifstream::binary);
	std::ofstream outf(out.c_str(), std::ofstream::out);

	// get dict size
	unsigned long long d_off, d_size;
	inf.read((char*)(&d_off), sizeof(d_off));
	inf.read((char*)(&d_size), sizeof(d_size));

	std::cout << "Off: " << d_off << " Size: " << d_size << "\n";

	inf.close();
	outf.close();
}

int main() {
	encode("test.txt", "out.txt");
	decode("out.txt", "del_me.txt");


	return 0;
}
