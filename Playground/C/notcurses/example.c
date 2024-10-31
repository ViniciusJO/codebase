/* #include <notcurses/notcurses.h> */
/**/
/* extern int sleep(int); */
/**/
/* int main(void) { */
/*   struct notcurses_options options = {0};                // Default options
 */
/*   struct notcurses *nc = notcurses_init(&options, NULL); // Initialize
 * Notcurses */
/*   if (nc == NULL) { */
/*     return -1; // Error initializing */
/*   } */
/**/
/*   struct ncplane *stdplane = notcurses_stdplane(nc); // Get the standard
 * plane */
/*   ncplane_putstr(stdplane, "Hello, Notcurses!\n\n");     // Print a string */
/**/
/*   notcurses_render(nc); // Render the output */
/*   sleep(2);             // Wait before exiting */
/**/
/*   ncinput ni; */
/*   while (notcurses_get(nc, NULL, &ni) != (uint32_t)-1) { */
/*     if (ni.id == 'q') { */
/*       break; // Quit on 'q' key */
/*     } else { */
/*       ncplane_putchar(stdplane, ni.id == 'a' ? 'y' : 'n'); */
/*     } */
/*   } */
/**/
/*   notcurses_stop(nc); // Clean up */
/*   return 0; */
/* } */

#include <notcurses/nckeys.h>
#include <notcurses/notcurses.h>
#include <unistd.h> // for sleep

typedef unsigned int uint;

void move_plane(struct ncplane *plane, int x, int y) {
    ncplane_move_yx(plane, y, x);          // Move plane diagonally
}

// Function to animate a plane moving across the screen
void animate_plane(struct notcurses *nc, struct ncplane *plane) {
  int y, x;
  ncplane_dim_yx(plane, (uint *)&y, (uint *)&x);
  struct timespec ts = {
      .tv_sec = 0,
      .tv_nsec = 100000000L // 100 milliseconds
  };

  for (int i = 0; i < 20; ++i) {
    ncplane_move_yx(plane, i, i);          // Move plane diagonally
    ncplane_erase(plane);                  // Clear the plane
    ncplane_putstr(plane, "Moving plane"); // Re-draw text
    notcurses_render(nc);                  // Render changes
    nanosleep(&ts, NULL);                  // Delay for animation effect
  }
}

int main(void) {
  // Initialize Notcurses with default options
  struct notcurses_options opts = {0};
  struct notcurses *nc = notcurses_init(&opts, NULL);
  if (nc == NULL) {
    return -1; // Failed to initialize
  }

  // Get the standard plane (this is like a canvas to draw on)
  struct ncplane *stdplane = notcurses_stdplane(nc);

  // Set colors for standard plane
  uint64_t red_on_black = NCCHANNELS_INITIALIZER(
      0, 0, 0, 255, 0, 255); // Red foreground, black background
  ncplane_set_channels(stdplane, red_on_black);

  // Draw some text on the standard plane
  ncplane_putstr(stdplane, "Hello, Notcurses! Press 'q' to quit.\n");

  // Create a new plane at position (3, 5) with size 1x20 (1 row, 20 columns)
  struct ncplane_options nopts = {
      .y = 3,
      .x = 5,
      .rows = 1,
      .cols = 20,
      .userptr = NULL,
      .name = "moving_plane",
      .resizecb = NULL,
      .flags = 0,
  };
  struct ncplane *moving_plane = ncplane_create(stdplane, &nopts);

  // Set text style and colors for moving plane
  uint64_t white_on_blue = NCCHANNELS_INITIALIZER(
      0, 0, 255, 255, 255, 255); // White text on blue background
  ncplane_set_channels(moving_plane, white_on_blue);
  ncplane_set_styles(moving_plane, NCSTYLE_BOLD | NCSTYLE_UNDERLINE);

  // Draw text on the moving plane
  ncplane_putstr(moving_plane, "Moving plane");

  // Render both planes initially
  notcurses_render(nc);

  // Handle user input
  int pos_x = 3, pos_y = 5;
  ncinput ni;
  while (notcurses_get(nc, NULL, &ni) != (uint32_t)-1) {
    ncplane_putstr(moving_plane, "Moving plane"); // Re-draw text
    notcurses_render(nc);                  // Render changes
    if (ni.id == 'q')
      break; // Exit on 'q' key press
    switch (ni.id) {
    case 'w':
    case NCKEY_UP: {
      move_plane(moving_plane, pos_x, pos_y-=1);
      ncplane_putstr(stdplane, "UP\r\n");
    } break;
    case 's':
    case NCKEY_DOWN: {
      move_plane(moving_plane, pos_x, pos_y+=1);
      ncplane_putstr(stdplane, "DOWN\n");
    } break;
    case 'a':
    case NCKEY_LEFT: {
      move_plane(moving_plane, pos_x-=1, pos_y);
      ncplane_putstr(stdplane, "LEFT\n");
    } break;
    case 'd':
    case NCKEY_RIGHT: {
      move_plane(moving_plane, pos_x+=1, pos_y);
      ncplane_putstr(stdplane, "RIGHT\n");
    } break;
    }

    ncplane_erase(moving_plane);                  // Clear the plane
    ncplane_putstr(moving_plane, "Moving plane"); // Re-draw text
    notcurses_render(nc);                  // Render changes
    // Animate the moving plane if any key is pressed
    /* animate_plane(nc, moving_plane); */
  }

  // Cleanup and stop Notcurses
  notcurses_stop(nc);
  return 0;
}
