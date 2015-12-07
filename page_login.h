#ifndef _page_login_h_
#define _page_login_h_


#include <FL/Fl_Group.H>


class Fl_Input;
namespace MultiLogin {


/**
 * @class Page_Login
 * Класс виджета ввода информации пользователя.
 */
class Page_Login : public Fl_Group
{
public:
    Page_Login(int x, int y, int w, int h, const char *label = 0);
    virtual ~Page_Login();

private:
    Fl_Input *inpUname;
    Fl_Input *inpPwd;
};


} // namespace MultiLogin


#endif // _page_login_h_
