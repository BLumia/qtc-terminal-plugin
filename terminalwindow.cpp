/*
 * Copyright (C) 2012 Adam Treat. All rights reserved.
 */

#include "terminalwindow.h"

#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icontext.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorsettings.h>
#include <utils/environment.h>
#include <utils/fileutils.h>

#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QVBoxLayout>

#include <qtermwidget.h>

namespace Terminal {
namespace Internal {

TerminalContainer::TerminalContainer(QWidget *parent)
    : QWidget(parent)
    , m_parent(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &TerminalContainer::contextMenuRequested);

    Core::Command *copyCmd = Core::ActionManager::command("PineappleTerminal.Copy");
    Core::Command *pasteCmd = Core::ActionManager::command("PineappleTerminal.Paste");

    m_copy = new QAction("&Copy", this);
    m_copy->setShortcut(copyCmd->keySequence());
    connect(m_copy, &QAction::triggered, this, &TerminalContainer::copyInvoked);
    addAction(m_copy);

    m_paste = new QAction("&Paste", this);
    m_paste->setShortcut(pasteCmd->keySequence());
    connect(m_paste, &QAction::triggered, this, &TerminalContainer::pasteInvoked);
    addAction(m_paste);

    m_autoCd = new QAction("Change &Directory", this);
    connect(m_autoCd, &QAction::triggered, this, &TerminalContainer::autoCdInvoked);
    addAction(m_autoCd);

    m_close = new QAction("Close", this);
    m_close->setShortcut(QKeySequence::Close);
    connect(m_close, &QAction::triggered, this, &TerminalContainer::closeInvoked);
    addAction(m_close);

    initializeTerm();
}

void TerminalContainer::initializeTerm()
{
    if (m_termWidget) {
        delete m_layout;
        delete m_termWidget;
    }

    Utils::Environment env = Utils::Environment::systemEnvironment();

    m_termWidget = new QTermWidget(0, this);
    m_termWidget->setWindowTitle(tr("Terminal"));
    m_termWidget->setWindowIcon(QIcon());
    m_termWidget->setScrollBarPosition(QTermWidget::ScrollBarRight);
//    qDebug() << m_termWidget->availableColorSchemes();
    // set shell program
    QString shell { env.value("SHELL") };
    m_termWidget->setShellProgram(shell.isEmpty() ? "/bin/bash" : shell);
#if defined(Q_OS_LINUX)
    m_termWidget->setColorScheme("Linux");
    m_termWidget->setKeyBindings("linux");
#elif defined(Q_OS_MACX)
    m_termWidget->setColorScheme("WhiteOnBlack");
    m_termWidget->setKeyBindings("macbook");
#else
    m_termWidget->setColorScheme("BlackOnLightYellow");
    m_termWidget->setKeyBindings("default");
#endif
    QFont font = TextEditor::TextEditorSettings::instance()->fontSettings().font();
    m_termWidget->setTerminalFont(font);
    m_termWidget->setTerminalOpacity(1.0);

    setFocusProxy(m_termWidget);

    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_termWidget);
    setLayout(m_layout);

    connect(m_termWidget, &QTermWidget::copyAvailable, this, &TerminalContainer::copyAvailable);
    connect(m_termWidget, &QTermWidget::finished, this, &TerminalContainer::finishedInvoked);

    ProjectExplorer::Project * proj = ProjectExplorer::ProjectTree::currentProject();
    QString path = proj ? proj->projectDirectory().toString() : QDir::currentPath();
    m_termWidget->setWorkingDirectory(path);

    env.set("TERM_PROGRAM", QString("qtermwidget5"));
    env.set("TERM", QString("xterm-256color"));
    env.set("QTCREATOR_PID", QString("%1").arg(QCoreApplication::applicationPid()));
    m_termWidget->setEnvironment(env.toStringList());
    m_termWidget->startShellProgram();
//    emit termInitialized();
}

void TerminalContainer::contextMenuRequested(const QPoint &point)
{
    QPoint globalPos = mapToGlobal(point);
    QMenu menu;
    menu.addAction(m_copy);
    menu.addAction(m_paste);
    menu.addAction(m_autoCd);
    menu.addAction(m_close);
    menu.exec(globalPos);
}

void TerminalContainer::copyInvoked()
{
//    QMenu menu;
//    menu.addAction(m_copy);
//    menu.addAction(m_paste);
//    menu.addAction(m_close);
//    menu.exec();
    m_termWidget->copyClipboard();
//    QKeyEvent * key = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier);
////    QCoreApplication::postEvent(m_termWidget, key);
//    m_termWidget->sendKeyEvent(key);
}

void TerminalContainer::pasteInvoked()
{
    m_termWidget->pasteClipboard();
}

void TerminalContainer::autoCdInvoked()
{
    ProjectExplorer::Project * proj = ProjectExplorer::ProjectTree::currentProject();
    if (proj) {
        termWidget()->changeDir(proj->projectDirectory().toString());
        return;
    }

    if (Core::IDocument *doc = Core::EditorManager::instance()->currentDocument()) {
        const QDir dir = doc->filePath().toFileInfo().absoluteDir();
        if (dir.exists())
            termWidget()->changeDir(dir.canonicalPath());
        return;
    }

    termWidget()->changeDir(QDir::currentPath());
}

void TerminalContainer::copyAvailable(bool available)
{
    m_copy->setEnabled(available);
}

void TerminalContainer::closeInvoked()
{
    QString cmd = "exit\n";
    m_termWidget->sendText(cmd);
}

void TerminalContainer::finishedInvoked()
{
    initializeTerm();
}

TerminalWindow::TerminalWindow(QObject *parent)
   : IOutputPane(parent)
   , m_terminalContainer(nullptr)
{
    Core::Context context("Terminal.Window");
}

QWidget *TerminalWindow::outputWidget(QWidget *parent)
{
    if (!m_terminalContainer)
        m_terminalContainer = new TerminalContainer(parent);
    return m_terminalContainer;
}

QList<QWidget *> TerminalWindow::toolBarWidgets() const
{
    return QList<QWidget *>();
}

QString TerminalWindow::displayName() const
{
    return tr("Terminal");
}

int TerminalWindow::priorityInStatusBar() const
{
    return 50;
}

void TerminalWindow::clearContents()
{
    if (!m_terminalContainer || !m_terminalContainer->termWidget())
        return;
    QString cmd = "clear\n";
    m_terminalContainer->termWidget()->sendText(cmd);
}

void TerminalWindow::visibilityChanged(bool visible)
{
    static bool initialized = false;
    if (!m_terminalContainer || !m_terminalContainer->termWidget() || initialized || !visible)
        return;

    m_terminalContainer->initializeTerm();
    initialized = true;
}

void TerminalWindow::setFocus()
{
    if (!m_terminalContainer || !m_terminalContainer->termWidget())
        return;
    m_terminalContainer->setFocus(Qt::OtherFocusReason);
}

bool TerminalWindow::hasFocus() const
{
    if (!m_terminalContainer || !m_terminalContainer->termWidget())
        return false;
    return m_terminalContainer->hasFocus();
}

bool TerminalWindow::canFocus() const
{
    return true;
}

bool TerminalWindow::canNavigate() const
{
    return false;
}

bool TerminalWindow::canNext() const
{
    return false;
}

bool TerminalWindow::canPrevious() const
{
    return false;
}

void TerminalWindow::goToNext()
{
    // no-op
}

void TerminalWindow::goToPrev()
{
    // no-op
}

} // namespace Internal
} // namespace Terminal
