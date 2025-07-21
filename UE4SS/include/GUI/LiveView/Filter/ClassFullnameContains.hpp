#pragma once

#include <GUI/LiveView/Filter/SearchFilter.hpp>
#include <Unreal/UClass.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::GUI::Filter
{
    class ClassFullnameContains
    {
      public:
        static inline StringType s_debug_name{STR("ClassFullnameContains")};
        static inline std::string s_internal_search_term{};

        static auto post_eval(UObject* object) -> bool
        {
            // If no search term, don't filter anything out
            if (s_internal_search_term.empty()) 
            {
                return false;
            }

            // Log once per search session
            static std::string last_logged_term;
            if (last_logged_term != s_internal_search_term)
            {
                Output::send(STR("Object path filter: Searching for '{}'\n"), ensure_str(s_internal_search_term));
                last_logged_term = s_internal_search_term;
            }

            if (!object)
            {
                return true; // Filter out null objects
            }

            // Get the object's own fullname (not the class type's fullname)
            std::string object_fullname = to_string(object->GetFullName());

            // Convert search term to lowercase for case-insensitive comparison
            std::string search_term_lower = s_internal_search_term;
            std::transform(search_term_lower.begin(), search_term_lower.end(), search_term_lower.begin(), [](char c) {
                return std::tolower(c);
            });

            // Convert object fullname to lowercase for case-insensitive comparison
            std::transform(object_fullname.begin(), object_fullname.end(), object_fullname.begin(), [](char c) {
                return std::tolower(c);
            });

            // Check if object fullname contains our search term
            if (object_fullname.find(search_term_lower) != std::string::npos)
            {
                return false; // Found a match - include this object
            }

            // No match - filter this object out
            return true;
        }
    };
} // namespace RC::GUI::Filter 