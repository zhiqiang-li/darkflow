#include "openhance.h"
#include "operatorworker.h"
#include "operatorinput.h"
#include "operatoroutput.h"
#include <Magick++.h>

class WorkerEnhance : public OperatorWorker {
public:
    WorkerEnhance(QThread *thread, Operator *op) :
        OperatorWorker(thread, op)
    {}
    Photo process(const Photo& photo, int, int) {
        Photo newPhoto(photo);
        newPhoto.image().enhance();
        return newPhoto;
    }
};

OpEnhance::OpEnhance(Process *parent) :
    Operator(OP_SECTION_COSMETIC, QT_TRANSLATE_NOOP("Operator", "Enhance"), Operator::NonHDR, parent)
{
    addInput(new OperatorInput(tr("Images"),OperatorInput::Set, this));
    addOutput(new OperatorOutput(tr("Images"), this));

}

OpEnhance *OpEnhance::newInstance()
{
    return new OpEnhance(m_process);
}

OperatorWorker *OpEnhance::newWorker()
{
    return new WorkerEnhance(m_thread, this);
}
