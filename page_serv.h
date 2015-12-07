#ifndef _page_serv_h_
#define _page_serv_h_


#include <FL/Fl_Group.H>


class Fl_Pack;
namespace MultiLogin {


/**
 * @class Page_Serv
 * Класс виджета данных подключений.
 */
class Page_Serv : public Fl_Group
{
public:
    Page_Serv(int x, int y, int w, int h, const char *label = 0);
    virtual ~Page_Serv();

private:
    Fl_Pack *pckPanel;
};


} // namespace MultiLogin


#endif // _page_serv_h_
