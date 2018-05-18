//
// CTA Train/Bus Tracker Clock
// For use on a Raspberry Pi + a HUB52 LED matrix display
//

#define SLIDE_TIME 5000

#include <curlpp/cURLpp.hpp>
#include "cta_clock.h"
#include "model.h"
#include "cta_model.h"

using namespace rgb_matrix;
using std::cout;
using std::endl;

using namespace itr::cta_clock::model;
using namespace itr::cta_clock::providers::cta_rail;

static int itr::cta_clock::usage(char program_name[]) {
    cout << program_name << " usage : " << endl;
}

void itr::cta_clock::interrupt_handler(int sig) {
    interrupted = true;
}

int itr::cta_clock::main(int argc, char *argv[]) {
    cout << "[main]\tStarting up..." << endl;
    cURLpp::Cleanup cleanup();

    // Initialize slideshow
    time_t last_slide_switch = time(nullptr);
    unsigned long num_slides, provider_id, line_id = 0;

    // Initialize matrix
    int x_orig = 0;
    int y_orig = 0;
    auto matrix_options = new RGBMatrix::Options();
    auto runtime_options = new RuntimeOptions();

    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, matrix_options, runtime_options)) {
        return usage(argv[0]);
    }

    if (!largeFont.LoadFont(LARGE_FONT)) {
        std::cerr << "[main]\tCouldn't load font " << LARGE_FONT << std::endl;
        //return 1;
    }
    if (!smallFont.LoadFont(SMALL_FONT)) {
        std::cerr << "[main]\tCouldn't load font " << SMALL_FONT << std::endl;
        //return 1;
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
    sprintf(to, " TO ");

    // register providers
    cout << "[providers]\tRegistering providers..." << endl;
    std::vector<Provider *> providers;
    providers.push_back(get_itr_cta_rail_lines());

    cout << "[providers]\t" << providers.size() << " providers registered." << endl;

    // count lines
    int num_lines = 0;
    for (auto l : providers) {
        num_lines += l->Lines.size();
    }

    cout << "[providers]\t" << num_lines << " lines registered." << endl;


    // register signal handlers
    signal(SIGTERM, interrupt_handler);
    signal(SIGINT, interrupt_handler);

    // begin drawing
    while (!interrupted) {
        x = 0;
        y = largeFont.baseline();

        canvas->Fill(0, 0, 0);

        // draw line name
        rgb_matrix::DrawText(canvas, largeFont, x, y, Color(255, 255, 255), nullptr, providers[0]->Lines[0].Name);
        y += largeFont.baseline();

        // draw directions
        for (auto &i : providers[0]->Lines[0].Directions) {
            x = rgb_matrix::DrawText(canvas, smallFont, 0, y, Color(255, 255, 255), nullptr, to, 0);
            rgb_matrix::DrawText(canvas, largeFont, x, y, Color(255, 255, 255), nullptr, i.Destination, 0);
            y += largeFont.baseline();
        }

        draw_lower_third(canvas);

        // send to screen
        canvas = matrix->SwapOnVSync(canvas);
    }

    // clean up
    cURLpp::terminate();
    matrix->Clear();
    delete matrix;
    delete matrix_options;
    delete runtime_options;

    std::cout << std::endl;
    return 0;
}

void itr::cta_clock::draw_lower_third(FrameCanvas *canvas) {
    int x = itr::cta_clock::draw_clock(canvas);

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

int itr::cta_clock::draw_clock(FrameCanvas *canvas) {
    char buf[16];
    const char *time_formats[2] = {"%l:%M %p", "%l %M %p"};
    time_t raw_time = time(nullptr);
    strftime(buf, 16, time_formats[raw_time % 2], localtime(&raw_time));

    return rgb_matrix::DrawText(canvas, smallFont, 0, MATRIX_ROWS - 1, Color(255, 255, 255), buf);
}

int main(int argc, char *argv[]) { itr::cta_clock::main(argc, argv); }
