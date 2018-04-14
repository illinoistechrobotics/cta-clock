//
// CTA Train/Bus Tracker Clock
// For use on with Raspberry Pi + a HUB52 LED matrix display
//

#include "cta_clock.h"
#include <iostream>
#include "matrix/include/graphics.h"
#include "matrix/include/led-matrix.h"
#include <csignal>

#define LARGE_FONT "matrix/fonts/6x10.bdf"
#define SMALL_FONT "matrix/fonts/5x7.bdf"
#define MATRIX_CHAIN 2
#define MATRIX_ROWS 32
#define MATRIX_COLS 64
#define LINES_ON_SCREEN 3
#define BUFFER_SIZE 64
#define MAX_X MATRIX_ROWS
#define MAX_Y (MATRIX_COLS * MATRIX_CHAIN)

using namespace rgb_matrix;
using std::cout;
using std::endl;

namespace itr::cta_clock {
    RGBMatrix *matrix;
    FrameCanvas *canvas;

    Font largeFont, smallFont;

    volatile bool interrupted = false;

    static int usage(char program_name[]) {
        std::cout << program_name << " usage : " << std::endl;
    }

    void interrupt_handler(int sig) {
        interrupted = true;
    }

    int main(int argc, char *argv[]) {
        int x_orig = 0;
        int y_orig = 0;
        auto matrix_options = new RGBMatrix::Options();
        auto runtime_options = new RuntimeOptions();

        if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, matrix_options, runtime_options)) {
            return usage(argv[0]);
        }

        if (!largeFont.LoadFont(LARGE_FONT)) {
            std::cerr << "Couldn't load font " << LARGE_FONT << std::endl;
            return 1;
        }
        if (!smallFont.LoadFont(SMALL_FONT)) {
            std::cerr << "Couldn't load font " << SMALL_FONT << std::endl;
            return 1;
        }

        matrix_options->chain_length = MATRIX_CHAIN;
        matrix_options->cols = MATRIX_COLS;
        matrix_options->rows = MATRIX_ROWS;
        matrix_options->hardware_mapping = "adafruit-hat";

        matrix = rgb_matrix::CreateMatrixFromOptions(*matrix_options, *runtime_options);

        if (matrix == NULL)
            return 1;

        matrix->SetBrightness(25);

        int x = x_orig;
        int y = y_orig;

        canvas = matrix->CreateFrameCanvas();

        char static_txt_buffer[LINES_ON_SCREEN][BUFFER_SIZE];

        sprintf(static_txt_buffer[0], " 29 State");
        sprintf(static_txt_buffer[1], "GRN Green Line");

        signal(SIGTERM, interrupt_handler);
        signal(SIGINT, interrupt_handler);

        while (!interrupted) {
            canvas->Fill(0, 0, 0);

            for (auto &i : static_txt_buffer) {
                rgb_matrix::DrawText(canvas, largeFont, x, y + largeFont.baseline(), Color(255, 255, 255), NULL, i, 0);
                y += largeFont.baseline();
            }
            y = y_orig;

            draw_lower_third(canvas);

            canvas = matrix->SwapOnVSync(canvas);
        }

        matrix->Clear();
        delete matrix;
        delete matrix_options;
        delete runtime_options;

        std::cout << std::endl;
        return 0;
    }

    void draw_lower_third(FrameCanvas *canvas) {
        int x = draw_clock(canvas) + 1;
        for (int y1 = MATRIX_ROWS; y1 > MATRIX_ROWS - 10; y1--) {
            canvas->SetPixel(x, y1, 255, 255, 255);
        }
    }

    int draw_clock(FrameCanvas *canvas) {
        char buf[16];
        const char *time_formats[2] = {"%l:%M %p", "%l %M %p"};
        time_t raw_time = time(nullptr);
        strftime(buf, 16, time_formats[raw_time % 2], localtime(&raw_time));

        return rgb_matrix::DrawText(canvas, smallFont, 0, MATRIX_ROWS - 1, Color(255, 255, 255), buf);
    }
}

int main(int argc, char *argv[]) { itr::cta_clock::main(argc, argv); }