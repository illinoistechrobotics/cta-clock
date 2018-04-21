//
// CTA Train/Bus Tracker Clock
// For use on with Raspberry Pi + a HUB52 LED matrix display
//

#include "cta_clock.h"

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
        auto curl = cURLpp::Cleanup();

        // Initialize matrix
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

        if (matrix == nullptr)
            return 1;

        matrix->SetBrightness(25);

        int x = x_orig;
        int y = y_orig;

        canvas = matrix->CreateFrameCanvas();

        char static_txt_buffer[LINES_ON_SCREEN][BUFFER_SIZE];

        char to[5];
        char lineName[64];
        char direction[2][64];

        sprintf(to, " TO ");

        sprintf(lineName, "29 State");
        sprintf(direction[0], "Navy Pier");
        sprintf(direction[1], "95th Red Line");

        signal(SIGTERM, interrupt_handler);
        signal(SIGINT, interrupt_handler);

        while (!interrupted) {
            x = 0;
            y = largeFont.baseline();

            canvas->Fill(0, 0, 0);

            rgb_matrix::DrawText(canvas, largeFont, x, y, Color(255, 255, 255), NULL, lineName);
            y += largeFont.baseline();


            for (auto &i : direction) {
                x = rgb_matrix::DrawText(canvas, smallFont, 0, y, Color(255, 255, 255), NULL, to, 0);
                rgb_matrix::DrawText(canvas, largeFont, x, y, Color(255, 255, 255), NULL, i, 0);
                y += largeFont.baseline();
            }

            draw_lower_third(canvas);

            canvas = matrix->SwapOnVSync(canvas);
        }

        cURLpp::terminate();
        matrix->Clear();
        delete matrix;
        delete matrix_options;
        delete runtime_options;

        std::cout << std::endl;
        return 0;
    }

    void draw_lower_third(FrameCanvas *canvas) {
        int x = draw_clock(canvas);

        int hr_y = MATRIX_ROWS - smallFont.baseline() - 2;

/*for (int x1 = 0; x1 < MATRIX_COLS * MATRIX_CHAIN; x1++) {
            canvas->SetPixel(x1, hr_y, 255, 255, 255);
       }*/
        for (int y1 = MATRIX_ROWS; y1 > hr_y - 1; y1--) {
            canvas->SetPixel(x, y1, 255, 255, 255);
        }

        char buf[512];
        sprintf(buf, "This is a really long alert message that will not fit on the screen.");

        rgb_matrix::DrawText(canvas, smallFont, x + 2, MATRIX_ROWS - 1, Color(255, 255, 255), buf);
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
