# TODO:
### Features (high priority):

- **Deniel:**
    - [ ] Loot system:
        - [X] Create a class ```LootContainer``` that can contain items. The loot container should have a ```float range``` attribute that dictates the minimum distance the player needs to be away from the container to open it. Once the player is withing range of the container, the container should be highlighted (for now: use rectangle sprite and change color to highlight) and the player should be able to press Q to open it. Once opened, the GUI should display the container's and the player's inventories (like in Minecraft) so that the player can drag items from the container into their inventory.
        - [ ] When the player is close to the ```LootContainer```, a sprite with the ```q_key_prompt.png``` texture should appear and hover over the box so that the player knows to press Q to open it.
    - [ ] Tilemap:
        - [ ] Make it possible to save a large tile map as a single file and load chunks from that file. Note: it's possible to load parts from a file by specifying the starting index and how many bytes you want to read. If you save a chunk from a tilemap as a string of bytes where each byte represents the ID of the tile, you should be able to implement the feature.
        - [ ] Make it possible to load tile textures from a spritesheet (e.g. a single image that contains every tile). Note: this is lower priority since we don't have any actual tile textures yet
- **Maks:**
    - [ ] Create a class ```Building``` where a player can enter and exit. Entering the building will teleport the player to a different map with the building's layout. If any zombies were chasing the player before they entered the building, they should have a chance to enter the building as well.
    - [X] Hitboxes
    - [ ] Wall collisions
    - [X] Particle system (blood)
    - [ ] Gun recoil animation
    - [X] Knockback

### Extra (low priority):
- **Deniel:**
    - [ ] Create a tool for making custom tile maps (or maybe find an existing one?)
    - [ ] Sounds and music
- **Maks:**
    - [ ] Pixel art