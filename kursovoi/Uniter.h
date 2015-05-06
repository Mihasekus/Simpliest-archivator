#include <iostream>
#include <string>
#include <vector>


class Uniter
{

private:
	vector<string> files;
	string path;
	string real_bin_file;
public:
	Uniter(vector<string> &vec, string p)
	{
		if (vec.size()>0) files.assign(vec.begin(), vec.end());
		path = p + "\\";
		real_bin_file = path + "binary.zipper";
	}
	void getInfo();
	void ToUnite();
	void ToUnpack(string binary);

	static string get_file_name(string fn)
	{
		return fn.substr(fn.find_last_of("\\") + 1, fn.size());
	}
};
