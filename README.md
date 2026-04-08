# TODO:
### Features (high priority):

- **Deniel:**
    - [ ] Tilemap:
        - [ ] Make it possible to save a large tile map as a single file and load chunks from that file. Note: it's possible to load parts from a file by specifying the starting index and how many bytes you want to read. If you save a chunk from a tilemap as a string of bytes where each byte represents the ID of the tile, you should be able to implement the feature.
        - [ ] Make it possible to load tile textures from a spritesheet (e.g. a single image that contains every tile). Note: this is lower priority since we don't have any actual tile textures yet
- **Maks:**
    - [ ] Rewrite TileMap class
    - [ ] Create a class ```Building``` where a player can enter and exit. Entering the building will teleport the player to a different map with the building's layout. If any zombies were chasing the player before they entered the building, they should have a chance to enter the building as well.
    - [ ] Gun recoil animation

### Extra (low priority):
- **Deniel:**
    - [ ] Create a tool for making custom tile maps (or maybe find an existing one?)
- **Maks:**
    - [ ] Pixel art