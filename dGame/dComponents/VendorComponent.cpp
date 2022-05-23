#include "VendorComponent.h"

#include <BitStream.h>

#include "Game.h"
#include "dServer.h"

VendorComponent::VendorComponent(Entity* parent) : Component(parent) {
	SetupConstants();
	RefreshInventory(true);
}

VendorComponent::~VendorComponent() = default;

void VendorComponent::Serialize(RakNet::BitStream* outBitStream, bool bIsInitialUpdate, unsigned int& flags) {
	outBitStream->Write1(); 
	outBitStream->Write1(); // Has standard items (Required for vendors with missions.)
	outBitStream->Write(HasCraftingStation()); // Has multi use items 
}

void VendorComponent::OnUse(Entity* originator) {
	GameMessages::SendVendorOpenWindow(m_Parent, originator->GetSystemAddress());
	GameMessages::SendVendorStatusUpdate(m_Parent, originator->GetSystemAddress());
}

float VendorComponent::GetBuyScalar() const {
	return m_BuyScalar;
}

float VendorComponent::GetSellScalar() const {
	return m_SellScalar;
}

void VendorComponent::SetBuyScalar(float value) {
	m_BuyScalar = value;
}

void VendorComponent::SetSellScalar(float value) {
	m_SellScalar = value;
}

std::map<LOT, int>& VendorComponent::GetInventory() {
	return m_Inventory;
}

bool VendorComponent::HasCraftingStation() {
	// As far as we know, only Umami has a crafting station
	return m_Parent->GetLOT() == 13800;
}

void VendorComponent::RefreshInventory(bool isCreation) {
	//Custom code for Max vanity NPC
	if (m_Parent->GetLOT() == 9749 && Game::server->GetZoneID() == 1201) {
		if (!isCreation) return;
		m_Inventory.insert({11909, 0}); //Top hat w frog
		m_Inventory.insert({7785, 0}); //Flash bulb
		m_Inventory.insert({12764, 0}); //Big fountain soda
		m_Inventory.insert({12241, 0}); //Hot cocoa (from fb)
		return;
	}
	m_Inventory.clear();
	auto* lootMatrixTable = CDClientManager::Instance()->GetTable<CDLootMatrixTable>("LootMatrix");
	std::vector<CDLootMatrix> lootMatrices = lootMatrixTable->Query([=](CDLootMatrix entry) { return (entry.LootMatrixIndex == m_LootMatrixID); });

	if (lootMatrices.empty()) return;
	// Done with lootMatrix table

	auto* lootTableTable = CDClientManager::Instance()->GetTable<CDLootTableTable>("LootTable");

	for (const auto& lootMatrix : lootMatrices) {
		int lootTableID = lootMatrix.LootTableIndex;
		std::vector<CDLootTable> vendorItems = lootTableTable->Query([=](CDLootTable entry) { return (entry.LootTableIndex == lootTableID); });
		if (lootMatrix.maxToDrop == 0 || lootMatrix.minToDrop == 0) {
			for (CDLootTable item : vendorItems) {
				m_Inventory.insert({item.itemid, item.sortPriority});
			}
		} else {
			auto randomCount = GeneralUtils::GenerateRandomNumber<int32_t>(lootMatrix.minToDrop, lootMatrix.maxToDrop);

			for (size_t i = 0; i < randomCount; i++) {
				if (vendorItems.empty()) break;

				auto randomItemIndex = GeneralUtils::GenerateRandomNumber<int32_t>(0, vendorItems.size() - 1);

				const auto& randomItem = vendorItems[randomItemIndex];

				vendorItems.erase(vendorItems.begin() + randomItemIndex);

				m_Inventory.insert({randomItem.itemid, randomItem.sortPriority});
			}
		}
	}

	//Because I want a vendor to sell these cameras
	if (m_Parent->GetLOT() == 13569) {
		auto randomCamera = GeneralUtils::GenerateRandomNumber<int32_t>(0, 2);

		switch (randomCamera) {
			case 0:
				m_Inventory.insert({16253, 0}); //Grungagroid
				break;
			case 1:
				m_Inventory.insert({16254, 0}); //Hipstabrick
				break;
			case 2:
				m_Inventory.insert({16204, 0}); //Megabrixel snapshot
				break;
			default:
				break;
		}
	}

	// Callback timer to refresh this inventory.
	m_Parent->AddCallbackTimer(m_RefreshTimeSeconds, [this]() {
		RefreshInventory();
	});


	//CALDERA MAR NPC - Wrynn - The Accursed (Venture Explorer)(Normal Stromling)
	if (m_Parent->GetLOT() == 4712 && Game::server->GetZoneID() == 1000) {
		m_Inventory.clear();
	}

	//CALDERA MAR NPC - Wrynn - The Accursed (Nimbus Station)(Normal Stromling)
	if (m_Parent->GetLOT() == 4712 && Game::server->GetZoneID() == 1200) {
		m_Inventory.clear();
	}

	//CALDERA MAR NPC - Izumi Moonstone (Ninjago Monastery Caves)
	if (m_Parent->GetLOT() == 9706 && Game::server->GetZoneID() == 2000) {
		m_Inventory.clear();
		m_Inventory.insert({14090, 0}); //Earth Armor Weapon
		m_Inventory.insert({14135, 0}); //Earth Armor Shoulderpads
		m_Inventory.insert({14140, 0}); //Earth Armor Helmet
		m_Inventory.insert({14170, 0}); //Earth Armor Shirt
		m_Inventory.insert({14171, 0}); //Earth Armor Pants
		m_Inventory.insert({14548, 0}); //Earth Armor Weapon
		m_Inventory.insert({14721, 0}); //Nya's Golden Daggers
		m_Inventory.insert({14824, 0}); //Double Hammers
		m_Inventory.insert({15918, 0}); //Athlete's Shade
		m_Inventory.insert({16089, 0}); //White Silk Kimono
		m_Inventory.insert({16090, 0}); //Red Silk Kimono
		m_Inventory.insert({16091, 0}); //Pink Silk Kimono
		m_Inventory.insert({16092, 0}); //Black Silk Kimono
		m_Inventory.insert({16123, 0}); //Murasaki Hakama
		m_Inventory.insert({16125, 0}); //Black Hakama Pants
		m_Inventory.insert({16126, 0}); //Red Hakama Pants
		m_Inventory.insert({16127, 0}); //Pink Hakama Pants
		m_Inventory.insert({16129, 0}); //Grey Hakama Pants
		m_Inventory.insert({16648, 0}); //Kawaii Umbrella
		m_Inventory.insert({16658, 0}); //Ninjago Earth Car
		m_Inventory.insert({16659, 0}); //Ninjago Earth Car
		m_Inventory.insert({16660, 0}); //Ninjago Earth Car
		m_Inventory.insert({16661, 0}); //Ninjago Earth Car
		m_Inventory.insert({16662, 0}); //Ninjago Earth Car
		m_Inventory.insert({16665, 0}); //Ninjago Lightning Car
		m_Inventory.insert({16666, 0}); //Ninjago Lightning Car
		m_Inventory.insert({16667, 0}); //Ninjago Lightning Car
		m_Inventory.insert({16668, 0}); //Ninjago Lightning Car
		m_Inventory.insert({16669, 0}); //Ninjago Lightning Car
		m_Inventory.insert({16670, 0}); //Ninjago Ice Car
		m_Inventory.insert({16671, 0}); //Ninjago Ice Car
		m_Inventory.insert({16672, 0}); //Ninjago Ice Car
		m_Inventory.insert({16673, 0}); //Ninjago Ice Car
		m_Inventory.insert({16674, 0}); //Ninjago Ice Car
		m_Inventory.insert({16675, 0}); //Ninjago Fire Car
		m_Inventory.insert({16676, 0}); //Ninjago Fire Car
		m_Inventory.insert({16677, 0}); //Ninjago Fire Car
		m_Inventory.insert({16678, 0}); //Ninjago Fire Car
		m_Inventory.insert({16679, 0}); //Ninjago Fire Car
		m_Inventory.insert({16697, 0}); //Ice Ninjago Rocket Part 1
		m_Inventory.insert({16698, 0}); //Ice Ninjago Rocket Part 2
		m_Inventory.insert({16699, 0}); //Ice Ninjago Rocket Part 3
		m_Inventory.insert({16700, 0}); //Earth Ninjago Rocket Part 1
		m_Inventory.insert({16701, 0}); //Earth Ninjago Rocket Part 2
		m_Inventory.insert({16702, 0}); //Earth Ninjago Rocket Part 3
		m_Inventory.insert({16703, 0}); //Fire Ninjago Rocket Part 1
		m_Inventory.insert({16704, 0}); //Fire Ninjago Rocket Part 2
		m_Inventory.insert({16705, 0}); //Fire Ninjago Rocket Part 3
		m_Inventory.insert({16706, 0}); //Lightning Ninjago Rocket Part 1
		m_Inventory.insert({16707, 0}); //Lightning Ninjago Rocket Part 2
		m_Inventory.insert({16708, 0}); //Lightning Ninjago Rocket Part 3
	}

	//CALDERA MAR NPC - Del (Nexus Tower)
	if (m_Parent->GetLOT() == 1867 && Game::server->GetZoneID() == 1900) {
		m_Inventory.clear();
		m_Inventory.insert({7791, 0}); //Blue Sun Headdress
		m_Inventory.insert({9869, 0}); //Vampire Nexus Force Cape
		m_Inventory.insert({9870, 0}); //Vampire Blue Cape
		m_Inventory.insert({9871, 0}); //Vampire Sentinels Cape
		m_Inventory.insert({9872, 0}); //Vampire Assembly Cape
		m_Inventory.insert({9873, 0}); //Vampire Venture League Cape
		m_Inventory.insert({9874, 0}); //Vampire Paradox Cape
		m_Inventory.insert({12512, 0}); //Cool Hat
		m_Inventory.insert({13740, 0}); //Stromling Disguise
		m_Inventory.insert({13742, 0}); //Ronin Disguise
		m_Inventory.insert({14095, 0}); //Racemaster
		m_Inventory.insert({14096, 0}); //Race Ace
		m_Inventory.insert({14097, 0}); //Master Driver
		m_Inventory.insert({14098, 0}); //Race Ace
		m_Inventory.insert({14099, 0}); //Racemaster
		m_Inventory.insert({14100, 0}); //Master Driver
		m_Inventory.insert({14101, 0}); //Bricksel Adams Shirt
		m_Inventory.insert({14102, 0}); //Photojournalist Shirt
		m_Inventory.insert({14103, 0}); //Paparazzi Shirt
		m_Inventory.insert({14104, 0}); //Survival Mech Shirt
		m_Inventory.insert({14105, 0}); //Spiderling Shirt
		m_Inventory.insert({14106, 0}); //Stromling Shirt
		m_Inventory.insert({14116, 0}); //4 Riders Shirt
		m_Inventory.insert({14117, 0}); //Kinga Hurl Shirt
		m_Inventory.insert({14118, 0}); //Murgle Blotch Shirt
		m_Inventory.insert({14119, 0}); //Nexus Force Shield MKIII
		m_Inventory.insert({14120, 0}); //Nexus Force Shield MKII
		m_Inventory.insert({14121, 0}); //Nexus Force Shield MKI
		m_Inventory.insert({14802, 0}); //Brown Wolf Head Helmet
		m_Inventory.insert({15977, 0}); //Shark Rocket
		m_Inventory.insert({15978, 0}); //Pencil Rocket
		m_Inventory.insert({15979, 0}); //Duck Rocket
	}

	//CALDERA MAR NPC - Jesse Studd (Nexus Tower)
	if (m_Parent->GetLOT() == 3921 && Game::server->GetZoneID() == 1900) {
		m_Inventory.clear();
		m_Inventory.insert({12220, 0}); //Pod Captain 1 Pack
		m_Inventory.insert({12221, 0}); //Pod Captain 2 Pack
		m_Inventory.insert({12222, 0}); //Pod Captain 3 Pack
		m_Inventory.insert({12223, 0}); //Pod Captain 4 Pack
		m_Inventory.insert({12224, 0}); //Pod Captain 5 Pack
		m_Inventory.insert({12225, 0}); //Pod Captain 6 Pack
		m_Inventory.insert({12226, 0}); //Pod Captain 7 Pack
		m_Inventory.insert({14191, 0}); //Green Frog
		m_Inventory.insert({14192, 0}); //White Cat
		m_Inventory.insert({14193, 0}); //Light Gray Mouse
		m_Inventory.insert({14194, 0}); //Brown Dog
		m_Inventory.insert({14559, 0}); //Dark Gray Frog
		m_Inventory.insert({14560, 0}); //Red Frog
		m_Inventory.insert({14561, 0}); //Yellow Frog
		m_Inventory.insert({14562, 0}); //Black Cat
		m_Inventory.insert({14563, 0}); //Tan Cat
		m_Inventory.insert({14564, 0}); //Orange Cat
		m_Inventory.insert({14565, 0}); //White Mouse
		m_Inventory.insert({14566, 0}); //Brown Mouse
		m_Inventory.insert({14567, 0}); //Dark Gray Mouse
		m_Inventory.insert({14568, 0}); //Dark Gray Dog
		m_Inventory.insert({14569, 0}); //Red Dog
		m_Inventory.insert({14570, 0}); //Reddish Brown Dog
		m_Inventory.insert({14571, 0}); //Red Scorpion
		m_Inventory.insert({14744, 0}); //Racemaster Car
		m_Inventory.insert({14745, 0}); //Racemaster Car
		m_Inventory.insert({14746, 0}); //Racemaster Car
		m_Inventory.insert({14747, 0}); //Racemaster Car
		m_Inventory.insert({14748, 0}); //Racemaster Car
		m_Inventory.insert({14749, 0}); //Race Ace Car
		m_Inventory.insert({14750, 0}); //Race Ace Car
		m_Inventory.insert({14751, 0}); //Race Ace Car
		m_Inventory.insert({14752, 0}); //Race Ace Car
		m_Inventory.insert({14753, 0}); //Race Ace Car
		m_Inventory.insert({14754, 0}); //Master Driver Car
		m_Inventory.insert({14755, 0}); //Master Driver Car
		m_Inventory.insert({14756, 0}); //Master Driver Car
		m_Inventory.insert({14757, 0}); //Master Driver Car
		m_Inventory.insert({14758, 0}); //Master Driver Car
		m_Inventory.insert({15976, 0}); //Duck Car Pack
		m_Inventory.insert({15980, 0}); //Dune Buggy Pack
	}

	//CALDERA MAR NPC - Moonbeam (Ninjago Monastery)
	if (m_Parent->GetLOT() == 1904 && Game::server->GetZoneID() == 2000) {
		m_Inventory.clear();
	}

	//CALDERA MAR NPC - Silas Penumbra (Nexus Tower)
	if (m_Parent->GetLOT() == 9706 && Game::server->GetZoneID() == 1900) {
		m_Inventory.clear();
	}
	GameMessages::SendVendorStatusUpdate(m_Parent, UNASSIGNED_SYSTEM_ADDRESS);
}

void VendorComponent::SetupConstants() {
	auto* compRegistryTable = CDClientManager::Instance()->GetTable<CDComponentsRegistryTable>("ComponentsRegistry");
	int componentID = compRegistryTable->GetByIDAndType(m_Parent->GetLOT(), COMPONENT_TYPE_VENDOR);

	auto* vendorComponentTable = CDClientManager::Instance()->GetTable<CDVendorComponentTable>("VendorComponent");
	std::vector<CDVendorComponent> vendorComps = vendorComponentTable->Query([=](CDVendorComponent entry) { return (entry.id == componentID); });
	if (vendorComps.empty()) return;
	m_BuyScalar = vendorComps[0].buyScalar;
	m_SellScalar = vendorComps[0].sellScalar;
	m_RefreshTimeSeconds = vendorComps[0].refreshTimeSeconds;
	m_LootMatrixID = vendorComps[0].LootMatrixIndex;
}