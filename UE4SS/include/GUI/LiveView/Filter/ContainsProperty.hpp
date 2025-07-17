#pragma once

#include <GUI/LiveView/Filter/SearchFilter.hpp>
#include <Unreal/UClass.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::GUI::Filter
{
    class ContainsProperty
    {
      public:
        static inline StringType s_debug_name{STR("ContainsProperty")};
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
                Output::send(STR("Contains property: Search started\n"));
                last_logged_term = s_internal_search_term;
            }

            if (!object)
            {
                return true; // Filter out null objects
            }

            // Get the struct (class) to iterate properties
            auto as_struct = Cast<UStruct>(object);
            if (!as_struct)
            {
                as_struct = object->GetClassPrivate();
            }

            if (!as_struct)
            {
                return true; // Filter out if we can't get the struct
            }

            // Convert search term to lowercase for case-insensitive comparison
            std::string search_term_lower = s_internal_search_term;
            std::transform(search_term_lower.begin(), search_term_lower.end(), search_term_lower.begin(), [](char c) {
                return std::tolower(c);
            });

            // Iterate through all properties and check if any contain our search term
            for (const auto& property : as_struct->ForEachPropertyInChain())
            {
                if (!property) continue;

                // Get property name and convert to lowercase
                std::string property_name = to_string(property->GetName());
                std::transform(property_name.begin(), property_name.end(), property_name.begin(), [](char c) {
                    return std::tolower(c);
                });

                // Check if property name contains our search term
                if (property_name.find(search_term_lower) != std::string::npos)
                {
                    return false; // Found a match - include this object
                }
            }

            // No properties matched - filter this object out
            return true;
        }
    };
} // namespace RC::GUI::Filter 