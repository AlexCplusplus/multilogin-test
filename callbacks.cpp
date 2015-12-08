#include <string>
#include <locale>

#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Wizard.H>

#include "callbacks.h"
#include "logic.h"
#include "page_login.h"
#include "page_serv.h"


namespace MultiLogin {


// Информационные сообщения
const char *MSG_INCORRECT_NAME = "Некорректный логин.";
const char *MSG_INCORRECT_PWD = "Некорректный пароль.";
const char *MSG_ERR_CONN_LOAD = "Невозможно загрузить данные подключений.";
const char *MSG_NO_CONN_SEL = "Не выбрано подключение.";


void pageLogin_submit(Fl_Window *, Logic *);
void pageServ_submit(Fl_Window *, Logic *);


// Массив указателей на функции обработки нажатия кнопки
void (*wzd_submit_callbacks[]) (Fl_Window *, Logic *) = {
    pageLogin_submit,
    pageServ_submit
};


/**
 * Обработка ввода данных текущей вкладки визарда
 */
void btnSubmit_Click(Fl_Widget *w, void *context)
{
    Fl_Window *win = static_cast<Fl_Window *>(w->parent());
    Fl_Wizard *wzd = static_cast<Fl_Wizard *>(win->child(0));
    Logic *logic = reinterpret_cast<Logic *>(context);

    int page_index = wzd->find(wzd->value());
    // Вызов функции в зависимости от текущей вкладки визарда
    wzd_submit_callbacks[page_index] (win, logic);
}


/**
 * Обработка для страницы данных пользователя
 */
void pageLogin_submit(Fl_Window *win, Logic *logic)
{
    Fl_Wizard *wzd = static_cast<Fl_Wizard *>(win->child(0));

    Page_Login *pgLogin = static_cast<Page_Login *>(wzd->child(0));
    Fl_Input *inpLogin = static_cast<Fl_Input *>(pgLogin->child(0));
    Fl_Input *inpPwd = static_cast<Fl_Input *>(pgLogin->child(1));

    Page_Serv *pgServ = static_cast<Page_Serv *>(wzd->child(1));
    Fl_Pack *pckPanel = static_cast<Fl_Pack *>(pgServ->child(0));

    Fl_Check_Button *chkTerm = static_cast<Fl_Check_Button *>(win->child(1));
    Fl_Box *boxMsg = static_cast<Fl_Box *>(win->child(3));

    // Функция проверки строки ввода
    auto check_input = [] (const char *s) -> bool {
        std::locale c_loc("C");
        int n = 0;      // Количество символов
        for ( ; *s; ++s, ++n) {
            if (!std::isalnum(*s, c_loc))
                return false;
        }
        return n > 0;
    };

    // Проверка ввода
    if (!check_input(inpLogin->value())) {
        boxMsg->label(MSG_INCORRECT_NAME);
        return;
    }
    if (!check_input(inpPwd->value())) {
        boxMsg->label(MSG_INCORRECT_PWD);
        return;
    }

    // Установка значений
    logic->username(inpLogin->value());
    logic->pwd(inpPwd->value());
    logic->term(static_cast<bool>(chkTerm->value()));

    // Запись в лог
    std::string msgLogged("Logged as ");
    msgLogged += logic->username();
    logic->write_to_log(msgLogged);

    // Загрузка данных подключений
    if (!logic->get_conns()) {
        boxMsg->label(MSG_ERR_CONN_LOAD);
        return;
    }

    boxMsg->label("");

    // Добавление виджета для каждого подключения на страницу выбора подключений
    pckPanel->begin();
    for (size_t i = 0; i < logic->conns_count(); ++i) {
        Fl_Check_Button *chkServ =
                new Fl_Check_Button(40, 30 * i, 160, 30, logic->conn_title(i).c_str());
        chkServ->user_data(reinterpret_cast<void *>(i));
    }
    pckPanel->end();

    // Переключение вкладки визарда
    wzd->next();
}


/**
 * Обработка для страницы выбора подключений
 */
void pageServ_submit(Fl_Window *win, Logic *logic)
{
    Fl_Wizard *wzd = static_cast<Fl_Wizard *>(win->child(0));

    Page_Serv *pgServ = static_cast<Page_Serv *>(wzd->child(1));
    Fl_Pack *pckPanel = static_cast<Fl_Pack *>(pgServ->child(0));

    Fl_Box *boxMsg = static_cast<Fl_Box *>(win->child(3));

    // Извлечение данных подключений
    int selected_conns = 0;
    for (int i = 0; i < pckPanel->children(); ++i) {
        Fl_Check_Button *chkServ = static_cast<Fl_Check_Button *>(pckPanel->child(i));

        bool checked = static_cast<bool>(chkServ->value());
        // Если подключение отмечено...
        if (checked) {
            ++selected_conns;
            size_t conn_key = reinterpret_cast<size_t>(chkServ->user_data());
            // ...запустить скрипт
            logic->establish_conn(conn_key);
        }
    }

    // Если ни одно подключение не выбрано, вывести сообщение
    if (!selected_conns) {
        boxMsg->label(MSG_NO_CONN_SEL);
        return;
    }

    // Сокрытие главного окна
    logic->write_to_log("Done.");
    win->hide();
}


} // namespace MultiLogin
