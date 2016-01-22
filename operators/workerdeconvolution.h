#ifndef WORKERDECONVOLUTION_H
#define WORKERDECONVOLUTION_H

#include <operatorworker.h>

class OpDeconvolution;

class WorkerDeconvolution : public OperatorWorker
{
    Q_OBJECT
public:
    WorkerDeconvolution(qreal luminosity, QThread *thread, OpDeconvolution *op);
    Photo process(const Photo &, int, int);
    void play();
private:
    qreal m_luminosity;
    void deconv(Magick::Image &image, Magick::Image &kernel, qreal luminosity);
};

#endif // WORKERDECONVOLUTION_H
