#ifndef _application_h_
#define _application_h_


#include <memory>


namespace MultiLogin {


struct ApplicationImplementation;
/**
 * @class Application
 * Класс приложения.
 */
class Application
{
public:
    Application();
    ~Application();

    /**
     * @brief Прорисовка графического интерфейса
     */
    void draw_gui();

private:
    std::unique_ptr<ApplicationImplementation> m;
};


} // namespace MultiLogin


#endif // _application_h_
