/*
 * Copyright (c) 2006-2016, Guillaume Gimenez <guillaume@blackmilk.fr>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of G.Gimenez nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL G.Gimenez BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors:
 *     * Guillaume Gimenez <guillaume@blackmilk.fr>
 *
 */
#include "preferences.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QPalette>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    init_platform();
    QApplication a(argc, argv);
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QCoreApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);
#endif
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    dflOriginalPalette = QApplication::palette(static_cast<const QWidget *>(NULL));
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString(":/l10n/darkflow_") + locale);
    a.installTranslator(&translator);

    dflMainWindow = new MainWindow();
    if (argc == 2)
        dflMainWindow->load(argv[1]);

    dflMainWindow->show();

    return a.exec();
}
