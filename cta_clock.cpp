//
// CTA Train/Bus Tracker Clock
// For use on with Raspberry Pi + a HUB52 LED matrix display
//

#include <iostream>
#include "matrix/include/graphics.h"
#include "matrix/include/led-matrix.h"
#include <csignal>

#define FONT "matrix/fonts/6x12.bdf"
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

        rgb_matrix::Font font;
        if (!font.LoadFont(FONT)) {
            std::cerr << "Couldn't load font " << FONT << std::endl;
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

        time_t raw_time;

        char static_txt_buffer[LINES_ON_SCREEN][BUFFER_SIZE];

        sprintf(static_txt_buffer[0], " 29 State");
        sprintf(static_txt_buffer[1], "GRN Green Line");

        // blink colon every second
        const char* time_formats[2] = {"%l:%M %p", "%l %M %p"};

        signal(SIGTERM, interrupt_handler);
        signal(SIGINT, interrupt_handler);

        while (!interrupted) {
            canvas->Fill(0, 0, 0);

            // update clock
            raw_time = time(nullptr);
            strftime(static_txt_buffer[2], 64 * sizeof(char), time_formats[raw_time % 2], localtime(&raw_time));

            for (auto &i : static_txt_buffer) {
                rgb_matrix::DrawText(canvas, font, x, y + font.baseline(), Color(255, 255, 255), NULL, i, 0);
                y += font.baseline();
            }
            y = y_orig;

            canvas = matrix->SwapOnVSync(canvas);
        }

        matrix->Clear();
        delete matrix;
        delete matrix_options;
        delete runtime_options;

        std::cout << std::endl;
        return 0;
    }
}

int main(int argc, char *argv[]) { itr::cta_clock::main(argc, argv); }