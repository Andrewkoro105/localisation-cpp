#pragma once

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <filesystem>
#include <open-lib/DynamicLibrary.hpp>

namespace loc {

    class LocalizationNonexistentKeyException : public std::exception{
    protected:
        std::string str;
        std::string now_language;
        std::string default_language;
        std::string key;

    public:
        LocalizationNonexistentKeyException(std::string now_language, std::string default_language, std::string key);
        const char* what() const noexcept override;
    };

    class System {
    protected:
        std::map<std::string, std::map<std::string, std::u32string>> text;
        std::string str_now_language;
        std::map<std::string, std::u32string>* now_language{nullptr};
        std::string str_default_language;
        std::map<std::string, std::u32string>* default_language{nullptr};

        std::vector<open_lib::DL*> modules;

        virtual std::u32string read_all_loc_in_directory(std::filesystem::path path);
        virtual void read_all_loc_in_directory(std::filesystem::path path, std::u32string& files);
    public:
        virtual void load_from_directory(std::filesystem::path path);
        virtual void clear();

        virtual void set_now_language(std::string language);
        virtual void set_default_language(std::string language);

        virtual std::u32string get_text(std::string key);
        virtual std::u32string get_text(std::string key, std::string language);

        virtual std::vector<std::string> get_languages();

        virtual void set_modules(std::vector<std::string> paths);

        virtual ~System();
    };

    static System system;
} // ui