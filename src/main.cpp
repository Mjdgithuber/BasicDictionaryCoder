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
			//auto p = std::make_pair<std::string, unsigned int>(line, (unsigned)dict.size());
			itr = hmap.insert({line, (unsigned)dict.size()}).first;
			//itr = hmap.insert(p).first;
			//itr = (hmap.insert(std::make_pair<std::string, unsigned int>(line, dict.size()).first;
			//hmap[line] = dict.size();
			dict.push_back(line);
		}

		unsigned e = itr->second;
		outf << e << "\n";
		//std::cout << line << "\n";
	}
	

	inf.close();
}

int main() {
	encode("test.txt", "out.txt");

	return 0;
}
