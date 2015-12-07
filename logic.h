#ifndef _logic_h_
#define _logic_h_


#include <memory>
#include <string>


class Fl_Group;
namespace MultiLogin {


struct LogicImplementation;
/**
 * @class Logic
 * Класс, представляющий логику приложения.
 */
class Logic
{
public:
    Logic();
    ~Logic();

    /**
     * @brief Имя пользователя
     */
    const std::string &username() const;
    void username(const std::string &);

    /**
     * @brief Пароль
     */
    const std::string &pwd() const;
    void pwd(const std::string &);

    /**
     * @brief Флаг завершения сессий
     */
    bool term() const;
    void term(bool);

    /**
     * @brief Количество доступных подключений
     */
    size_t conns_count() const;

    /**
     * @brief Получение адреса подключения
     */
    const std::string &conn_address(size_t conn_key) const;
    /**
     * @brief Получение наименования подключения
     */
    const std::string &conn_title(size_t conn_key) const;
    /**
     * @brief Получение цветовой метки подключения
     */
    const std::string &conn_color(size_t conn_key) const;

    /**
     * @brief Загрузка данных подключений
     */
    bool get_conns();
    /**
     * @brief Запуск скрипта
     */
    void establish_conn(size_t conn_key);

    /**
     * @brief Запись в лог
     */
    void write_to_log(const std::string &);

private:
    std::unique_ptr<LogicImplementation> m;
};


} // namespace MultiLogin


#endif // _logic_h_
