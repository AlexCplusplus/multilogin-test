#include <FL/Fl_Pack.H>
#include "page_serv.h"


namespace MultiLogin {


Page_Serv::Page_Serv(int x, int y, int w, int h, const char *label)
    : Fl_Group(x, y, w, h, label),
      pckPanel(nullptr)
{
    begin();
    pckPanel = new Fl_Pack(x + 120, x + 30, 240, 210);  // Контейнер для списка подключений
    end();

    pckPanel->box(FL_THIN_DOWN_FRAME);
}


Page_Serv::~Page_Serv() { }


} // namespace MultiLogin
