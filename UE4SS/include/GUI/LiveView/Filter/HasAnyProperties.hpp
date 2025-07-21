#pragma once

#include <GUI/LiveView/Filter/SearchFilter.hpp>
#include <Unreal/UClass.hpp>

namespace RC::GUI::Filter
{
    class HasAnyProperties
    {
      public:
        static inline StringType s_debug_name{STR("HasAnyProperties")};
        static inline bool s_enabled{};

        static auto post_eval(UObject* object) -> bool
        {
            // If filter is disabled, don't filter anything out
            if (!s_enabled) 
            {
                return false;
            }

            if (!object)
            {
                return true; // Filter out null objects
            }

            // First, filter out non-instances (UClass, UFunction, UPackage, etc.)
            // We only want actual object instances that have meaningful property data
            if (!is_instance(object))
            {
                return true; // Filter out type definitions and containers
            }

            // For instances, check if they have any properties by looking at their class definition
            auto object_class = object->GetClassPrivate();
            if (!object_class)
            {
                return true; // Filter out if we can't get the class
            }

            // Check if instances of this class have any properties
            for (const auto& property : object_class->ForEachPropertyInChain())
            {
                if (property)
                {
                    return false; // Class has properties, so instance has meaningful data - include it
                }
            }

            // Instance's class has no properties - filter this object out
            return true;
        }
    };
} // namespace RC::GUI::Filter 