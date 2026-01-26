# TODO:
### Features (high priority):
- **Deniel:**
    - [ ] Make the window resizable and add fullscreen mode (please make a separate branch for this)
    - [X] Create ```Item``` class which should contain following attributes (note: this class is purely for holding INFORMATION about an item; if an item needs to be shown in the GUI, use ```GUIItem``` as described in the section below):
        - [X] ```bool canDealMeleeDamage``` (true if the item is a close-range weapon like a knife)
        - [X] ```bool canDealRangedDamage``` (true if the item can fire bullets)
        - [X] ```bool isHealthPack``` (true if the item can be used to heal the player)
        - [X] ```int damage``` (Note: if ```isHealthPack``` is true, this attribute will be used to determine how much health the player gains)
        - [X] ```float useRate``` (how many times the item can be used per second)
        - [X] ```sf::Texture texture``` (texture to use when displaying the item)
    - [ ] Inventory (similar to Minecraft):
        - [ ] Create a class ```GUIItem``` for an item that should have an item texture and can be dragged in and out of an **inventory slot** using the mouse. This class should be used **exclusively** for displaying items on the GUI.
        - [ ] Create a class ```InventorySlot``` for a **SINGLE** square-shaped inventory slot where an item can be placed in and out of. We need to be able to place ```InventorySlot``` instances **anywhere** on the screen (e.g. ```InventorySlot``` should have a ```sf::Vector2f position``` attribute). The slot should be highligthed when the mouse hovers over it.
        - [ ] Create a class ```Inventory``` that should contain a matrix (with a given width and height) of ```InventorySlot```s.
    - [ ] Loot system:
        - [ ] Create a class ```LootContainer``` that can contain items. The loot container should have a ```float range``` attribute that dictates the minimum distance the player needs to be away from the container to open it. Once the player is withing range of the container, the container should be highlighted (for now: use rectangle sprite and change color to highlight) and the player should be able to press Q to open it. Once opened, the GUI should display the container's and the player's inventories (like in Minecraft) so that the player can drag items from the container into their inventory.
    - [ ] Tilemap:
        - [ ] Make it possible to save a large tile map as a single file and load chunks from that file. Note: it's possible to load parts from a file by specifying the starting index and how many bytes you want to read. If you save a chunk from a tilemap as a string of bytes where each byte represents the ID of the tile, you should be able to implement the feature.
        - [ ] Make it possible to load tile textures from a spritesheet (e.g. a single image that contains every tile). Note: this is lower priority since we don't have any actual tile textures yet
- **Maks:**
    - [ ] Create a class ```Building``` where a player can enter and exit. Entering the building will teleport the player to a different map with the building's layout. If any zombies were chasing the player before they entered the building, they should have a chance to enter the building as well.
    - [ ] Hitboxes, walls and collision detection
    - [ ] Particle system (blood)
    - [ ] Gun recoil animation
    - [ ] Knockback?

### Extra (low priority):
- **Deniel:**
    - [ ] Create a tool for making custom tile maps (or maybe find an existing one?)
    - [ ] Sounds and music
- **Maks:**
    - [ ] Pixel art