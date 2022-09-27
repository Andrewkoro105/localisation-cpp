#pragma once

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <filesystem>

namespace loc {

    class LocalizationNonexistentKeyException : public std::exception{
    protected:
        std::string str;
        std::string nowLanguage;
        std::string defaultLanguage;
        std::string key;

    public:
        LocalizationNonexistentKeyException(std::string nowLanguage, std::string defaultLanguage, std::string key);
        const char* what() const noexcept override;
    };

    class LocalizationSystem {
    protected:
        std::map<std::string, std::map<std::string, std::u32string>> text;
        std::string strNowLanguage;
        std::map<std::string, std::u32string>* nowLanguage{nullptr};
        std::string strDefaultLanguage;
        std::map<std::string, std::u32string>* defaultLanguage{nullptr};

    public:
        void loadFromDirectory(std::filesystem::path path);
        void clear();

        void setNowLanguage(std::string language);
        void setDefaultLanguage(std::string language);

        std::u32string getText(std::string key);
        std::u32string getText(std::string key, std::string language);

        std::vector<std::string> getLanguages();
    };

    static LocalizationSystem system;
} // ui