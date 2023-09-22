#ifndef VECTOR_CONFIG_H
#define VECTOR_CONFIG_H

typedef unsigned int uint;

#define PREVIEW_MODE 0

#if PREVIEW_MODE
const int WINDOW_WIDTH  = 720;
const int WINDOW_HEIGHT = 480;
const uint SAMPLES_NUM  = 10;
const uint RENDER_DEPTH = 10;

#define PRINT_PROGRESS 0

#else

const int WINDOW_WIDTH  = 1920;
const int WINDOW_HEIGHT = 1080;
const uint SAMPLES_NUM  = 1000;
const uint RENDER_DEPTH = 50;

#define PRINT_PROGRESS 1

#endif

const char *const WINDOW_TITLE   = "Render preview";
const double NEAR_ZERO_VEC_1DLEN = 1e-5;
const char *const OUTPUTFILE     = "output.png";

#endif // VECTOR_CONFIG_H
