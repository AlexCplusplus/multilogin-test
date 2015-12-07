#include <vector>
#include <set>
#include <fstream>
#include <sstream>

#include "logic.h"


namespace MultiLogin {


/**
 * Структура, хранящая данные подключения
 */
struct conn_opts
{
    std::string address;
    std::string color;
    std::string title;
};


/**
 * Внутренняя структура класса
 */
struct LogicImplementation
{
    LogicImplementation();

    bool get_conn_addresses(std::set<std::string> &);
    bool get_conn_info(const std::set<std::string> &);

    void write_to_log(const std::string &);

    std::string uname;
    std::string upwd;
    bool term;

    std::vector<conn_opts> conns;
    std::fstream log;
};


LogicImplementation::LogicImplementation()
    : uname(),
      upwd(),
      term(false),
      conns(),
      log()
{
    log.open("tmp/multilogin.log", std::ios_base::out | std::ios_base::app);
}


/**
 * Загрузка адресов подключений
 */
bool LogicImplementation::get_conn_addresses(std::set<std::string> &conn_addresses)
{
    // Файл адресов подключений
    std::string hosts_filename("/root/.ssh/known_hosts");
    std::ifstream hosts_file(hosts_filename, std::ios_base::in);
    if (!hosts_file.is_open()) {
        write_to_log(hosts_filename + ": can not open file.");
        return false;
    }

    std::string buff;
    while (hosts_file.good()) {
        // Чтение файла по строкам
        getline(hosts_file, buff, '\n');
        if (!buff.empty()) {
            // Извлечение подстроки адреса
            std::string ip = buff.substr(0, buff.find(' '));
            if (!ip.empty())
                conn_addresses.insert(ip);
        }
    }

    // Если адреса подключений не загружены, сделать запись в лог
    if (conn_addresses.empty()) {
        write_to_log("No connection addresses loaded.");
        return false;
    }

    // Запись в лог об успешной загрузке
    write_to_log("Connection addresses loaded.");
    return true;
}

/**
 * Загрузка информации подключений
 */
bool LogicImplementation::get_conn_info(const std::set<std::string> &conn_addresses)
{
    // Файл информации подключений
    std::string label_filename("/opt/var/srv_color_label");
    std::ifstream label_file(label_filename, std::ios_base::in);
    if (!label_file.is_open()) {
        write_to_log(label_filename + ": can not open file.");
        return false;
    }

    conns.clear();

    std::string buff;
    while (label_file.good()) {
        // Чтение файла по строкам
        getline(label_file, buff, '\n');
        if (!buff.empty()) {
            // Извлечение подстрок адреса, цветовой метки и наименования
            size_t col_sep = buff.find('=');
            size_t title_sep = buff.find('=', col_sep + 1);
            std::string adr = buff.substr(0, col_sep);
            std::string col = buff.substr(col_sep + 1, title_sep - col_sep - 1);
            std::string title = buff.substr(title_sep + 1);

            if (adr.empty() || col.empty() || title.empty()) {
                write_to_log("Invalid connection info.");
                continue;
            }

            // Если адрес загружен, добавить структуру данных подключения в вектор
            auto itr = conn_addresses.find(adr);
            if (itr != conn_addresses.end()) {
                conn_opts info { adr, col, title };
                conns.push_back(info);
            }
        }
    }

    // Запись в лог
    write_to_log("Connections info loaded.");
    return true;
}

void LogicImplementation::write_to_log(const std::string &msg)
{
    log << __FILE__ << ":" << __LINE__ << ": " << msg << std::endl;
}


Logic::Logic()
    : m(nullptr)
{
    m.reset(new LogicImplementation);
}

Logic::~Logic() { }


const std::string &Logic::username() const
{ return m->uname; }
void Logic::username(const std::string &n)
{ m->uname = n; }

const std::string &Logic::pwd() const
{ return m->upwd; }
void Logic::pwd(const std::string &pw)
{ m->upwd = pw; }

bool Logic::term() const
{ return m->term; }
void Logic::term(bool t)
{ m->term = t; }

size_t Logic::conns_count() const
{ return m->conns.size(); }

const std::string &Logic::conn_address(size_t conn_key) const
{ return m->conns.at(conn_key).address; }
const std::string &Logic::conn_color(size_t conn_key) const
{ return m->conns.at(conn_key).color; }
const std::string &Logic::conn_title(size_t conn_key) const
{ return m->conns.at(conn_key).title; }

/**
 * Загрузить данные подключений
 */
bool Logic::get_conns()
{
    // Набор адресов подключений
    std::set<std::string> conn_addresses;
    if (m->get_conn_addresses(conn_addresses) && m->get_conn_info(conn_addresses)) {
        write_to_log("Connections data loaded.");
        return true;
    }

    write_to_log("An error occured on connections data loading.");
    return false;
}

/**
 * Запустить скрипт
 */
void Logic::establish_conn(size_t conn_key)
{
    // Неверный ключ
    if (!(conn_key < conns_count())) {
        write_to_log("Invalid connection key.");
        return;
    }

    // Структура данных подключения
    conn_opts info = m->conns.at(conn_key);

    // Создание командной строки
    std::stringstream cmd(std::ios_base::out);
    cmd << "sh /opt/bin/tl-client.sh " << username() << ' ' << pwd();
    if (term())
        cmd << " TERM";
    cmd << ' ' << info.color << " '" << info.title << "' "
        << info.address << " &";

    // Запись в лог
    std::string msgScriptCall("Calling: ");
    msgScriptCall += cmd.str();;
    write_to_log(msgScriptCall);

    // Запуск скрипта
    system(cmd.str().c_str());
}

/**
 * Записать сообщение в лог
 */
void Logic::write_to_log(const std::string &msg)
{
    m->write_to_log(msg);
}


} // namespace MultiLogin
