#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <vector>
#include <stdio.h>
#include <memory.h>
#include <algorithm>

#include "ErrorHandle.h"

#define COMMENT_MARK "#"
#define IS_SPACE(ch) (ch.find(" ") == 0 || ch.find("\t") == 0 || ch.find("\n") == 0 || ch.find("\r") == 0)

class Config
{
private:
	std::string conf_path;
	std::vector<std::pair<std::string, std::string>> conf_vec;

private:
	TG_CODE ReadConfFile();
	TG_CODE SaveConfFile();
	std::string TrimSpace(const std::string& str_data);

public:
	Config() = delete;
	explicit Config(const std::string & conf_path);

	std::string Get(const std::string & conf_name);
	std::string Get(const std::string & conf_name, const std::string & default_value);
	TG_CODE Get(const std::string & conf_name, std::string & conf_value);
	
	TG_CODE Add(const std::string & conf_name, const std::string & conf_value, const std::string & conf_remark = std::string(""));
	TG_CODE Update(const std::string & conf_name, const std::string & conf_value, const std::string & conf_remark = std::string(""));
	TG_CODE Delete(const std::string & conf_name);
};


inline Config::Config(const std::string & path) : conf_path(path)
{
	ReadConfFile();
}

inline TG_CODE Config::ReadConfFile()
{
	size_t file_size;
	char* file_buf = NULL;
	bool line_head = true;
	bool remark = false;
	bool make_value = false;
	std::string conf_name;
	std::string conf_value;
	std::string conf_remark;

	FILE * _file = fopen(conf_path.c_str(), "r+");

	if (_file == NULL)
	{
		_file = fopen(conf_path.c_str(), "w+");
		if (NULL == _file)
		{
			return TG_CODE(UNI_INVALID_FP_VALUE);
		}
	}

	fseek(_file, 0, SEEK_END);
	file_size = ftell(_file);

	if (file_size)
	{
		file_buf = (char*)malloc(file_size);
	}

	if (nullptr != file_buf)
	{
		memset(file_buf, 0, file_size);

		if (fread(file_buf, file_size, 1, _file))
		{
			fclose(_file);
		}
		else
		{
			if (nullptr != file_buf)
			{
				free(file_buf);
			}
			return TG_CODE(CFG_FILE_READ_FAILED);
		}
	}
	else
	{
		return TG_CODE(UNI_ALLOC_MEM_FAILED);
	}

	for (unsigned i = 0; i < file_size; ++i)
	{
		if (file_buf[i] != '\n')
		{
			if ((file_buf[i] == ' ' || file_buf[i] == '\t' || file_buf[i] == '\r') && !remark)
			{
				line_head = false;
				continue;
			}
			if ((file_buf[i] == COMMENT_MARK[0] && line_head) || remark)
			{
				if (file_buf[i] != '\r')
				{
					line_head = false;
					remark = true;
					conf_remark += file_buf[i];
				}
				continue;
			}

			if (file_buf[i] == '=' && !line_head)
			{
				make_value = true;
				continue;
			}

			if (make_value)
			{
				line_head = false;
				conf_value += file_buf[i];
			}
			else
			{
				line_head = false;
				conf_name += file_buf[i];
			}
		}
		else
		{
			if (remark)
			{
				conf_vec.push_back(std::make_pair(conf_remark, ""));
			}
			else
			{
				if (conf_name.empty())
				{
					conf_vec.push_back(std::make_pair("\r\n", ""));
					line_head = true;
					continue;
				}
				conf_vec.push_back(std::make_pair(conf_name, conf_value));
			}

			conf_remark.clear();
			conf_name.clear();
			conf_value.clear();
			make_value = false;
			remark = false;
			line_head = true;
		}
	}

	if (!conf_name.empty())
	{
		conf_vec.push_back(std::make_pair(conf_name, TrimSpace(conf_value)));
	}

	if (remark)
	{
		conf_vec.push_back(std::make_pair(conf_remark, ""));
	}

	free(file_buf);
	return TG_CODE(NO_PROBLEM);
}

inline TG_CODE Config::SaveConfFile()
{
	std::string str_conf;

	for (auto itr : conf_vec)
	{
		if (itr.first == "\r\n")
		{
			str_conf += itr.first;
			continue;
		}
		else if (itr.first.find(COMMENT_MARK) == 0)
		{
			str_conf += itr.first;
		}
		else
		{
			str_conf += TrimSpace(itr.first);
			str_conf += '=';
			str_conf += TrimSpace(itr.second);
		}

		str_conf += "\r\n";
	}

	while (str_conf.find('\r') == 0 || str_conf.find('\n') == 0)
	{
		str_conf = str_conf.substr(1, str_conf.length() - 1);
	}

	FILE * _file = fopen(conf_path.c_str(), "w+");

	if (_file == NULL)
	{
		return TG_CODE(UNI_INVALID_FP_VALUE);
	}

	if (!fwrite(str_conf.c_str(), str_conf.length(), 1, _file))
	{
		return TG_CODE(CFG_FILE_WRITE_FAILED);
	}

	fclose(_file);
	return TG_CODE(NO_PROBLEM);
}

inline std::string Config::TrimSpace(const std::string & str_data)
{
	std::string str_ret = str_data;
	while (IS_SPACE(str_ret))
	{
		str_ret = str_ret.substr(1, str_ret.length());
	}
	reverse(str_ret.begin(), str_ret.end());

	while (IS_SPACE(str_ret))
	{
		str_ret = str_ret.substr(1, str_ret.length());
	}
	reverse(str_ret.begin(), str_ret.end());

	return str_ret;
}

inline std::string Config::Get(const std::string & conf_name)
{
	if (conf_name.empty())
	{
		return "";
	}

	for (auto itr : conf_vec)
	{
		if (conf_name == itr.first)
		{
			return itr.second;
		}
	}

	return "";
}

std::string Config::Get(const std::string & conf_name, const std::string & default_value)
{
	if (conf_name.empty())
	{
		if (default_value.empty())
		{
			return std::string("");
		}
		
		return default_value;
	}

	for (auto itr : conf_vec)
	{
		if (conf_name == itr.first)
		{
			return itr.second;
		}
	}

	if (default_value.empty())
	{
		return std::string("");
	}

	return default_value;
}

inline TG_CODE Config::Get(const std::string & conf_name, std::string & conf_value)
{
	if (conf_name.empty())
	{
		return TG_CODE(CFG_KEY_VALUE_NULL);
	}

	for (auto itr : conf_vec)
	{
		if (conf_name == itr.first)
		{
			conf_value = itr.second;
			return TG_CODE(NO_PROBLEM);
		}
	}

	return TG_CODE(CFG_NOT_FOUND_VALUE);
}

inline TG_CODE Config::Add(const std::string & conf_name, const std::string & conf_value, const std::string & conf_remark)
{
	if (conf_name.empty())
	{
		return TG_CODE(CFG_KEY_VALUE_NULL);
	}

	if (!conf_remark.empty())
	{
		if (conf_remark.find(COMMENT_MARK) != 0)
		{
			std::string tmp = COMMENT_MARK;
			tmp += conf_remark;
			conf_vec.push_back(std::make_pair("\r\n", ""));
			conf_vec.push_back(std::make_pair(tmp, ""));
		}
	}

	if (conf_value.find(COMMENT_MARK) == 0)
	{
		conf_vec.push_back(std::make_pair(conf_name, ""));
	}

	conf_vec.push_back(std::make_pair(conf_name, conf_value));

	return SaveConfFile();
}

inline TG_CODE Config::Update(const std::string & conf_name, const std::string & conf_value, const std::string & conf_remark)
{
	if (conf_name.empty())
	{
		return TG_CODE(CFG_KEY_VALUE_NULL);
	}

	std::vector<std::pair<std::string, std::string>> tmp_vec;
	for (auto itr : conf_vec)
	{
		if (itr.first == conf_name)
		{
			if (!conf_remark.empty())
			{
				if (conf_remark.find(COMMENT_MARK) != 0)
				{
					std::string tmp = COMMENT_MARK;
					tmp += conf_remark;
					tmp_vec.push_back(std::make_pair("\r\n", ""));
					tmp_vec.push_back(std::make_pair(tmp, ""));
				}
			}
			tmp_vec.push_back(std::make_pair(conf_name, conf_value));
		}
		else
		{
			tmp_vec.push_back(itr);
		}
	}

	if (tmp_vec.size() >= conf_vec.size())
	{
		conf_vec.clear();
		conf_vec = tmp_vec;
	}

	return SaveConfFile();
}

inline TG_CODE Config::Delete(const std::string & conf_name)
{
	if (conf_name.empty())
	{
		return TG_CODE(CFG_KEY_VALUE_NULL);
	}

	for (auto itr = conf_vec.begin(); itr != conf_vec.end(); )
	{
		if (itr->first == conf_name)
		{
			itr = conf_vec.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	return SaveConfFile();
}

#endif // !CONFIG_H_

