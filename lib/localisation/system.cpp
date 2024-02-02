#include "system.hpp"

#include <fstream>
#include <algorithm>
#include <option_result/option_result.hpp>

typedef unsigned uint;

namespace loc {
	bool find_not_space(const char32_t& a) {
		bool r = a != U' ' && a != U'\n' && a != U'\t';
		return r;
	}
	
	bool find_space(const char32_t& a) {
		return a == U' ' || a == U'\n' || a == U'\t';
	}
	
	bool find_quote_marks(const char32_t& a) {
		return a == U'"';
	}
	
	std::string::iterator search_quote_mark(std::string::iterator first, std::string::iterator last) {
		std::string::iterator result;
		while((result = std::find_if(first, last, find_quote_marks)) != last && *(result - 1) == U'\\');
		return result;
	}
	
	LocalizationNonexistentKeyException::LocalizationNonexistentKeyException(std::string now_language, std::string default_language, std::string key) :
		now_language(now_language), default_language(default_language), key(key),
		str(std::string{"could not find the key '"} + key + std::string{"' in either the specified language '"} + now_language + std::string{"' or the standard '"} + default_language + std::string{"'"}) {
	}
	
	const char* LocalizationNonexistentKeyException::what() const noexcept {
		return str.c_str();
	}
	
	std::string read_loc_file(std::filesystem::path path) {
		std::string result{};
		std::basic_ifstream<char> fin(path);
		std::getline(fin, result, '\0');
		return result + '\n';
	}
	
	std::string System::read_all_loc_in_directory(std::filesystem::path path) {
		std::string result;
		for(auto& language_file: std::filesystem::directory_iterator(path)) {
			if(language_file.path().extension() == ".loc") {
				if(language_file.is_directory()) {
					result += read_all_loc_in_directory(language_file.path());
				} else {
					result += read_loc_file(language_file.path());
				}
			} else {
				for(auto& module: modules) {
					auto read = module->get_sim<std::string(*)(std::filesystem::path path)>("read");
					result += read(language_file.path());
				}
			}
		}
		return result;
	}
	
	void System::load_from_directory(std::filesystem::path path) {
		std::string files{read_all_loc_in_directory(path)};
		
		std::string key;
		
		std::string::iterator cursor = files.begin();
		while((cursor = std::find_if(cursor, files.end(), find_not_space)) != files.end()) {
			if(*cursor == '"') {
				std::string::iterator end_text{search_quote_mark(cursor + 1, files.end())};
				(*now_language)[key] = {cursor + 1, end_text};
				cursor = end_text + 1;
			} else {
				std::string::iterator end_name{std::find_if(cursor, files.end(), find_space)};
				if(*cursor == '-') {
					now_language = &text[{cursor + 1, end_name}];
				} else {
					key = {cursor, end_name};
				}
				cursor = end_name;
			}
		}
	}
	
	void System::clear() {
		text.clear();
		now_language = nullptr;
		default_language = nullptr;
		str_now_language = "";
		str_default_language = "";
	}
	
	void System::set_now_language(std::string language) {
		str_now_language = language;
		now_language = &text[language];
	}
	
	void System::set_default_language(std::string language) {
		str_default_language = language;
		default_language = &text[language];
	}
	
	std::string System::get_text(std::string key) {
		if(now_language && now_language->find(key) != now_language->end())
			return (*now_language)[key];
		else if(default_language && default_language->find(key) != default_language->end())
			return (*default_language)[key];
		else
			throw LocalizationNonexistentKeyException{str_now_language, str_default_language, key};
	}
	
	std::string System::get_text(std::string key, std::string language) {
		return System::text[language][key];
	}
	
	std::vector<std::string> System::get_languages() {
		std::vector<std::string> result{};
		
		for(auto& pair: text) {
			result.push_back(pair.first);
		}
		
		return result;
	}
	
	void System::set_modules(std::vector<std::string> paths) {
		for(auto path: paths) {
			modules.push_back(new open_lib::DL{path});
		}
	}
	
	System::~System() {
		for(auto& module: modules) {
			delete module;
		}
	}
	
}