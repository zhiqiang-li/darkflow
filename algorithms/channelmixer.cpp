#include "channelmixer.h"
#include "photo.h"
#include <Magick++.h>

ChannelMixer::ChannelMixer(qreal r, qreal g, qreal b, QObject *parent) :
    Algorithm(parent),
    m_rgb()
{
    m_rgb[0] = r;
    m_rgb[1] = g;
    m_rgb[2] = b;
}

void ChannelMixer::applyOnImage(Magick::Image &image)
{
    image.modifyImage();
    int h = image.rows(),
            w = image.columns();
    Magick::Pixels pixel_cache(image);
#pragma omp parallel for
    for (int y = 0 ; y < h ; ++y ) {
        Magick::PixelPacket *pixels = pixel_cache.get(0,y,w,1);
        if ( !pixels ) continue;
        for (int x = 0 ; x < w ; ++x ) {
            using Magick::Quantum;
            pixels[x].red=
            pixels[x].green=
            pixels[x].blue=clamp<double>(pixels[x].red*m_rgb[0]+pixels[x].green*m_rgb[1]+pixels[x].blue*m_rgb[2],0,QuantumRange);
        }
    }
    pixel_cache.sync();

}