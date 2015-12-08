#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Wizard.H>

#include "callbacks.h"
#include "logic.h"
#include "page_login.h"
#include "page_serv.h"

#include "application.h"


namespace MultiLogin {


// Метки виджетов
const char *APP_NAME = "MultiLogin";
const char *LBL_TERM = "Завершить существующие сессии";
const char *LBL_SUBMIT = "Вход";


/**
 * Внутренняя структура класса
 */
struct ApplicationImplementation
{
    ApplicationImplementation();

    Fl_Window *create_main_window() const;

    std::unique_ptr<Logic> logic;
    std::unique_ptr<Fl_Window> win;
};


ApplicationImplementation::ApplicationImplementation()
    : logic(nullptr),
      win(nullptr)
{
    logic.reset(new Logic);
    win.reset(create_main_window());
}


/**
 * Создание графического интерфейса
 */
Fl_Window* ApplicationImplementation::create_main_window() const
{
    Fl_Window *win = new Fl_Window(30, 30, 520, 380, APP_NAME);
    win->begin();
    {
        // Child #0
        Fl_Wizard *wzd = new Fl_Wizard(10, 10, 500, 300);
        wzd->begin();
        {
            new MultiLogin::Page_Login(20, 20, 480, 280);   // Child #0
            new MultiLogin::Page_Serv(20, 20, 480, 280);    // Child #1
        }
        wzd->end();
        wzd->box(FL_THIN_DOWN_FRAME);

        // Child #1
        Fl_Check_Button *chkTerm = new Fl_Check_Button(20, 320, 300, 30, LBL_TERM);
        chkTerm->callback(chkTerm_Click, reinterpret_cast<void *>(logic.get()));

        // Child #2
        Fl_Button *btnSubmit = new Fl_Button(420, 320, 80, 30, LBL_SUBMIT);
        btnSubmit->callback(btnSubmit_Click, reinterpret_cast<void *>(logic.get()));

        // Child #3
        Fl_Box *boxMsg = new Fl_Box(10, 360, 500, 20);
        boxMsg->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
    }
    win->end();

    return win;
}


Application::Application()
    : m(nullptr)
{
    m.reset(new ApplicationImplementation);
}

Application::~Application() { }


void Application::draw_gui()
{
    m->win->show();
}


} // namespace MultiLogin
