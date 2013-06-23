
/* Global constants */

/* Graphical stuff */
#define KEYBOARD_WIDTH 640
#define KEYBOARD_HEIGHT 480
#define KEY_COUNT 52
#define KEY_WIDTH (KEYBOARD_WIDTH / KEY_COUNT)
#define KEYBOARD_XOFFSET 0
#define KEYBOARD_YOFFSET 0
#define FRAME_RATE 30

/* FFT Settings */
#define WINDOW_SIZE 17000
#define BUFFER_SIZE 102000 
#define CHUNK_SIZE 17000 
#define HEADER_LENGTH 44
#define MAX_POS (BUFFER_LENGTH - WINDOW_SIZE)
#define WINDOW_FUNC WF_Hanning

