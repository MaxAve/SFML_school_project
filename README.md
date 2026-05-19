# TODO:

### Suggestions

- Make guns & ammo usable (in general all items)
- Max creates Tile-Map Editor after Deniel implements the TileMap management
- implement tracer for flying bullets
- armoring (like in minecraft)


### Features (high priority):

- **Deniel:**
    - Map:
        - [ ] Make items droppable
        - [ ] Make it possible to save doors, structures, lootboxes\
        - [ ] Data:
            - Door: ID (int), Target door ID (int), Position (x,y), Size (x, y)
            - Structure: Type (int), Position (x,y) <-- Do this later (once we have a structure class)
            - Loot box: Type (int), Position (x,y), Items it contains (list of item ID's)\
        Example:
```
door(0, 1, (10,10), (50,150))
door(1, 0, (200,200), (50,150))
structure(69, (400, 500))
box(0, (-140, 230), (1, 0, 0, 2, 4))
```
- **Maks:**
    - [ ] Gun recoil animation

### Extra (low priority):
- **Deniel:**
    - [ ] Create a tool for making custom tile maps (or maybe find an existing one?)
- **Maks:**
    - [ ] Pixel art