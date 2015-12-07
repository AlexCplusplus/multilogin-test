#include <FL/Fl.H>
#include "application.h"

int main()
{
    MultiLogin::Application app;
    app.draw_gui();
    return Fl::run();
}
