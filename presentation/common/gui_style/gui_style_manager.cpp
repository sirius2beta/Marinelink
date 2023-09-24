#include "gui_style_manager.h"

// Qt
//#include <QQmlContext>
#include <QVariant>
#include <QDebug>


// Industrial
//#include "theme.h"
//#include "theme_configurator.h"

// Internal
#include "settings_provider.h"
//#include "presentation_context.h"

using namespace presentation;

GuiStyleManager::GuiStyleManager(QObject* parent):
    QObject(parent)
//    ,m_configurator(new ThemeConfigurator(this))
{
//    presentationContext->rootContext()->setContextProperty(
//                "industrial", QVariant::fromValue(m_configurator->theme()));
}

GuiStyleManager::~GuiStyleManager()
{
}

void GuiStyleManager::setPaletteStyle(PaletteStyle paletteStyle)
{

    switch (paletteStyle) {
    case Day:

        break;
    case Night:
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(25,25,25));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        //Palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor(200, 200, 200));

        palette.setColor(QPalette::HighlightedText, Qt::black);
        palette.setColor(QPalette::Disabled, QPalette::Button,QColor(50,50,50));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText,QColor(150,150,150));

        palette.setColor(QPalette::Base,QColor(45,45,45));

        break;
    }
}

void GuiStyleManager::loadSettingsPalette()
{
    this->setPaletteStyle(PaletteStyle(settings::Provider::value(settings::gui::paletteStyle).toInt()));
}

void GuiStyleManager::setSizings(int baseSize)
{
//    m_configurator->setBaseSize(baseSize);
}

void GuiStyleManager::loadSettingsSizings()
{
    this->setSizings(settings::Provider::value(settings::gui::uiSize).toInt());
}

QPalette GuiStyleManager::getPallete()
{
    return palette;
}
