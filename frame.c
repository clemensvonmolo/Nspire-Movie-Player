#include <os.h>
#include "inc.h"
#include "jpeg.h"
#include <string.h>

static inline uint16_t convert_color_cx(uint8_t (*ptr)[3]) {
    uint8_t r,g,b, *colors = (uint8_t*)ptr;
    uint16_t final = 0;
    r = colors[0];
    g = colors[1];
    b = colors[2];

    final |= (r>>3)<<11; //Divide by 8 then bitshift to end
    final |= (g>>2)<<5;  //Divide by 4 then bitshift
    final |= (b>>3);     //Divide by 8

    return final;
}

static inline uint8_t convert_color_grey(uint8_t *grey) {
    return (*grey>>4); //Divide by 16
}

static void* fbuffer_ptr;

static inline void write_to_buffer_cx(uint8_t (*ptr)[3]) {
    *(uint16_t*)fbuffer_ptr = convert_color_cx(ptr);
    fbuffer_ptr = ((uint16_t*)fbuffer_ptr) + 1;
}

static inline void write_to_buffer_grey(uint8_t *grey) {
    static uint8_t nibble = 0;
    if (nibble) {
        *(uint8_t*)fbuffer_ptr |= convert_color_grey(grey);
        fbuffer_ptr = ((uint8_t*)fbuffer_ptr) + 1;
        nibble = 0;
    }else{
        *(uint8_t*)fbuffer_ptr = convert_color_grey(grey)<<4;
        nibble = 1;
    }
}

void process_next_frame(FILE*fp, frameinfo* Frameinfo) {
    if (!init) return;
    uint32_t next_size;

    // code for skipping to a certain point in the video
    if (!Frameinfo->targetNotReached) {
        next_size = file_read_uint32(fp);
        Frameinfo->currentFrame++;
    } else {
        if (Frameinfo->currentFrame > Frameinfo->targetFrame) {
            fseek(fp,movie_start_offset,SEEK_SET);
            Frameinfo->currentFrame = 0;
        }
        while (Frameinfo->targetNotReached) {
            if (feof(fp)) return;
            next_size = file_read_uint32(fp);
            Frameinfo->currentFrame++;
            fseek(fp,next_size,SEEK_CUR);
            if((Frameinfo->currentFrame)-(Frameinfo->targetFrame) == 0) {
                Frameinfo->targetNotReached = 0;
                next_size = file_read_uint32(fp);
            }

        }
    }

    int x = 320,y = 240;
    static uint32_t readbuf_size = 0;

    // reuse buffer if large enough, resize otherwise
    if(!readbuf_size) {
        Frameinfo->fileBuffer = malloc(next_size);
        //printf("readbuf malloc\n");
        if (!Frameinfo->fileBuffer) return;
        readbuf_size = next_size;
    } else if(readbuf_size < next_size) {
        //printf("readbuf realloc\n");
        Frameinfo->fileBuffer = realloc(Frameinfo->fileBuffer,next_size);
        if (!Frameinfo->fileBuffer) return;
        readbuf_size = next_size;
    }

    fread(Frameinfo->fileBuffer, 1, next_size, fp);

    void *data = custom_jpeg_load(Frameinfo->fileBuffer, next_size);
    if (data) {
        unsigned i = x*y;
        void* ptr = data;
        int pix = 0;

        while (i) {
            *(Frameinfo->framebuffer+(pix)) = convert_color_cx(ptr);
            pix++;
            ptr = ((char*)ptr) + COMP_PER_PIXEL_CX;
            i--;
        }
        lcd_blit(Frameinfo->framebuffer,Frameinfo->screenType);

    }
}
