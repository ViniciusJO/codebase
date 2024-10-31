#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"

#include "../dyn_array.h"

DECLARE_DYN_ARRAY(double)
DECLARE_DYN_ARRAY(char)
DECLARE_DYN_POINTER_ARRAY(char)

DynamicPointerArray(char) arr;

int main(void) {

  init_char_pointer_array(&arr, 8);
  (void)arr.data;
  // Initialize the window and context
  const int screenWidth = 1600;
  const int screenHeight = 900;
  InitWindow(screenWidth, screenHeight, "raygui - dmenu selection");

  // Sample list of items for the dmenu
  const char *items[] = {"Option 1", "Option 2", "Option 3", "Option 4",
                         "Option 5", "_--------------_"};
  int numItems = sizeof(items) / sizeof(items[0]);
  int selectedItem = -1; // No item selected initially

  // Search string
  char searchText[256] = "\0"; // Empty search string

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  while (!WindowShouldClose()) { // Detect window close button or ESC key
    // GUI Interaction
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw search box and update search text
    GuiTextBox((Rectangle){20, 20, screenWidth - 40, 30}, searchText, 256,
               true);

    // Draw filtered list of items
    int filteredIndex = 0;
    for (int i = 0; i < numItems; i++) {
      // Only display items that match the search text
      if (strlen(searchText) == 0 || strstr(items[i], searchText) != NULL) {
        if (GuiButton(
                (Rectangle){20, 70 + (screenHeight - 70.0f) / (numItems+1) * filteredIndex, screenWidth - 40, (screenHeight - 70.0f) / (numItems+1) },
                items[i])) {
          selectedItem = i; // Capture the selected item
        }
        filteredIndex++;
      }
    }

    // Display the selected item (if any)
    if (selectedItem >= 0) {
      DrawText(TextFormat("Selected: %s", items[selectedItem]), 20,
               screenHeight - 40, 20, DARKGRAY);
    }

    EndDrawing();
  }

  // De-Initialization
  CloseWindow(); // Close window and OpenGL context

  return 0;
}


