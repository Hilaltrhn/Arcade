/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** DLLoader.hpp
*/
/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-arcade-11
** File description:
** DLLoader.hpp
*/

#ifndef DLLOADER_HPP
    #define DLLOADER_HPP

    #include <dlfcn.h>
    #include <functional>
    #include <filesystem>
    #include <stdexcept>
    #include <algorithm>
    #include <iostream>
    #include <chrono>
    #include <thread>
    #include <string>
    #include <memory>
    #include <vector>

namespace Arcade {
    template <typename T>
    class DLLoader {
    public:
        explicit DLLoader(const std::string &path) : _handle(nullptr) {
            _handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!_handle) {
                throw std::runtime_error("DLoader: Cannot load library: " + std::string(dlerror()));
            }
        }

        ~DLLoader() {
            if (_handle) {
                dlclose(_handle);
            }
        }

        DLLoader(const DLLoader &) = delete;
        DLLoader &operator=(const DLLoader &) = delete;

        T *getInstance(const std::string &symbol) const {
            return getInstance(std::vector<std::string>{symbol});
        }

        T *getInstance(const std::vector<std::string> &symbols) const {
            using CreatorFunc = T *(*)();

            for (const auto &symbol : symbols) {
                dlerror();
                void *ptr = dlsym(_handle, symbol.c_str());
                const char *error = dlerror();

                if (!error && ptr) {
                    return reinterpret_cast<CreatorFunc>(ptr)();
                }
            }

            std::string availableSymbols;
            for (std::size_t i = 0; i < symbols.size(); ++i) {
                availableSymbols += symbols[i];
                if (i + 1 < symbols.size()) {
                    availableSymbols += ", ";
                }
            }

            throw std::runtime_error("DLLoader Symbol Error: none of these symbols were found: " + availableSymbols);
        }

    private:
        void *_handle;
    };
}

#endif
