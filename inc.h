#ifndef INC_H
#define INC_H
#define STBI_HEADER_FILE_ONLY
#define is_cx true
#include <os.h>

typedef struct {
	scr_type_t screenType;
	int currentFrame;
	int targetFrame;
	int targetNotReached;
	uint16_t *framebuffer;
	void *fileBuffer;

}frameinfo;

/* File reading */
inline uint32_t file_read_uint32(FILE*fp);
void header_read(FILE*fp);
void movie_rewind(FILE*fp);

/* Config file */
int config_file_already_written();
void write_config_file();

/* Frame decoding */
void process_next_frame(FILE*fp, frameinfo* Frameinfo);

/* Timers */
void setup_timer(int fps);
void restore_timer();
void tick_timer();

/* Globals */
extern int fps, init;
extern long int movie_start_offset;
extern uint8_t* loadJpegBuf;

#define COMP_PER_PIXEL_CX 3
#define COMP_PER_PIXEL_GREY 1

#endif
