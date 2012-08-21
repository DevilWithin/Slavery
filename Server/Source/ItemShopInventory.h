#ifndef ItemShopInventory_h__
#define ItemShopInventory_h__

#include "ItemInformation.h"
#include <vector>

/// A list of items that make up the item store
class ItemShopInventory{
public:

	/// The items
	std::vector<ItemInfo> m_items;

};


#endif // ItemShopInventory_h__
