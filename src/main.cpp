#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//typedef short unsigned int W_OFFSET_T;


template <typename W_OFFSET_T>
void encode(const std::string& in, const std::string& out) {
	std::vector<std::string> dict;
	std::unordered_map<std::string, W_OFFSET_T> hmap;

	std::string line;
	std::ifstream inf(in.c_str(), std::ifstream::in);
	std::ofstream outf(out.c_str(), std::ofstream::out | std::ofstream::binary);

	// read cols and encode + write them to out file
	unsigned long long d_off = 0, d_size;
	outf.write((const char*)(&d_off), sizeof(d_off));
	outf.write((const char*)(&d_size), sizeof(d_size));
	while(inf >> line) {
		// check if current line is in dict
		auto itr = hmap.find(line);
		if(itr == hmap.end()) {
			itr = hmap.insert({line, (W_OFFSET_T)dict.size()}).first;
			dict.push_back(line);
		}

		// get word offset & write to file
		W_OFFSET_T w_off = itr->second;
		outf.write((const char*)(&w_off), sizeof(w_off));
		d_off += sizeof(w_off);
	}

	// dump dict with nulls to file
	d_size = dict.size();
	for(unsigned i = 0; i < d_size; i++)
		outf.write(dict[i].c_str(), dict[i].size() + 1);

	// return to output start to write dict info
	outf.seekp(0);
	outf.write((const char*)(&d_off), sizeof(d_off));
	outf.write((const char*)(&d_size), sizeof(d_size));


	//outf.write((const char*)(&d_size), sizeof(d_size));
	//for(unsigned i = 0; i < dict.size(); i++)
		//outf << dict[i] << "\n";
	

	inf.close();
	outf.close();
}

template <typename W_OFFSET_T>
void decode(const std::string& in, const std::string& out) {
	std::vector<std::string> dict;
	
	std::ifstream inf(in.c_str(), std::ifstream::in | std::ifstream::binary);
	std::ofstream outf(out.c_str(), std::ofstream::out);

	// get dict size
	unsigned long long d_off, d_size;
	inf.read((char*)(&d_off), sizeof(d_off));
	inf.read((char*)(&d_size), sizeof(d_size));

	//std::cout << "Off: " << d_off << " Size: " << d_size << "\n";

	// jump to dict
	long pos = inf.tellg();
	inf.seekg(pos+d_off);

	char buf;
	std::string w;
	while(d_size > 0) {
		inf.read(&buf, 1);
		if(buf) w += buf;
		else { // add to dict
			dict.push_back(w);
			d_size--;
			w.clear();
		}
	}

	std::cout << "Dict size: " << dict.size() << " -> words:\n";
	//for(unsigned i = 0; i < dict.size(); i++)
	//	std::cout << dict[i] << "\n";

	// decode file
	inf.seekg(pos); // reset
	for(unsigned i = 0; i < (d_off / sizeof(W_OFFSET_T)); i++) {
		// get dict offset and write to file
		unsigned off;
		inf.read((char*) &off, sizeof(W_OFFSET_T));
		outf << dict[off] << "\n";
	}

	inf.close();
	outf.close();
}

int main() {
	encode<short unsigned>("med.txt", "encode.zl");
	decode<short unsigned>("encode.zl", "decode.txt");

	//encode("test.txt", "out.txt");
	//decode("out.txt", "del_me.txt");


	return 0;
}
