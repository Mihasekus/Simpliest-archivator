#include <iostream>
#include <string>
#include <vector>



int digs(double w)
{
	int yield = 0;
	while (w>10) { yield++; w /= 10; }
	return yield + 1;
}

class Uniter(vector<string> &vec, string p)
{
	if (vec.size()>0) files.assign(vec.begin(), vec.end());
	path = p + "\\";
	real_bin_file = path + "binary.zipper";
}

void Uniter::getInfo()
{
	char byte[1];

	basic_string<char> s_info = "";
	remove((this->path + "info.txt").c_str());
	FILE *info = fopen((this->path + "info.txt").c_str(), "a+");
	int bytes_size = 0;
	for (vector<string>::iterator itr = this->files.begin(); itr != this->files.end(); ++itr)
	{
		FILE *f = fopen((*itr).c_str(), "rb");
		if (!f) break;

		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		string name = Uniter::get_file_name(*itr);
		char *m_size = new char[digs(size) + 1];
		_itoa(size, m_size, 10);
		fclose(f);

		bytes_size += digs(size);
		bytes_size += strlen(name.c_str());

		s_info.append(m_size);
		s_info.append("||");
		s_info.append(name);
		s_info.append("||");

		delete[] m_size;

	}
	bytes_size = s_info.size() + 2;
	char *b_buff = new char[digs(bytes_size)];
	_itoa(bytes_size, b_buff, 10);
	if (digs(bytes_size)<5) fputs(string(5 - digs(bytes_size), '0').c_str(), info);
	fputs(b_buff, info);
	fputs("||", info);
	fputs(s_info.c_str(), info);

	fclose(info);
}

void Uniter::ToUnite()
{
	char byte[1];

	getInfo();
	FILE *f, *main = fopen((this->real_bin_file).c_str(), "wb");
	FILE *info = fopen((this->path + "info.txt").c_str(), "rb");
	while (!feof(info))
	{
		if (fread(byte, 1, 1, info) == 1) fwrite(byte, 1, 1, main);
	}

	fclose(info);
	remove((this->path + "info.txt").c_str());

	for (vector<string>::iterator itr = this->files.begin(); itr != this->files.end(); ++itr)
	{
		f = fopen((*itr).c_str(), "rb");
		if (!f){ cout << *itr << " не найден!" << endl; break; }
		while (!feof(f))
		{
			if (fread(byte, 1, 1, f) == 1) fwrite(byte, 1, 1, main);
		}
		cout << *itr << " добавлен в архив '" << this->real_bin_file << "'." << endl;
		fclose(f);
	}
	fclose(main);
}


void Uniter::ToUnpack(string binary)
{
	FILE *bin = fopen(binary.c_str(), "rb");
	char info_block_size[5];
	fread(info_block_size, 1, 5, bin);
	int _sz = atoi(info_block_size);

	char *info_block = new char[_sz];
	fread(info_block, 1, _sz, bin);

	vector<string> tokens;
	char *tok = strtok(info_block, "||");
	int toks = 0;
	while (tok)
	{
		if (strlen(tok) == 0) break;
		tokens.push_back(tok);
		tok = strtok(NULL, "||");
		toks++;
	}
	if (toks % 2 == 1) toks--;
	int files = toks / 2;

	char byte[1];

	for (int i = 0; i<files; i++)
	{
		const char* size = tokens[i * 2].c_str();
		const char* name = tokens[i * 2 + 1].c_str();
		char full_path[255];
		strcpy(full_path, this->path.c_str());
		strcat(full_path, name);
		int _sz = atoi(size);
		
		FILE *curr = fopen(full_path, "wb");
		for (int r = 1; r <= _sz; r++)
		{
			if (fread(byte, 1, 1, bin) == 1) fwrite(byte, 1, 1, curr);
		}
		fclose(curr);

		delete[] size;
		delete[] name;
	}
	fclose(bin);

}
