# Nspire Movie Player

This is a movie player for the Ti NSpire CX calculator. http://youtu.be/Yx0L7ce5hq4

Precompiled binaries are available for download at the Github Releases Tab


## Status

At the moment, this is **only** working for the NSpire CX. ~~Porting to the older models is underway (and should work but is untested)~~

Timing is not currently implemented, the calculator tries to play the video as fast as possible.

## What's the difference between this and nPlayer?

I haven't had a look since the start of 2011 at nPlayer but at that time, it looked like it couldn't run for more than a few seconds. The compression they used was run length encoding.

This player uses a public domain jpeg decoder to decode each frame and relies on jpeg compression to keep file sizes low. This has a performance penalty on the calculator (manages 9~10 fps) but significantly reduces the file size (especially for the CX where there's more pixel data).

I don't know about nPlayer, but a typical NMP movie file size is about ~2.86Mb per minute at 15fps.

~~Technically, this can also work with any image type supported by the stb_image.c file. This includes PNG images.~~
Only works with JPEG now to avoid checking image type every frame




##### (The following sections assume Linux as your OS)
## Compiling

Clone this repo to some location on your computer. Alternatively, download an archive and extract it.

1. Make sure you have a working Ndless SDK.
1. Go into the source code directory
1. Run ```make``` and it should compile everything.


## Installation

1. Acquire binaries by either compiling or downloading them from the releases tab
1. Make sure you have Ndless installed on your calculator
    - get it [here](https://github.com/ndless-nspire/Ndless/releases/latest/) if you don't have it yet
1. Copy the nspiremovieplayer.tns file to your calculator using either the official linking program or TILP
1. Run nspiremovieplayer.tns on your calculator once to register the .nmp file extension

After following these steps you can simply open a .nmp video file using the file explorer on your calculator.


## Controls

Press ```Esc``` on the calculator to exit the movie player.\
Pressing the ```S``` key while a video is playing will open a popup-window which displays the current timestamp of the video in seconds and allows you to jump to another timestamp (also in seconds).

## How to convert movies for this?

First you need the following installed on your host machine:
* ffmpeg
* A C compiler

You can simply run ```make helper``` then you can use the ```./convert.sh``` script inside the *videos directory* and it'll do everything for you. Choose a filename with the extension ```.nmp.tns```. For the FPS choose something between 8-11 (may vary) so the video plays back in real-time and not slow-motion.

Otherwise, if you enjoy more control over the ffmpeg options, make a folder for all your frames. Then compile the *helper.c* program and copy the executable into your folder.

Change into your directory and run the following command on the command line:

    ffmpeg -i /path/to/movie -s qvga -an -r 15 frame-%05d.jpg

This will spit out a whole lot of files. When it finishes, run the moviecompile program and it will produce a file. Copy this to your calculator along with the main program and run it.

Enjoy your movie.

## License

Licensed under GPLv3. See LICENSE.
