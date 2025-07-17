#pragma once

#include <GUI/LiveView/Filter/SearchFilter.hpp>
#include <Unreal/UObject.hpp>
#include <algorithm>
#include <cctype>

namespace RC::GUI::Filter
{
    class ObjectTypeFilter
    {
      public:
        static inline StringType s_debug_name{STR("ObjectTypeFilter")};
        static inline std::string s_internal_object_type{};

        static auto post_eval(UObject* object) -> bool
        {
            // If no search term, don't filter anything out
            if (s_internal_object_type.empty()) 
            {
                return false;
            }

            // Log once per search session
            static std::string last_logged_term;
            if (last_logged_term != s_internal_object_type)
            {
                Output::send(STR("Object type filter: Search started\n"));
                last_logged_term = s_internal_object_type;
            }

            if (!object)
            {
                return true; // Filter out null objects
            }

            // Get the object's full name (e.g., "Function /Script/GameplayTasks.GameplayTask:ReadyForActivation")
            auto object_full_name = to_string(object->GetFullName());
            
            // Extract the object type (first word before the space)
            std::string object_type;
            size_t space_pos = object_full_name.find(' ');
            if (space_pos != std::string::npos)
            {
                object_type = object_full_name.substr(0, space_pos);
            }
            else
            {
                object_type = object_full_name; // If no space, use the entire name
            }
            
            // Convert both to lowercase for case-insensitive comparison
            std::string search_term = s_internal_object_type;
            std::transform(search_term.begin(), search_term.end(), search_term.begin(), [](char c) {
                return std::tolower(c);
            });
            std::transform(object_type.begin(), object_type.end(), object_type.begin(), [](char c) {
                return std::tolower(c);
            });
            
            // Check for exact equality of object type
            if (object_type == search_term)
            {
                return false; // Found exact match - include this object
            }

            // No match - filter this object out
            return true;
        }
    };
} // namespace RC::GUI::Filter 