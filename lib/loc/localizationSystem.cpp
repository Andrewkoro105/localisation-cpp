#include "localizationSystem.hpp"

#include <fstream>
#include <algorithm>
#include <iostream>
#include "zip.h"

typedef unsigned uint;

namespace loc {
    bool findNotSpace(const char32_t &a) {
        bool r = a != U' ' && a != U'\n' && a != U'\t';
        return r;
    }

    bool findSpace(const char32_t &a) {
        return a == U' ' || a == U'\n' || a == U'\t';
    }

    bool findQuoteMarks(const char32_t &a){
        return a == U'"';
    }

    std::u32string::iterator searchQuoteMark(std::u32string::iterator first, std::u32string::iterator last){
        std::u32string::iterator result;
        while ((result = std::find_if(first, last, findQuoteMarks)) != last && *(result - 1) == U'\\');
        return result;
    }

    LocalizationNonexistentKeyException::LocalizationNonexistentKeyException(std::string nowLanguage, std::string defaultLanguage, std::string key) :
        nowLanguage(nowLanguage), defaultLanguage(defaultLanguage), key(key),
        str(std::string{"could not find the key '"} + key + std::string{"' in either the specified language '"} + nowLanguage + std::string{"' or the standard '"} + defaultLanguage + std::string{"'"}){}

    const char *LocalizationNonexistentKeyException::what() const noexcept {
        return str.c_str();
    }

    enum class FileType{
        file,
        dir,
        zip,
        notLoc
    };

    FileType isLoc(std::filesystem::path path){
        std::string strPath{path.string()};

        if (strPath.erase(0, strPath.size() - 8) == ".loc.zip")
            return FileType::zip;
        else if(path.extension() == ".loc")
            if (std::filesystem::is_directory(path))
                return FileType::dir;
            else
                return FileType::file;
        else
            return FileType::notLoc;

    }

    void readLocFile(std::filesystem::path path, std::u32string& files){
        std::u32string str{};
        std::basic_ifstream<char32_t> fin(path);
        std::getline(fin, str, U'\0');
        files += str + U'\n';
    }
#if _MSC_VER >= 1900
    std::u32string to_utf32(std::string const & s)
    {
        std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
        auto asInt = convert.from_bytes(s);
        return std::u32string(reinterpret_cast<char32_t const *>(asInt.data()), asInt.length());
    }

#else
    std::u32string to_utf32(std::string s){
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv{};
        return conv.from_bytes(s);
    }
#endif

    void readLocZip(std::filesystem::path path, std::u32string& files){
        int err = 0;
        zip *zipFile = zip_open(path.c_str(), 0, &err);

        zip_int64_t numberFile{zip_get_num_entries(zipFile, 0)};

        for (int i = 0; i < numberFile; ++i) {
            struct zip_stat st;
            zip_stat_init(&st);
            zip_stat_index(zipFile, i, 0, &st);
            std::string fileName{st.name};
            if (fileName.erase(0, fileName.size() - 4) == ".loc"){
                char * text = new char [st.size];

                zip_file_t* fileInZip = zip_fopen_index(zipFile, i, 0);
                zip_fread(fileInZip, text, st.size);
                zip_fclose(fileInZip);

                files += to_utf32(std::string{text}) + U'\n';
            }

        }
    }

    void readAllLocInDirectory(std::filesystem::path path, std::u32string& files){
        for (auto &languageFile: std::filesystem::directory_iterator(path)) {
            switch (isLoc(languageFile.path().string())) {
                case FileType::file:
                    readLocFile(languageFile.path(), files);
                    break;
                case FileType::dir:
                    readAllLocInDirectory(languageFile.path(), files);
                    break;
                case FileType::zip:
                    readLocZip(languageFile.path(), files);
                    break;
            }
        }
    }

    std::u32string readAllLocInDirectory(std::filesystem::path path){
        std::u32string result;
        readAllLocInDirectory(path, result);
        return result;
    }

    void LocalizationSystem::loadFromDirectory(std::filesystem::path path) {

        std::u32string files{readAllLocInDirectory(path)};

        std::string key;

        std::u32string::iterator cursor = files.begin();
        while ((cursor = std::find_if(cursor, files.end(), findNotSpace)) != files.end()){
            if (*cursor == U'"'){
                std::u32string::iterator endText{searchQuoteMark(cursor + 1, files.end())};
                (*nowLanguage)[key] = {cursor + 1, endText};
                cursor = endText + 1;
            } else{
                std::u32string::iterator endName{std::find_if(cursor, files.end(), findSpace)};
                if (*cursor == U'-'){
                    nowLanguage = &text[{cursor + 1, endName}];
                } else{
                    key = {cursor, endName};
                }
                cursor = endName;
            }
        }
    }

    void LocalizationSystem::clear() {
        text.clear();
        nowLanguage = nullptr;
        defaultLanguage = nullptr;
        strNowLanguage = "";
        strDefaultLanguage = "";
    }

    void LocalizationSystem::setNowLanguage(std::string language) {
        strNowLanguage = language;
        nowLanguage = &text[language];
    }

    void LocalizationSystem::setDefaultLanguage(std::string language) {
        strDefaultLanguage = language;
        defaultLanguage = &text[language];
    }

    std::u32string LocalizationSystem::getText(std::string key) {
        if (nowLanguage && nowLanguage->find(key) != nowLanguage->end())
            return (*nowLanguage)[key];
        else if (defaultLanguage && defaultLanguage->find(key) != defaultLanguage->end())
            return (*defaultLanguage)[key];
        else
            throw LocalizationNonexistentKeyException{strNowLanguage, strDefaultLanguage, key};
    }

    std::u32string LocalizationSystem::getText(std::string key, std::string language) {
        return LocalizationSystem::text[language][key];
    }

    std::vector<std::string> LocalizationSystem::getLanguages() {
        std::vector<std::string> result{};

        for (auto & pair : text) {
            result.push_back(pair.first);
        }

        return result;
    }
}