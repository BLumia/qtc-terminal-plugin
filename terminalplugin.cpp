/*
 * Copyright (C) 2012 Adam Treat. All rights reserved.
 */

#include "terminalplugin.h"
#include "terminalwindow.h"

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/id.h>
#include <extensionsystem/pluginmanager.h>

#include <QtCore/QDebug>
#include <QtCore/QtPlugin>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>

namespace Terminal {
namespace Internal {


/*! Constructs the Terminal plugin. Normally plugins don't do anything in
    their constructor except for initializing their member variables. The
    actual work is done later, in the initialize() and extensionsInitialized()
    methods.
*/
TerminalPlugin::TerminalPlugin()
{
}

/*! Plugins are responsible for deleting objects they created on the heap, and
    to unregister objects from the plugin manager that they registered there.
*/
TerminalPlugin::~TerminalPlugin()
{
    ExtensionSystem::PluginManager::instance()->removeObject(m_window.data());
}

/*! Initializes the plugin. Returns true on success.
    Plugins want to register objects with the plugin manager here.

    \a errorMessage can be used to pass an error message to the plugin system,
       if there was any.
*/
bool TerminalPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    m_window.reset(new TerminalWindow(this));
    ExtensionSystem::PluginManager::instance()->addObject(m_window.data());

    QAction * m_copy = new QAction("Copy", this);
    Core::Command *copyCmd = Core::ActionManager::registerAction(m_copy, "PineappleTerminal.Copy",
                                                                Core::Context(Core::Constants::C_GENERAL_OUTPUT_PANE));
    copyCmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+C")));

    QAction * m_paste = new QAction("Paste", this);
    Core::Command *pasteCmd = Core::ActionManager::registerAction(m_paste, "PineappleTerminal.Paste",
                                                                Core::Context(Core::Constants::C_GENERAL_OUTPUT_PANE));
    pasteCmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+V")));


    return true;
}

/*! Notification that all extensions that this plugin depends on have been
    initialized. The dependencies are defined in the plugins .pluginspec file.

    Normally this method is used for things that rely on other plugins to have
    added objects to the plugin manager, that implement interfaces that we're
    interested in. These objects can now be requested through the
    PluginManagerInterface.

    The TerminalPlugin doesn't need things from other plugins, so it does
    nothing here.
*/
void TerminalPlugin::extensionsInitialized()
{
}

} // namespace Internal
} // namespace Terminal
