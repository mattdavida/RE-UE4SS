#pragma once

#include <GUI/LiveView/Filter/SearchFilter.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

namespace RC::GUI::Filter
{
    class HidePreClearObjects
    {
      public:
        static inline StringType s_debug_name{STR("HidePreClearObjects")};
        static inline bool s_enabled{false};
        static inline int32_t s_clear_object_count{0};

        static auto post_eval(UObject* object) -> bool
        {
            // If clear mode is disabled, don't filter anything out
            if (!s_enabled) 
            {
                return false;
            }

            if (!object)
            {
                return true; // Filter out null objects
            }

            // Get the object's index in the UObjectArray
            // Objects created before "clear" was pressed have lower indices
            auto object_item = object->GetObjectItem();
            if (!object_item)
            {
                return true; // Filter out if we can't get object item
            }

            // Use the object's serial number to determine if it existed before clear
            // Objects with serial numbers >= s_clear_object_count were created after clear
            auto object_serial_number = Unreal::Container::UnrealVC->FUObjectItem_get_serial_number(object_item);
            
            // Hide objects that existed before clear (serial numbers <= threshold)
            return object_serial_number <= s_clear_object_count;
        }

        // Called when "Clear" button is pressed
        static auto enable_clear_mode() -> void
        {
            // Find the highest serial number currently in use
            int32_t max_serial = 0;
            for (size_t i = 0; i < Unreal::UObjectArray::GetNumElements(); i++)
            {
                if (auto* obj_item = static_cast<FUObjectItem*>(Unreal::Container::UnrealVC->UObjectArray_index_to_object(i)))
                {
                    if (!obj_item->IsUnreachable())
                    {
                        auto serial = Unreal::Container::UnrealVC->FUObjectItem_get_serial_number(obj_item);
                        if (serial > max_serial)
                        {
                            max_serial = serial;
                        }
                    }
                }
            }
            
            s_clear_object_count = max_serial;
            s_enabled = true;
        }

        // Called when "Restore" button is pressed  
        static auto disable_clear_mode() -> void
        {
            s_enabled = false;
        }

        // Get status for UI display
        static auto is_clear_mode_enabled() -> bool
        {
            return s_enabled;
        }

        static auto get_clear_count() -> int32_t
        {
            return s_clear_object_count;
        }
    };
} // namespace RC::GUI::Filter 