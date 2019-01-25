//
// Created by 吴跃 on 2019/1/25.
//

#ifndef BLURVIEW_BLUR_H
#define BLURVIEW_BLUR_H

#include <malloc.h>

#define ABS(a) ((a)<(0)?(-a):(a))
#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))


int *blur_ARGB_8888(int *, int, int, int);
short *blur_RGB_565(short *, int, int, int);

#endif //BLURVIEW_BLUR_H
