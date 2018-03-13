#pragma once

#include <tuple>
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


        ///
        /// \brief Perform 2D quadratic (bilinear) interpolation
        ///
        /// This uses bilinear interpolation to interpolate a 2D function
        /// between `xvals`, and `yvals`, at a location `val` where fvals
        /// contains the value of the function at the four points.
        /// Each of `xvals` and `yvals` are length 2 arrays that contain
        /// the `x` and `y` location of the points.
        /// fvals contains the value of the function at each of the points clockwise
        /// from the bottom left.
        ///
        /// Only defined on `float` or `double`
        ///
        /// NOTE: THIS HAS NOT BEEN TESTED THOROUGHLY. PLEASE PERFORM MORE TESTING BEFORE USE.
        /// CURRENTLY UNUSED.
        ///
        template <typename T>
        T interp2D(const std::pair<T, T> xlocs, const std::pair<T, T> ylocs, const std::tuple<T, T, T, T> fvals, const std::pair<T, T> loc) {

            // we use the polynomial method from the Wikipedia page for bilinear interpolation
            const T x = loc.first;     const T y = loc.second;
            const T x1 = xlocs.first;  const T x2 = xlocs.second;
            const T y1 = ylocs.first;  const T y2 = ylocs.second;
            const T f11 = std::get<0>(fvals); const T f12 = std::get<1>(fvals);
            const T f22 = std::get<3>(fvals); const T f21 = std::get<2>(fvals);

            // compute polynomial coefficients
            const T a0 = (f11*x2*y2)/((x1 - x2)*(y1 - y2)) + (f12*x2*y1)/((x1 -x2)*(y2 - y1)) + (f21*x1*y2)/((x1 - x2)*(y2 - y1)) + (f22*x1*y1)/((x1 - x2)*(y1 - y2));
            const T a1 = (f11*y2)/((x1 - x2)*(y2 - y1)) + (f12*y1)/((x1 - x2)*(y1 - y2)) + (f21*y2)/((x1 - x2)*(y1 - y2)) + (f22*y1)/((x1 - x2)*(y2 - y1));
            const T a2 = (f11*x2)/((x1 - x2)*(y2 - y1)) + (f12*x2)/((x1 - x2)*(y1 - y2)) + (f21*x1)/((x1 - x2)*(y1 - y2)) + (f22*x1)/((x1 - x2)*(y2 - y1));
            const T a3 = f11/((x1 - x2)*(y1 - y2)) + f12/((x1 - x2)*(y2 - y1)) + f21/((x1 - x2)*(y2 - y1)) + f22/((x1 - x2)*(y1 - y2));

            // and return the evaluated polynomial
            return a0 + a1*x + a2*y + a3*x*y;

        }

    } // END: namespace utils
} // END: namespace anita
