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
#include "opturnblack.h"
#include "operatorworker.h"
#include "operatorinput.h"
#include "operatoroutput.h"
#include <Magick++.h>

class WorkerTurnBlack : public OperatorWorker {
public:
    WorkerTurnBlack(QThread *thread, Operator *op) :
        OperatorWorker(thread, op)
    {}
    Photo process(const Photo &photo, int, int) {
        Photo newPhoto(photo);
        Magick::Image &image = newPhoto.image();
        ResetImage(image);
        int w = image.columns(),
            h = image.rows();
        std::shared_ptr<Ordinary::Pixels> cache(new Ordinary::Pixels(image));
        dfl_parallel_for(y, 0, h, 4, (image), {
                             Magick::PixelPacket *pixels = cache->get(0, y, w, 1);
                             for (int x = 0 ; x < w ; ++x) {
                                 pixels[x].red = pixels[x].green = pixels[x].blue = 0;
                             }
                             cache->sync();
                         });
        return newPhoto;
    }
};

OpTurnBlack::OpTurnBlack(Process *parent) :
    Operator(OP_SECTION_MASK, QT_TRANSLATE_NOOP("Operator", "Turn black"), Operator::All, parent)
{
    addInput(new OperatorInput(tr("Images"),OperatorInput::Set, this));
    addOutput(new OperatorOutput(tr("Black frame"), this));
}

OpTurnBlack *OpTurnBlack::newInstance()
{
    return new OpTurnBlack(m_process);
}

OperatorWorker *OpTurnBlack::newWorker()
{
    return new WorkerTurnBlack(m_thread, this);
}
