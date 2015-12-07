#include <FL/Fl_Input.H>
#include "page_login.h"


namespace MultiLogin {


// Метки виджетов
const char *LBL_UNAME = "Логин";
const char *LBL_PWD = "Пароль";


Page_Login::Page_Login(int x, int y, int w, int h, const char *label)
    : Fl_Group(x, y, w, h, label),
      inpUname(nullptr),
      inpPwd(nullptr)
{
    begin();
    inpUname = new Fl_Input(x + 160, y + 80, 180, 30, LBL_UNAME);   // Ввод логина
    inpPwd = new Fl_Input(x + 160, y + 140, 180, 30, LBL_PWD);      // Ввод пароля
    end();

    inpPwd->input_type(FL_SECRET_INPUT);
}


Page_Login::~Page_Login() { }


} // namespace MultiLogin
