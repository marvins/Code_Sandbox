/**
 * @file    Main_Window.hpp
 * @author  Marvin Smith
 * @date    9/29/2017
 */
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

// C++ Libraries
#include <string>


// Qt Libraries
#include <QMainWindow>
#include <QStackedWidget>



/**
 * @class Main_Window
 */
class Main_Window : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     */
    Main_Window( QWidget*  parent = nullptr );


protected:

    /**
     * @brief Initialize GUI
    */
    void Initialize_GUI();


private:

    /**
     * @brief Build Menu
     */
    void Build_Menu();


    /// Class Name
    std::string m_class_name;

	QStackedWidget* m_stack_widget;

}; // End of Main_Window Class

#endif
