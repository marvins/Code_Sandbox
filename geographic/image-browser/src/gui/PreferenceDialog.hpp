/**
 * @file   PreferenceDialog.hpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#ifndef __SRC_GUI_PREFERENCEDIALOG_HPP__
#define __SRC_GUI_PREFERENCEDIALOG_HPP__

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

/**
 * @class PreferenceDialog
 */
class PreferenceDialog : public QDialog{

    Q_OBJECT

    public:

        /**
         * Default Constructor
         */
        PreferenceDialog( QWidget* parent = NULL );

    private:

        /// Main Layout
        QVBoxLayout* mainLayout;

        /// Main Label
        QLabel*      mainLabel;


};

#endif
