#include "photo.h"
#include <QFile>
#include <QString>
#include <Magick++.h>

#include <string>

using namespace Magick;

Photo::Photo(QObject *parent) :
    QObject(parent),
    m_image(new Image),
    m_error(false)
{
}

Photo::Photo(const Blob &blob, QObject *parent) :
    QObject(parent),
    m_image(NULL),
    m_error(false)
{
    try {
        m_image = new Magick::Image(blob);
    }
    catch (std::exception *e) {
        qWarning(e->what());
        delete e;
        setError();
    }
}

Photo::Photo(const Magick::Image *image, QObject *parent) :
    QObject(parent),
    m_image(NULL),
    m_error(false)
{
    try {
        m_image = new Magick::Image(*image);
    }
    catch (std::exception *e) {
        qWarning(e->what());
        delete e;
        setError();
    }
}

Photo::Photo(const Photo &photo) :
    QObject(photo.parent()),
    m_image(NULL),
    m_error(false)
{
    try {
        m_image = new Magick::Image(*photo.m_image);
    }
    catch (std::exception *e) {
        qWarning(e->what());
        delete e;
        setError();
    }
}

Photo::~Photo()
{
    delete m_image;
}

Photo &Photo::operator=(const Photo &photo)
{
    Magick::Image *newImage;
    try {
        newImage = new Magick::Image(*photo.m_image);
        m_image = newImage;
    }
    catch (std::exception *e) {
        qWarning(e->what());
        delete e;
        setError();
    }
    return *this;
}

bool Photo::load(const QString &filename)
{
    QFile file(filename);
    if ( !file.open(QFile::ReadOnly) )
        return false;
    QByteArray data = file.readAll();
    Blob blob(data.data(), data.length());
    try {
        Image *newImage = new Magick::Image(blob);
        delete m_image;
        m_image = newImage;
    }
    catch (std::exception *e) {
        qWarning(e->what());
        delete e;
        return false;
    }
    return true;
}

bool Photo::save(const QString &filename, const QString &magick)
{
    Blob blob;
    try {
        m_image->write(&blob, magick.toStdString());
    }
    catch (std::exception *e) {
        qWarning(e->what());
        delete e;
        setError();
        return false;
    }

    QFile file(filename);
    file.open(QFile::WriteOnly);
    if ( !file.write(reinterpret_cast<const char*>(blob.data()),blob.length()) ) {
        setError();
        return false;
    }
    return true;
}

void Photo::create(long width, long height)
{
    try {
        delete m_image;
        m_image = new Magick::Image(Geometry(width,height),Color(0,0,0));
    }
    catch (std::exception *e) {
        qWarning(e->what());
        delete e;
        setError();
    }
}

void Photo::createAlike(const Photo *photo)
{
    create(photo->m_image->columns(), photo->m_image->rows());
}

bool Photo::error() const
{
    return m_error;
}

Magick::Image *Photo::image() const
{
    return m_image;
}

void Photo::setError()
{
    m_error = true;
    delete m_image;
    m_image = NULL;
}



