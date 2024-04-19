#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
 
namespace Utility {
    /**
     * @brief Construct a string tree from an array of strings, a head, and an array of depths.
     *
     * @tparam N Size of array.
     *
     * @param array     Array of tree content.
     * @param head      First item in the tree.
     * @param depths    Array of depths which indicies correspond to the array of tree content array[i]<->depths[i]
     * @param padding   Preceding whitespace of array elements (not the header).
     * @param align     Align items inside tree by delimiter
     * @param delimiter Delimiter for alignment
     *
     * @return std::string in tree format
     */
    template <size_t N>
    static std::string StringTree(const std::array<std::string, N>& array, std::string head, const size_t* depths, int padding = 0, bool align = false, char delimiter = ':') {
        static std::string tree_node = "|---";

        std::stringstream ss;
        size_t last = 0;
        size_t pre_delim_length = 0;
        std::string padding_string = "";

        if (padding > 0) { padding_string = std::string(padding, ' '); }

        ss << head << std::endl;

        if (align) {
            for (size_t o = 0; o < N; o++) {
                std::string elem = array[o];
                if (size_t pos = elem.find_first_of(delimiter); pos != std::string::npos) {
                    size_t offset = 1;
                    size_t incr = 0;

                    // Count spaces before the delimiter
                    while (pos - incr > 0 && elem.at(pos - incr - 1) == ' ') {
                        offset++;
                        incr++;
                    }

                    // Calculate length of substring before delimiter
                    size_t sub_str_size = elem.substr(0, pos - offset).size();

                    // Update maximum length if needed
                    if (sub_str_size > pre_delim_length) {
                        pre_delim_length = sub_str_size + 1;
                    }
                }
            }
        }

        pre_delim_length += padding;

        for (size_t i = 0; i < N; ++i) {
            ss << padding_string;

            for (size_t j = 0; j < depths[i]; ++j) {
                ss << tree_node;
            }

            if (align) {
                std::string elem = array[i];
                if (size_t pos = elem.find_first_of(delimiter); pos != std::string::npos) {
                    size_t expected_pos = pre_delim_length + 1; // Expected position after alignment
                    if (pos != expected_pos) {
                        // Calculate the number of spaces needed to align the delimiter
                        int num_spaces = static_cast<int>(expected_pos) - static_cast<int>(pos);

                        // Adjust the string to align the delimiter
                        if (num_spaces > 0) {
                            size_t curr_depth = tree_node.size() * depths[i];
                            if (num_spaces > curr_depth) {
                                num_spaces = num_spaces - (tree_node.size() * depths[i]);
                            } else {
                                num_spaces = (tree_node.size() * depths[i]) - num_spaces;
                            }

                            // Add spaces before the delimiter
                            elem.insert(pos, num_spaces, ' ');
                        } else {
                            // Remove excess spaces before the delimiter
                            elem.erase(pos + num_spaces, -num_spaces);
                        }
                    }
                }
                ss << elem << std::endl;
            } else {
                ss << array[i] << std::endl;
            }

            last = depths[i];
        }

        return ss.str();
    }
}