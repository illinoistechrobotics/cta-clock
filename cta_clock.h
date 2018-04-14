//
// Created by tberg on 4/13/18.
//

#ifndef CTA_CLOCK_CTA_CLOCK_H
#define CTA_CLOCK_CTA_CLOCK_H

namespace itr::cta_clock {
    static int usage(char program_name[]);

    void interrupt_handler(int sig);

    int main(int argc, char *argv[]);

    void draw_lower_third(FrameCanvas *canvas);

    int draw_clock(FrameCanvas *canvas, int x);
}

#endif //CTA_CLOCK_CTA_CLOCK_H
