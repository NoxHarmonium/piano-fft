
/* Global constants */

/* Graphical stuff */
#define KEYBOARD_WIDTH 200  
#define KEYBOARD_HEIGHT 50
#define KEY_COUNT 52
#define KEY_WIDTH (KEYBOARD_WIDTH / KEY_COUNT)
#define KEYBOARD_XOFFSET 50
#define KEYBOARD_YOFFSET 50
#define FRAME_RATE 30

/* FFT Settings */
#define WINDOW_SIZE 1024
#define BUFFER_SIZE 44100
#define CHUNK_SIZE 11025
#define HEADER_LENGTH 44
#define MAX_POS (BUFFER_LENGTH - WINDOW_SIZE)

