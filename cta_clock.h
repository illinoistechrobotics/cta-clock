//
// Created by tberg on 4/13/18.
//

#ifndef CTA_CLOCK_CTA_CLOCK_H
#define CTA_CLOCK_CTA_CLOCK_H

#include <iostream>
#include "matrix/include/graphics.h"
#include "matrix/include/led-matrix.h"
#include <csignal>
#include <curlpp/cURLpp.hpp>

#define LARGE_FONT "matrix/fonts/6x10.bdf"
#define SMALL_FONT "matrix/fonts/4x6.bdf"
#define MATRIX_CHAIN 2
#define MATRIX_ROWS 32
#define MATRIX_COLS 64
#define LINES_ON_SCREEN 3
#define BUFFER_SIZE 64
#define MAX_X MATRIX_ROWS
#define MAX_Y (MATRIX_COLS * MATRIX_CHAIN)

using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
using rgb_matrix::Font;

namespace itr {
    namespace cta_clock {
        RGBMatrix *matrix;

        FrameCanvas *canvas;

        Font largeFont, smallFont;

        volatile bool interrupted = false;

        static int usage(char program_name[]);

        void interrupt_handler(int sig);

        int main(int argc, char *argv[]);

        void draw_lower_third(rgb_matrix::FrameCanvas *canvas);

        int draw_clock(rgb_matrix::FrameCanvas *canvas);
    }
}

#endif //CTA_CLOCK_CTA_CLOCK_H
