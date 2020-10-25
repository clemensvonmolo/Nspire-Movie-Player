#include <os.h>
#include "inc.h"

int fps = 0, init = 0;
uint8_t* loadJpegBuf;
//#define DEBUG

static void set_lcd_mode(unsigned int mode)
{
    uint32_t control = *IO_LCD_CONTROL;
    control &= ~0b1110;
    control |= mode << 1;
    *IO_LCD_CONTROL = control;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        if (config_file_already_written()) {
            show_msgbox(
                "Nspire Movie Player",
                "The program plays .nmp.tns movies.\n"
                "To play a video, simply open a movie file.\n");
        }else{
            int index = show_msgbox_2b(
                "Nspire Movie Player",
                "The program plays .nmp.tns movies.\n"
                "To play a video, simply open a movie file.\n"
                "Note: You have not added the correct line to "
                "your ndless.cfg.tns file. Select the option "
                "below if you want me to do it for you.",
                    "Do it",
                    "No thank you");

            switch (index) {
                case 1:
                    write_config_file();
                    show_msgbox("Nspire Movie Player",
                        "Done. You may now open movie files");
                    break;
                case 2:
                default:
                    show_msgbox("Nspire Movie Player",
                        "No problems. Remember, you can't use this until "
                        "you add the appropriate line in your ndless.cfg.tns file");
                    break;
            }
        }
        return 0;
    }
    FILE *fp = fopen(argv[1],"rb");
    if (!fp) return 0;

    header_read(fp);
    setup_timer(fps);
    #ifndef DEBUG
    unsigned prev_speed = set_cpu_speed(CPU_SPEED_150MHZ);
    #endif
    scr_type_t screenType = 0;
	frameinfo Info;
	Info.currentFrame = 0;
	Info.targetFrame = 0;
	Info.targetNotReached = 0;
	Info.screenType = screenType;
	Info.framebuffer = malloc(320*240*sizeof(uint16_t)); // display has QVGA 320x240 resolution
	loadJpegBuf = malloc(3*320*240+1); // 3*8bit channels * num_pixels +1
	if(!Info.framebuffer)
		return -1;

    set_lcd_mode(6);
    while (!isKeyPressed(KEY_NSPIRE_ESC)) {
        if(isKeyPressed(KEY_NSPIRE_S)) {
			#ifndef DEBUG
			set_cpu_speed(prev_speed);
			#endif
			const char * title = "PAUSE";
			int currTimestamp = (int)(Info.currentFrame/fps);
			char subtitle[39];
			sprintf(subtitle,"Current timestamp: %10d seconds",currTimestamp);
			const char * msg ="Skip to new timestamp?";
			int value = 0;
			if(show_1numeric_input(title,subtitle,msg,&value,-3600,3600)) {
				Info.targetFrame = value*fps;
				if (value==0) Info.targetFrame = 1;
				Info.targetNotReached = 1;
			}
			#ifndef DEBUG
			set_cpu_speed(CPU_SPEED_150MHZ);
			#endif
		}
		else if (!feof(fp)) {
			process_next_frame(fp,&Info);
		}else{
		  goto end;
		}
    }
    end:
    #ifndef DEBUG
	set_cpu_speed(prev_speed);
	#endif
	//free(Info.framebuffer);
	//free(Info.fileBuffer);  // Ndless automatically releases
	//free(loadJpegBuf);     // the memory on exit
    restore_timer();
    fclose(fp);
    return 0;
}
