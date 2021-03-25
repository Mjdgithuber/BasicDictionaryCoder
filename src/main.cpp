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
	std::ofstream outf(out.c_str(), std::ifstream::out);
	while(inf >> line) {
		// check if current line is in dict
		auto itr = hmap.find(line);
		if(itr == hmap.end()) {
			itr = hmap.insert({line, (unsigned)dict.size()}).first;
			dict.push_back(line);
		}

		unsigned e = itr->second;
		outf << e << "\n";
	}

	for(unsigned i = 0; i < dict.size(); i++)
		outf << dict[i] << "\n";
	

	inf.close();
}

int main() {
	encode("test.txt", "out.txt");

	return 0;
}
