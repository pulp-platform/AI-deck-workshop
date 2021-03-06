#-----------------------------------------------------------------------------#
# File: img_proc.h                                                            #
# Original code: Bitcraze                                                     #
# Link: https://github.com/bitcraze/AIdeck_examples/tree/master/GAP8/         #
#             image_processing_examples/simple_kernel_example                 #
# Contributors:                                                               #
#          Bitcraze <contact@bitcraze.io>                                     #
#          Hanna Müller <hanmuell@iis.ee.ethz.ch>                             #
# Date:    12.04.2021                                                         #
#-----------------------------------------------------------------------------# 

#ifndef __IMG_PROC_H__
#define __IMG_PROC_H__

#include "pmsis.h"

typedef struct {
    char *srcBuffer;     // pointer to the input vector
    char *resBuffer;     // pointer to the output vector
    uint32_t width;      // image width
    uint32_t height;     // image height
    uint32_t nPE;        // number of cores
    uint32_t grayscale;        // grayscale if one
} plp_example_kernel_instance_i32;

void demosaicking(char *input, char* output, int width, int height, int grayscale);
void cluster_demosaicking(void* args);
void inverting(char *input, char* output, int width, int height);
void cluster_inverting(void* args);

#endif