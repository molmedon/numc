#pragma once

#include <vector>

namespace anita { namespace utils {

        ///
        /// \brief Clamp a std::vector to a range inplace
        ///
        template <typename T>
        void clamp(std::vector<T>& input_vec, const T min_value, const T max_value) {
            for (T& val : input_vec) {
                val = std::min(std::max(val, min_value), max_value);
            }
        }

        ///
        /// \brief Clamp a value to a range
        ///
        template <typename T>
        T clamp(const T val, const T min_value, const T max_value) {
            return std::min(std::max(val, min_value), max_value);
        }

    } // END: namespace utils
} // END: namespace anita
