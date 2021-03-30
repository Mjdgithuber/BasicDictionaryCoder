#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

	inf.close();
	outf.close();
}

char load_dict(std::vector<std::string>& dict, std::ifstream& in_file, unsigned long long& dict_size, unsigned long long& dict_offset,
	const std::string& query) {
	// read dict info
	in_file.read((char*)(&dict_offset), sizeof(dict_offset));
	in_file.read((char*)(&dict_size), sizeof(dict_size));

	// jump to dict
	long pos = in_file.tellg();
	in_file.seekg(pos+dict_offset);

	// read one char at a time to build word
	char ret = -1;
	char buf;
	std::string w;
	while(dict_size > 0) {
		in_file.read(&buf, 1);
		if(buf) w += buf;
		else { // add to dict
			dict.push_back(w);
			dict_size--;

			// check for match
			if(w == query)
				ret = dict.size() - 1;
			w.clear();
		}
	}

	// move file head back to data section
	in_file.seekg(pos);
	if(pos != in_file.tellg()) {
		std::cout << "Failed to decode file!\n";
		return -2;
	}
	return ret;
}

template <typename W_OFFSET_T>
void decode(const std::string& in, const std::string& out) {
	std::vector<std::string> dict;
	
	std::ifstream inf(in.c_str(), std::ifstream::in | std::ifstream::binary);
	std::ofstream outf(out.c_str(), std::ofstream::out);

	// get dict size
	unsigned long long d_off, d_size;
	
	if(load_dict(dict, inf, d_size, d_off, "") == -2)
		return;

	// decode file
	for(unsigned i = 0; i < (d_off / sizeof(W_OFFSET_T)); i++) {
		// get dict offset and write to file
		unsigned off;
		inf.read((char*) &off, sizeof(W_OFFSET_T));
		outf << dict[off] << "\n";
	}

	inf.close();
	outf.close();
}

template <typename W_OFFSET_T>
void query(const std::string& in, const std::string& query) {
	std::ifstream inf(in.c_str(), std::ifstream::in | std::ifstream::binary);

	int index;
	unsigned long long d_off, d_size;
	std::vector<std::string> dict;
	if((index = load_dict(dict, inf, d_size, d_off, query)) < 0) {
		std::cout << "'" << query << "' does not show up in " << in << "\n";
		return;
	}

	int count = 0;
	for(unsigned i = 0; i < (d_off / sizeof(W_OFFSET_T)); i++) {
		unsigned off;
		inf.read((char*) &off, sizeof(W_OFFSET_T));
		count += (off == index) ? 1 : 0;
	}

	std::cout << "'" << query << "' shows up " << count << " times in " << in << "\n";

	inf.close();
}

int main(int argc, char** argv) {
	std::string args[3];

	if(argc != 3 && argc != 4) {
		std::cout << "Must be run with at 3 or 4 args!\n" <<
			"\t-c <file_to_compress>\n" <<
			"\t-d <file_to_decode>\n" <<
			"\t-q <file_to_query> query_term\n";
		return 0;
	}

	for(unsigned i = 0; i < (argc-1) && i < 3; i++)
		args[i] = argv[i+1];

	if(args[0] == "-c")
		encode<unsigned>(args[1], args[1] + ".enc");
	else if(args[0] == "-d")
		decode<unsigned>(args[1], args[1] + ".dec");
	else if(args[0] == "-q" && argc == 4)
		query<unsigned>(args[1], args[2]);
	else
		std::cout << args[0] << " is " << ((args[0] == "-q") ? "missing query string" : "not a valid command") << "!\n";

	return 0;
}
