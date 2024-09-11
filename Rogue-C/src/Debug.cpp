#include "./include/core/game/Debug.h"
#include "./include/core/game/Scene.h"
#include "./include/core/physics/Collisions.h"
#include "./include/raylib/raylib.h"

namespace Serialization::Debug {
    static std::unordered_map<std::size_t, std::function<void (int, int, const Serialization::Node*)>> printFunctions {
        { typeid(Vec2).hash_code(), [](int x, int y, const Serialization::Node* vec) { 
            DrawText(std::format("{}: | x:{:5.2f}, y:{:5.2f} |", vec->name, std::stof(vec->children[0].value), std::stof(vec->children[1].value)).c_str(), x, y, 10, WHITE); } },
        { typeid(Color).hash_code(), [](int x, int y, const Serialization::Node* color) { 
                DrawText(std::format("{}:   | r:{}, g:{}, b:{}, a:{} |", color->name, 
                    color->children[0].value, color->children[1].value, color->children[2].value, color->children[3].value).c_str(), x, y, 10, WHITE); 
                Color newCol = color->Read<Color>();              
                DrawRectangle(MeasureText(color->name.c_str(), 10) + 7 + x, y, 10, 10, newCol);
            } }
    };

    static Entity currentEntity = -1;
    static bool hierarchyOpened = false;

    void PrintEntity(int x, int& y, const Node& node) {
        int startingX = x;
        int startingY = y;
        Entity id = std::stoi(node.children[0].value);
        if(IMGUIButton(x, y, "SELECT")) {
            currentEntity = id;
        }

        DrawText(node.name.c_str(), x + 60, y, 10, WHITE);

        if(id == currentEntity) {
            for(auto const& comp : node.children) {
                y += 14;
                Serialization::Debug::PPrint(x + 10, y, &comp);
                y += 14;
            }
        }

        DrawRectangleLines(startingX - 4, startingY - 4, 245 - startingX, y - startingY + 18, WHITE);

        y += 16;
    }

    void PrintHierarchy() {
        if(IsKeyPressed(KEY_F3)) {
            hierarchyOpened = !hierarchyOpened;
        }        

        if(hierarchyOpened == false) {
            return;
        }

        Serialization::Node root;
        Core::SerializedScene scene;
        Serialization::Write(&root, scene);

        DrawRectangle(0, 0, 250, GetRenderHeight(), Color(0, 0, 0, 180));
        int y = 5;
        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 5, y, 10, WHITE);
        DrawText(std::format("Entities: {}", root.children.size()).c_str(), 5, y + 16, 10, WHITE);
        y += 36;

        for(auto const& entity : root.children) {
            PrintEntity(5, y, entity);
        }
    }

    void PPrint(int x, int& y, const Serialization::Node* whatToPrint) {
        if(printFunctions.find(whatToPrint->type) != printFunctions.end()) {
            printFunctions[whatToPrint->type](x, y, whatToPrint);
            return;
        }

        DrawText(std::format("{}: {}", whatToPrint->name, whatToPrint->value).c_str(), x, y, 10, WHITE);

        for(auto const& comp : whatToPrint->children) {
            y += 14;
            Serialization::Debug::PPrint(x + 10, y, &comp);
        }
    }

    bool IMGUIButton(int x, int y, const char* text) {
        float width = MeasureText(text, 10);
        bool pressed = false;
        Color color = Color{ .r = 50, .g = 50, .b = 50, .a = 50};
        if(PointInRectangle(Vec2(GetMouseX(), GetMouseY()), Vec2(x + (width / 2) + 3, y + 5), Vec2(width + 6, 10))) {
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                color = Color{ .r = 200, .g = 200, .b = 200, .a = 150};
                pressed = true;
            } else {
                color = Color{ .r = 150, .g = 150, .b = 150, .a = 100};
            }
        }

        DrawRectangle(x, y - 2, width + 8, 14, color);
        DrawText(text, x + 4, y, 10, WHITE);
        DrawRectangleLines(x, y - 2, width + 8, 14, WHITE);
        return pressed;
    }
}