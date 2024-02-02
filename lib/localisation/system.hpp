#pragma once

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <filesystem>
#include <open-lib/DynamicLibrary.hpp>
#include <absl/container/flat_hash_map.h>

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
        absl::flat_hash_map<std::string, absl::flat_hash_map<std::string, std::string>> text;
        std::string str_now_language;
        absl::flat_hash_map<std::string, std::string>* now_language{nullptr};
        std::string str_default_language;
        absl::flat_hash_map<std::string, std::string>* default_language{nullptr};

        std::vector<open_lib::DL*> modules;

        virtual std::string read_all_loc_in_directory(std::filesystem::path path);
		
    public:
        virtual void load_from_directory(std::filesystem::path path);
        virtual void clear();

        virtual void set_now_language(std::string language);
        virtual void set_default_language(std::string language);

        virtual std::string get_text(std::string key);
        virtual std::string get_text(std::string key, std::string language);

        virtual std::vector<std::string> get_languages();

        virtual void set_modules(std::vector<std::string> paths);

        virtual ~System();
    };

    static System system;
}