#include <cstdio>
#include "console.h"
#include "ui_console.h"
#include <QDateTime>
#include "darkflow.h"

Console *console = NULL;

Console::Console(QWidget *parent) :
    QMainWindow(parent),
    m_level(Info),
    m_raiseLevel(Error),
    m_trapLevel(LastLevel),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(DF_ICON));
    ui->textEdit->setStyleSheet("QTextEdit { background-color: black }");
    connect(this, SIGNAL(message(Level, QString)), this, SLOT(recvMessage(Level, QString)), Qt::QueuedConnection);
}

Console::~Console()
{
    delete ui;
}

void Console::init()
{
    qRegisterMetaType<Level>("Level");
    console = new Console();
    dflInfo(tr("Darkflow Started!"));
}

void Console::fini()
{
    console->hide();
    delete console;
    console = NULL;
}

void Console::show()
{
    console->QMainWindow::show();
    console->raise();
}

void Console::close()
{
    console->QMainWindow::close();
}

Console::Level Console::getLevel()
{
    return console->m_level;
}

void Console::setLevel(Console::Level level)
{
    console->m_level = level;
}

void Console::setTrapLevel(Console::Level level)
{
    console->m_trapLevel = level;
}

void Console::setRaiseLevel(Console::Level level)
{
    console->m_raiseLevel = level;
}

void Console::trap(Console::Level level)
{
    if ( level >= console->m_trapLevel )
        DF_TRAP();
}

void Console::recvMessage(Console::Level level, QString message)
{
    if ( level < m_level )
        return;
    ui->textEdit->setTextBackgroundColor(Qt::black);
    switch(level) {
    case Console::Debug:
        ui->textEdit->setTextColor(Qt::darkYellow);
        break;
    case Console::Info:
        ui->textEdit->setTextColor(Qt::green);
        break;
    case Console::Warning:
        ui->textEdit->setTextColor(Qt::magenta);
        break;
    case Console::Error:
        ui->textEdit->setTextColor(Qt::red);
        message = message;
        break;
    default:
        message = tr("Unknown LogLevel!: %0").arg(message);
    case Console::Critical:
        ui->textEdit->setTextBackgroundColor(Qt::red);
        ui->textEdit->setTextColor(Qt::white);
        show();
        break;
    }
    if ( level >= m_raiseLevel ) {
        show();
        raise();
    }
    ui->textEdit->append(QDateTime::currentDateTime().toString("yyyy/MM/dd-HH:mm:ss")+": "+message);
}

static void message(Console::Level level, const char *fmt, va_list ap)
{
    Console::trap(level);
    char *msg;
    vasprintf(&msg, fmt, ap);
    emit console->message(level, msg);
    free(msg);
}

void dflMessage(Console::Level level, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    message(level, fmt, ap);
    va_end(ap);
}

void dflDebug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    message(Console::Debug, fmt, ap);
    va_end(ap);
}

void dflInfo(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    message(Console::Info, fmt, ap);
    va_end(ap);
}

void dflWarning(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    message(Console::Warning, fmt, ap);
    va_end(ap);
}
void dflError(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    message(Console::Error, fmt, ap);
    va_end(ap);
}
void dflCritical(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    message(Console::Critical, fmt, ap);
    va_end(ap);
}

void dflMessage(Console::Level level, const QString& msg) {
    Console::trap(level);
    emit console->message(level, msg);
}

void dflDebug(const QString &msg)
{
    dflMessage(Console::Debug, msg);
}

void dflInfo(const QString &msg)
{
    dflMessage(Console::Info, msg);
}

void dflWarning(const QString &msg)
{
    dflMessage(Console::Warning, msg);
}

void dflError(const QString &msg)
{
    dflMessage(Console::Error, msg);
}

void dflCritical(const QString &msg)
{
    dflMessage(Console::Critical, msg);
}
