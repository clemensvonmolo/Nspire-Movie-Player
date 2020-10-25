#include "jpeg.h"
#include "inc.h"
static inline void* malloc_return_buf(void);
#define STBI_NO_HDR
#define STBI_ONLY_JPEG
#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/*
    Replaces the allocation for the final image buffer inside stb_image.h
    because allocating and freeing that buffer every frame is inefficient.
    loadJpegBuf is a global variable that is allocated in main once and released on program exit
    The buffer is large enough for 320x240 pixels with 24 bit per pixel
*/
static inline void* malloc_return_buf(void) {
    return loadJpegBuf;
}

/*
    Patched together from stbi_load_from_memory
    removed check for image type as it is known at compile time
    and some other stuff that isn't critical (I think)
*/
void* custom_jpeg_load(void const *buffer, int len) {
    int ___x, ___y, ___comp;
    int *x = &___x;
    int *y = &___y;
    int *comp = &___comp;
    int req_comp = 3;

    stbi__context ___s;

    stbi__start_mem(&___s,buffer,len);
    //return stbi__load_and_postprocess_8bit(&s,x,y,comp,req_comp);

    // start of stbi__load_and_postprocess_8bit
    stbi__context *s = &___s;
    stbi__result_info ___ri;




    //void *result = stbi__load_main(s, x, y, comp, req_comp, &ri, 8);

    // start of stbi__load_main
    stbi__result_info *ri = &___ri;
    //int bpc = 8;
    memset(ri, 0, sizeof(*ri)); // make sure it's initialized if we add new fields
    ri->bits_per_channel = 8; // default is 8 so most paths don't have to be changed
    ri->channel_order = STBI_ORDER_RGB; // all current input & output are this, but this is here so we can add BGR order
    ri->num_channels = 0;

    // we can affort to skip testing the image type
    return stbi__jpeg_load(s,x,y,comp,req_comp, ri);
    //end of stbi_load_main



    /*
    if (result == NULL)
       return NULL;

    if (___ri.bits_per_channel != 8) {
       STBI_ASSERT(___ri.bits_per_channel == 16);
       result = stbi__convert_16_to_8((stbi__uint16 *) result, *x, *y, req_comp == 0 ? *comp : req_comp);
       ___ri.bits_per_channel = 8;
    }

    // @TODO: move stbi__convert_format to here

    if (stbi__vertically_flip_on_load) {
       int channels = req_comp ? req_comp : *comp;
       stbi__vertical_flip(result, *x, *y, channels * sizeof(stbi_uc));
    }

    return (void*) result;
    // end of stbi__load_and_postprocess_8bit
    */
}
