// this shit is vibe coded as hell

#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>
#include "environment/GameMap.hpp"

class TileMapEditor
{
private:
// ── constants ────────────────────────────────────────────
static constexpr int   TILE_PX        = 16;   // pixels per tile in the spritesheet
static constexpr int   SHEET_TILES    = 16;   // tiles per row/column in the spritesheet
static constexpr int   SHEET_PX       = TILE_PX * SHEET_TILES;  // 256

static constexpr int   CHUNK_TILES    = 16;   // tiles per chunk edge
static constexpr float DISPLAY_TILE   = 32.f; // on-screen pixels per tile at zoom=1

static constexpr float UI_PANEL_W     = 280.f;
static constexpr float UI_SHEET_SCALE = 2.f;  // spritesheet displayed at 2×
static constexpr float UI_SHEET_PX    = SHEET_PX * UI_SHEET_SCALE;

static constexpr float ZOOM_MIN = 0.25f;
static constexpr float ZOOM_MAX = 8.f;
static constexpr float ZOOM_STEP = 1.15f;

static constexpr int EMPTY_TILE = 96;
static constexpr int CHUNK_SIZE = 16;

// ── types ─────────────────────────────────────────────────
struct TileCoord { int x, y; };

inline static TileCoord topLeftCorner = {9999, 9999};
inline static TileCoord bottomRightCorner = {-9999, -9999};

inline static bool rectangleMode = false;
inline static TileCoord rectToolCorner;

inline static mapSizeX = 0;
inline static mapSizeY = 0;

// Key: world tile position, Value: tile index in the spritesheet (0-255)
using TileMap = std::unordered_map<int, std::unordered_map<int, int>>;

// ── helpers ───────────────────────────────────────────────
static sf::IntRect tileRect(int index)
{
    int col = index % SHEET_TILES;
    int row = index / SHEET_TILES;
    return { { col * TILE_PX, row * TILE_PX }, { TILE_PX, TILE_PX } };
}

static bool loadTileMap(TileMap& tileMap, const std::string& path)
{
    std::ifstream in(path, std::ios::binary);

    if (!in.is_open())
        return false;

    tileMap.clear();

    // --------------------------------------------------------
    // Read header
    // --------------------------------------------------------

    size_t mapSize[2] = { 0, 0 };

    in.read(reinterpret_cast<char*>(mapSize), sizeof(size_t) * 2);

    size_t chunkCountX = mapSize[0];
    size_t chunkCountY = mapSize[1];

    // --------------------------------------------------------
    // Read chunks
    // --------------------------------------------------------

    for (size_t chunkY = 0; chunkY < chunkCountY; ++chunkY)
    {
        for (size_t chunkX = 0; chunkX < chunkCountX; ++chunkX)
        {
            int chunk[16][16];

            in.read(reinterpret_cast<char*>(chunk), sizeof(chunk));

            // Reconstruct tiles
            for (int localY = 0; localY < CHUNK_SIZE; ++localY)
            {
                for (int localX = 0; localX < CHUNK_SIZE; ++localX)
                {
                    uint8_t tile =
                        chunk[localY][localX];

                    // Skip empty tiles
                    // if (tile == EMPTY_TILE)
                    //     continue;

                    int worldX =
                        static_cast<int>(chunkX) * CHUNK_SIZE + localX;

                    int worldY =
                        static_cast<int>(chunkY) * CHUNK_SIZE + localY;

                    tileMap[worldX][worldY] = tile;
                }
            }
        }
    }

    return true;
}

static bool saveTileMap(const TileMap& tileMap, const std::string& path)
{
    if (tileMap.empty())
        return false;

    // --------------------------------------------------------
    // Find map bounds
    // --------------------------------------------------------

    int minX = INT32_MAX;
    int minY = INT32_MAX;
    int maxX = INT32_MIN;
    int maxY = INT32_MIN;

    for (const auto& xPair : tileMap)
    {
        int x = xPair.first;

        for (const auto& yPair : xPair.second)
        {
            int y = yPair.first;

            minX = std::min(minX, x);
            minY = std::min(minY, y);

            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);
        }
    }

    std::cout << minX << "," << minY << "," << maxX << "," << maxY << "\n";

    // int minX = topLeftCorner.x;
    // int minY = topLeftCorner.y;
    // int maxX = bottomRightCorner.x;
    // int maxY = bottomRightCorner.y;

    // --------------------------------------------------------
    // Convert tile bounds to chunk bounds
    // --------------------------------------------------------

    int minChunkX = minX / CHUNK_SIZE;
    int minChunkY = minY / CHUNK_SIZE;

    int maxChunkX = maxX / CHUNK_SIZE;
    int maxChunkY = maxY / CHUNK_SIZE;

    size_t chunkCountX = static_cast<size_t>(maxChunkX - minChunkX + 1);
    size_t chunkCountY = static_cast<size_t>(maxChunkY - minChunkY + 1);

    // --------------------------------------------------------
    // Open file
    // --------------------------------------------------------

    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
        return false;

    // --------------------------------------------------------
    // Write header
    // --------------------------------------------------------

    size_t header[2] = { chunkCountX, chunkCountY };

    out.write(reinterpret_cast<char*>(header), sizeof(size_t) * 2);

    // --------------------------------------------------------
    // Write chunks (top-left to bottom-right)
    // --------------------------------------------------------

    for (int chunkY = minChunkY; chunkY <= maxChunkY; ++chunkY)
    {
        for (int chunkX = minChunkX; chunkX <= maxChunkX; ++chunkX)
        {
            // Fill with EMPTY_TILE by default
            int chunk[16][16];

            // Fill chunk
            for (int localY = 0; localY < CHUNK_SIZE; ++localY)
            {
                for (int localX = 0; localX < CHUNK_SIZE; ++localX)
                {
                    int worldX = chunkX * CHUNK_SIZE + localX;
                    int worldY = chunkY * CHUNK_SIZE + localY;

                    auto xIt = tileMap.find(worldX);

                    if (xIt != tileMap.end())
                    {
                        auto yIt = xIt->second.find(worldY);

                        if (yIt != xIt->second.end())
                        {
                            chunk[localY][localX] =
                                static_cast<uint8_t>(yIt->second);
                        }
                    }
                }
            }

            out.write(reinterpret_cast<char*>(chunk), sizeof(chunk));
        }
    }

    return true;
}

public:
// ── main ─────────────────────────────────────────────────
static int start(std::string layer1Path="", std::string layer1PathSave="", std::string layer2Path="", std::string layer2PathSave="")
{
    // Window
    sf::RenderWindow window(
        sf::VideoMode({ 1280u, 800u }),
        "Tilemap Editor  |  LClick=place  RClick=pan  Wheel=zoom",
        sf::Style::Default
    );
    window.setFramerateLimit(60);

    // ── Spritesheet ──────────────────────────────────────
    sf::Texture sheetTex;
    if (!sheetTex.loadFromFile("resources/textures/environment/tilemap.png"))
    {
        // Generate a placeholder checkerboard spritesheet so the editor is
        // immediately usable even without an image file.
        sf::Image placeholder({ (unsigned)SHEET_PX, (unsigned)SHEET_PX }, sf::Color::Black);
        for (int ty = 0; ty < SHEET_TILES; ++ty)
        for (int tx = 0; tx < SHEET_TILES; ++tx)
        {
            int idx = ty * SHEET_TILES + tx;
            // Vary colours so tiles look distinct
            sf::Color base(
                static_cast<uint8_t>((tx * 16 + 40) & 0xFF),
                static_cast<uint8_t>((ty * 16 + 40) & 0xFF),
                static_cast<uint8_t>((idx * 7  + 80) & 0xFF)
            );
            for (int py = 0; py < TILE_PX; ++py)
            for (int px = 0; px < TILE_PX; ++px)
            {
                bool checker = ((px / 4 + py / 4) % 2 == 0);
                sf::Color c = checker ? base : sf::Color(
                    static_cast<uint8_t>(base.r / 2),
                    static_cast<uint8_t>(base.g / 2),
                    static_cast<uint8_t>(base.b / 2)
                );
                placeholder.setPixel({ (unsigned)(tx * TILE_PX + px),
                                       (unsigned)(ty * TILE_PX + py) }, c);
            }
            // Draw a 1-px border so tiles are easier to distinguish
            for (int p = 0; p < TILE_PX; ++p)
            {
                placeholder.setPixel({ (unsigned)(tx*TILE_PX + p), (unsigned)(ty*TILE_PX)            }, sf::Color(200,200,200));
                placeholder.setPixel({ (unsigned)(tx*TILE_PX),     (unsigned)(ty*TILE_PX + p)        }, sf::Color(200,200,200));
            }
        }
        if (!sheetTex.loadFromImage(placeholder))
        {
            std::cerr << "Failed to create placeholder spritesheet.\n";
            return 1;
        }
        std::cout << "Note: 'spritesheet.png' not found. Using generated placeholder.\n";
    }
    sheetTex.setSmooth(false);

    // ── State ─────────────────────────────────────────────
    TileMap  tileMap;
    int tileMapData[16*16][16*16]; // TODO this is stupid
    int      selectedTile = 0;   // spritesheet index 0-255
    float    zoom         = 1.f;
    sf::Vector2f viewOffset(0.f, 0.f); // canvas pan in world pixels

    bool  isPanning      = false;
    sf::Vector2i panStart;
    sf::Vector2f panOffsetStart;

    // ── RenderTexture for the canvas area ────────────────
    // We'll draw to a view instead; just track the canvas rect.
    float canvasLeft = UI_PANEL_W;

    // ── Font (system fallback) ────────────────────────────
    sf::Font font;
    bool hasFont = font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
                   font.openFromFile("/System/Library/Fonts/Helvetica.ttc") ||
                   font.openFromFile("C:/Windows/Fonts/arial.ttf");
    (void)hasFont;

    if(layer1Path.length() > 0)
        loadTileMap(tileMap, layer1Path);

    // ── Grid / chunk helper rectangles ───────────────────
    sf::RectangleShape chunkBorder, tileCursor;
    chunkBorder.setFillColor(sf::Color::Transparent);
    chunkBorder.setOutlineThickness(1.f);
    chunkBorder.setOutlineColor(sf::Color(255, 200, 100, 160));
    chunkBorder.setSize({ CHUNK_TILES * DISPLAY_TILE, CHUNK_TILES * DISPLAY_TILE });

    tileCursor.setFillColor(sf::Color(255, 255, 255, 40));
    tileCursor.setOutlineThickness(1.f);
    tileCursor.setOutlineColor(sf::Color::White);
    tileCursor.setSize({ DISPLAY_TILE, DISPLAY_TILE });

    // ── UI: spritesheet panel ─────────────────────────────
    sf::RectangleShape uiBackground({ UI_PANEL_W, 800.f });
    uiBackground.setFillColor(sf::Color(30, 30, 38));

    sf::Sprite sheetSprite(sheetTex);
    // Position inside the panel
    const float SHEET_OFFSET_X = 10.f;
    const float SHEET_OFFSET_Y = 50.f;
    sheetSprite.setScale({ UI_SHEET_SCALE, UI_SHEET_SCALE });
    sheetSprite.setPosition({ SHEET_OFFSET_X, SHEET_OFFSET_Y });

    // Selection highlight on the spritesheet
    sf::RectangleShape sheetHighlight({ TILE_PX * UI_SHEET_SCALE, TILE_PX * UI_SHEET_SCALE });
    sheetHighlight.setFillColor(sf::Color(255, 220, 50, 70));
    sheetHighlight.setOutlineThickness(2.f);
    sheetHighlight.setOutlineColor(sf::Color(255, 220, 50));

    // Thin vertical separator
    sf::RectangleShape separator({ 2.f, 800.f });
    separator.setFillColor(sf::Color(60, 60, 75));
    separator.setPosition({ UI_PANEL_W, 0.f });

    // ── Main loop ─────────────────────────────────────────
    while (window.isOpen())
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool mouseOnCanvas = mousePos.x > (int)canvasLeft;

        // ── Lambda: screen → world tile coord ────────────
        auto screenToTile = [&](sf::Vector2i screen) -> TileCoord
        {
            float wx = (screen.x - canvasLeft - viewOffset.x) / (zoom * DISPLAY_TILE);
            float wy = (screen.y               - viewOffset.y) / (zoom * DISPLAY_TILE);
            return { (int)std::floor(wx), (int)std::floor(wy) };
        };

        // ── Events ───────────────────────────────────────
        while (const std::optional<sf::Event> ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseButtonPressed = ev->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    if(rectangleMode)
                    {
                        rectToolCorner = screenToTile(mousePos);
                    }
                }
            }

            if (const auto* mouseButtonPressed = ev->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    if(rectangleMode)
                    {
                        auto rectToolCorner2 = screenToTile(mousePos);
                        for(int x = std::min(rectToolCorner.x, rectToolCorner2.x); x <= std::max(rectToolCorner.x, rectToolCorner2.x); x++)
                        {
                            for(int y = std::min(rectToolCorner.y, rectToolCorner2.y); y <= std::max(rectToolCorner.y, rectToolCorner2.y); y++)
                            {
                                tileMap[x][y] = selectedTile;
                                tileMapData[y][x] = selectedTile; // More stupid
                                topLeftCorner.x = std::min(x, topLeftCorner.x);
                                topLeftCorner.y = std::min(y, topLeftCorner.y);
                                bottomRightCorner.x = std::max(x, bottomRightCorner.x);
                                bottomRightCorner.y = std::max(y, bottomRightCorner.y);
                            }
                        }
                    }
                }
            }

            if(const auto* keyPressed = ev->getIf<sf::Event::KeyPressed>())
            {
                if(keyPressed->scancode == sf::Keyboard::Scan::S)
                {
                    if(layer1PathSave.length() > 0)
                    {
                        //layer1PathSave += "2";
                        std::cout << "[LOG] Saving map as " << layer1PathSave << "\n";
                        saveTileMap(tileMap, layer1PathSave);
                    }
                    else
                    {
                        std::cout << "[ERR] No save path provided\n";
                    }
                }
            }

            // ── Zoom ──────────────────────────────────
            if (const auto* mw = ev->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (mouseOnCanvas)
                {
                    float oldZoom = zoom;
                    if (mw->delta > 0) zoom = std::min(zoom * ZOOM_STEP, ZOOM_MAX);
                    else               zoom = std::max(zoom / ZOOM_STEP, ZOOM_MIN);

                    // Zoom toward mouse cursor
                    float mx = mw->position.x - canvasLeft;
                    float my = (float)mw->position.y;
                    viewOffset.x = mx - (mx - viewOffset.x) * (zoom / oldZoom);
                    viewOffset.y = my - (my - viewOffset.y) * (zoom / oldZoom);
                }
            }

            // ── Pan: start ────────────────────────────
            if (const auto* mb = ev->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mb->button == sf::Mouse::Button::Right && mouseOnCanvas)
                {
                    isPanning      = true;
                    panStart       = mb->position;
                    panOffsetStart = viewOffset;
                }

                // ── Tile selection in spritesheet ─────
                if (mb->button == sf::Mouse::Button::Left && !mouseOnCanvas)
                {
                    float rx = mb->position.x - SHEET_OFFSET_X;
                    float ry = mb->position.y - SHEET_OFFSET_Y;
                    if (rx >= 0 && ry >= 0 && rx < UI_SHEET_PX && ry < UI_SHEET_PX)
                    {
                        int col = (int)(rx / (TILE_PX * UI_SHEET_SCALE));
                        int row = (int)(ry / (TILE_PX * UI_SHEET_SCALE));
                        selectedTile = row * SHEET_TILES + col;
                    }
                }
            }

            // ── Pan: stop ─────────────────────────────
            if (const auto* mb = ev->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mb->button == sf::Mouse::Button::Right)
                    isPanning = false;
            }
        }

        // ── Continuous pan ────────────────────────────────
        if (isPanning)
        {
            sf::Vector2i cur = sf::Mouse::getPosition(window);
            viewOffset.x = panOffsetStart.x + (cur.x - panStart.x);
            viewOffset.y = panOffsetStart.y + (cur.y - panStart.y);
        }

        // ── Continuous tile placement ─────────────────────
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseOnCanvas && !rectangleMode)
        {
            TileCoord tc = screenToTile(mousePos);
            tileMap[tc.x][tc.y] = selectedTile;
            topLeftCorner.x = std::min(tc.x, topLeftCorner.x);
            topLeftCorner.y = std::min(tc.y, topLeftCorner.y);
            bottomRightCorner.x = std::max(tc.x, bottomRightCorner.x);
            bottomRightCorner.y = std::max(tc.y, bottomRightCorner.y);
        }

        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            rectangleMode = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
            rectangleMode = false;
        }

        // ── Draw ──────────────────────────────────────────
        window.clear(sf::Color(18, 18, 24));

        // ── Canvas ───────────────────────────────────────
        {
            // Determine visible tile range
            sf::Vector2u ws = window.getSize();
            float invZoomTile = 1.f / (zoom * DISPLAY_TILE);
            int tx0 = (int)std::floor((-viewOffset.x) * invZoomTile) - 1;
            int ty0 = (int)std::floor((-viewOffset.y) * invZoomTile) - 1;
            int tx1 = (int)std::ceil((ws.x - canvasLeft - viewOffset.x) * invZoomTile) + 1;
            int ty1 = (int)std::ceil((ws.y              - viewOffset.y) * invZoomTile) + 1;

            float tileSize = zoom * DISPLAY_TILE;
            float chunkSize = tileSize * CHUNK_TILES;

            // Draw background grid (subtle)
            sf::RectangleShape gridLine;
            gridLine.setFillColor(sf::Color(40, 40, 52));

            // Vertical grid lines
            for (int tx = tx0; tx <= tx1; ++tx)
            {
                float sx = canvasLeft + viewOffset.x + tx * tileSize;
                gridLine.setSize({ 1.f, (float)ws.y });
                gridLine.setPosition({ sx, 0.f });
                window.draw(gridLine);
            }
            // Horizontal grid lines
            for (int ty = ty0; ty <= ty1; ++ty)
            {
                float sy = viewOffset.y + ty * tileSize;
                gridLine.setSize({ ws.x - canvasLeft, 1.f });
                gridLine.setPosition({ canvasLeft, sy });
                window.draw(gridLine);
            }

            // Draw chunk borders
            int cx0 = (int)std::floor((float)tx0 / CHUNK_TILES);
            int cy0 = (int)std::floor((float)ty0 / CHUNK_TILES);
            int cx1 = (int)std::ceil( (float)tx1 / CHUNK_TILES);
            int cy1 = (int)std::ceil( (float)ty1 / CHUNK_TILES);

            chunkBorder.setSize({ chunkSize, chunkSize });
            chunkBorder.setOutlineThickness(std::max(1.f, zoom));
            for (int cy = cy0; cy <= cy1; ++cy)
            for (int cx = cx0; cx <= cx1; ++cx)
            {
                float sx = canvasLeft + viewOffset.x + cx * chunkSize;
                float sy = viewOffset.y              + cy * chunkSize;
                chunkBorder.setPosition({ sx, sy });
                window.draw(chunkBorder);
            }

            // Draw placed tiles
            sf::Sprite tileSprite(sheetTex);
            for (int ty = ty0; ty <= ty1; ++ty)
            {
                auto rowIt = tileMap.find(0); // dummy; iterate below
                (void)rowIt;
            }

            for (auto& [tx, col] : tileMap)
            {
                for (auto& [ty, idx] : col)
                {
                    if (tx < tx0 || tx > tx1 || ty < ty0 || ty > ty1) continue;
                    tileSprite.setTextureRect(tileRect(idx));
                    tileSprite.setScale({ tileSize / TILE_PX, tileSize / TILE_PX });
                    float sx = canvasLeft + viewOffset.x + tx * tileSize;
                    float sy = viewOffset.y              + ty * tileSize;
                    tileSprite.setPosition({ sx, sy });
                    window.draw(tileSprite);
                }
            }

            // Cursor highlight under mouse
            if (mouseOnCanvas)
            {
                TileCoord tc = screenToTile(mousePos);
                float sx = canvasLeft + viewOffset.x + tc.x * tileSize;
                float sy = viewOffset.y              + tc.y * tileSize;
                tileCursor.setSize({ tileSize, tileSize });
                tileCursor.setPosition({ sx, sy });
                window.draw(tileCursor);
            }
        }

        // ── UI Panel ─────────────────────────────────────
        window.draw(uiBackground);
        window.draw(separator);
        window.draw(sheetSprite);

        // Highlight selected tile on sheet
        {
            int col = selectedTile % SHEET_TILES;
            int row = selectedTile / SHEET_TILES;
            float hx = SHEET_OFFSET_X + col * TILE_PX * UI_SHEET_SCALE;
            float hy = SHEET_OFFSET_Y + row * TILE_PX * UI_SHEET_SCALE;
            sheetHighlight.setPosition({ hx, hy });
            window.draw(sheetHighlight);
        }

        // Labels
        if (hasFont)
        {
            // Panel title
            sf::Text title(font, "SPRITESHEET", 13u);
            title.setFillColor(sf::Color(180, 180, 200));
            title.setPosition({ SHEET_OFFSET_X, 26.f });
            window.draw(title);

            // Selected tile info
            std::string info = "Tile: " + std::to_string(selectedTile) +
                               "  (" + std::to_string(selectedTile % SHEET_TILES) +
                               ", " + std::to_string(selectedTile / SHEET_TILES) + ")";
            sf::Text infoText(font, info, 12u);
            infoText.setFillColor(sf::Color(255, 220, 80));
            infoText.setPosition({ SHEET_OFFSET_X, SHEET_OFFSET_Y + UI_SHEET_PX + 8.f });
            window.draw(infoText);

            // Zoom level
            std::string zoomStr = "Zoom: " + std::to_string((int)(zoom * 100)) + "%";
            sf::Text zoomText(font, zoomStr, 12u);
            zoomText.setFillColor(sf::Color(140, 200, 255));
            zoomText.setPosition({ SHEET_OFFSET_X, SHEET_OFFSET_Y + UI_SHEET_PX + 28.f });
            window.draw(zoomText);

            // Tile count
            int placed = 0;
            for (auto& [tx, col] : tileMap) placed += (int)col.size();
            sf::Text countText(font, "Tiles placed: " + std::to_string(placed), 12u);
            countText.setFillColor(sf::Color(160, 220, 160));
            countText.setPosition({ SHEET_OFFSET_X, SHEET_OFFSET_Y + UI_SHEET_PX + 48.f });
            window.draw(countText);

            //std::cout << topLeftCorner.x << ", " << topLeftCorner.y << " | " << bottomRightCorner.x << ", " << bottomRightCorner.y << "\n";

            // Keybind reminder
            sf::Text help(font,
                "LMB (sheet) = select\n"
                "LMB (canvas) = place\n"
                "RMB drag = pan\n"
                "Wheel = zoom\nB = bush mode (normal)\nR = rectangle mode", 11u);
            help.setFillColor(sf::Color(110, 110, 130));
            help.setPosition({ SHEET_OFFSET_X, SHEET_OFFSET_Y + UI_SHEET_PX + 75.f });
            window.draw(help);

            sf::Text mode(font, rectangleMode ? "Brush: rectangle" : "Brush: normal", 11u);
            mode.setFillColor(sf::Color(110, 110, 130));
            mode.setPosition({ SHEET_OFFSET_X, SHEET_OFFSET_Y + UI_SHEET_PX + 75.f - 15.f });
            window.draw(mode);
        }

        window.display();
    }

    return 0;
}
};