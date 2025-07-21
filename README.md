# UE4SS - Enhanced QOL Fork

Personal enhanced fork of [UE4SS](https://github.com/Re-UE4SS/RE-UE4SS) with Quality of Life improvements.

## 🚀 Enhancements

**Console:**
- Right-click → Copy selected text  

**Live View:**  
- Object Type filter (Function, Class, Package, Enum, etc.) - specify which objects you want to search for in live view 
- Contains Property filter  - does obj contain a certain property name - substr search not equality 
- Has Any Properties filter (hide empty containers/type definitions)
- Enhanced property search - filter properties list for a specific property
- Sticky headers when scrolling properties

**Lua Global Functions:**
- `FindClassNamesContaining(searchTerm)` - Find classes containing search term

**UEnum Methods (Lua):**
- `enum:GetDisplayNameByValue(value)` - Get user-friendly enum name
- `enum:ForEachName(callback, includeFriendlyNames)` - Iterate with optional friendly names

**In Game Console:**
- Comprehensive `help` command with full API reference