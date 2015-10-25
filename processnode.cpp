#include <QPainterPath>
#include <QPainter>
#include <QString>
#include <QGraphicsTextItem>
#include <QCursor>

#include "process.h"
#include "processnode.h"
#include "processbutton.h"
#include "processport.h"
#include "processprogressbar.h"
#include "operator.h"
#include "operatorinput.h"

#include "operatorparameterdropdown.h"
#include "processdropdown.h"

#include "operatorparameterfilescollection.h"
#include "processfilescollection.h"

#define PEN_WIDTH 2

ProcessNode::ProcessNode(QPointF pos,
                         Operator *op,
                         Process *process,
                         QGraphicsItem *parent) :
    QObject(NULL),
    QGraphicsPathItem(parent),
    m_operator(op),
    m_process(process),
    m_caption(NULL)
{
    qreal barHeight;
    qreal x = pos.x();
    qreal y = pos.y();
    QPainterPath pp;
    qreal w = 200, h;
    qreal xradius=3, yradius=3;

    QVector<OperatorInput*> inputs = op->getInputs();
    QVector<OperatorParameter*> parameters = op->getParameters();

    int portRowsCount = inputs.count();
    int parameterRowsCount = parameters.count();
    if (0 == portRowsCount)
        portRowsCount = 1; //always an output

    m_caption = new QGraphicsTextItem(this);
    m_caption->setPlainText(op->getClassIdentifier());
    m_caption->setPos(x, y);
    barHeight = m_caption->boundingRect().height();
    h = barHeight*(2+portRowsCount+parameterRowsCount);

    setPen(QPen(Qt::black,PEN_WIDTH));
    setBrush(QBrush(Qt::darkGray));
    pp.addRoundedRect(x, y, w, h, xradius, yradius);

    setFlag(QGraphicsItem::ItemIsMovable);


    pp.addRect(x,y+barHeight-PEN_WIDTH,w,0);
    setPath(pp);
    setCursor(QCursor(Qt::ArrowCursor));


    addPorts(inputs, barHeight);
    addButtons(barHeight);
    addParameters(parameters, barHeight, (1+portRowsCount)*barHeight);

    connect(m_process, SIGNAL(stateChanged()), this, SLOT(operatorStateChanged()));
}

void ProcessNode::addParameters(QVector<OperatorParameter*>& parameters, qreal size, qreal offset)
{
    qreal x = boundingRect().x();
    qreal y = boundingRect().y();
    qreal w = boundingRect().width();

    for (int i=0 ; i < parameters.count() ; ++i ) {
        if ( OperatorParameterDropDown *dropdown = dynamic_cast<OperatorParameterDropDown*>(parameters[i]) )
        {
            new ProcessDropDown(QRectF(x,y+size*i+offset, w, size),dropdown, m_process, this);
        }
        else if ( OperatorParameterFilesCollection *filesCollection = dynamic_cast<OperatorParameterFilesCollection*>(parameters[i]) )
        {
            new ProcessFilesCollection(QRectF(x,y+size*i+offset, w, size),filesCollection, m_process, this);
        }
    }
}

void ProcessNode::addButtons(qreal size)
{
    qreal x = boundingRect().x();
    qreal y = boundingRect().y();
    qreal w = boundingRect().width();
    qreal h = boundingRect().height();

    ProcessButton *buttClose = new ProcessButton(QRectF(x+w-size,y,size,size),
                                                 ProcessButton::Close,m_process, this);
    connect(buttClose, SIGNAL(buttonClicked()), this, SLOT(closeButtonClicked()));

    ProcessButton *buttPassThrough= new ProcessButton(QRectF(x+w-size*2,y,size,size),
                                                      ProcessButton::Ghost,m_process, this);
    connect(buttPassThrough, SIGNAL(buttonClicked()), this, SLOT(passThroughClicked()));

    ProcessButton *buttViewImage= new ProcessButton(QRectF(x+w-size*3,y,size,size),
                                                    ProcessButton::Display,m_process, this);
    connect(buttViewImage, SIGNAL(buttonClicked()), this, SLOT(viewImageClicked()));

    ProcessButton *buttPlay= new ProcessButton(QRectF(x,y+h-size,size,size),
                                               ProcessButton::Play,m_process, this);
    connect(buttPlay, SIGNAL(buttonClicked()), this, SLOT(playClicked()));

    ProcessButton *buttAbort= new ProcessButton(QRectF(x+size,y+h-size,size,size),
                                                ProcessButton::Abort,m_process, this);
    connect(buttAbort, SIGNAL(buttonClicked()), this, SLOT(abortClicked()));

    ProcessProgressBar *progress = new ProcessProgressBar(QRectF(x+size*2, y+h-size, w-size*2, size), m_process, this);
    connect(m_operator, SIGNAL(progress(int,int)), progress, SLOT(progress(int,int)));
}
void ProcessNode::addPorts(QVector<OperatorInput*>& inputs, qreal size)
{
    qreal x = boundingRect().x();
    qreal y = boundingRect().y();
    qreal w = boundingRect().width();

    for (int i=0 ; i < inputs.count(); ++i)
    {
        new ProcessPort(QRectF(x,y+size*(i+1),w,size),inputs[i]->name(),
                        ProcessPort::InputPort, m_process, this);
    }
    new ProcessPort(QRectF(x,y+size,w,size),"Out", ProcessPort::OutputPort, m_process, this);
}

ProcessNode::~ProcessNode()
{
    //disconnect(m_process, SIGNAL(stateChanged()), this, SLOT(operatorStateChanged()));
    delete m_operator;
}

void ProcessNode::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawPath(path());
}

int ProcessNode::type() const { return Type; }

void ProcessNode::operatorStateChanged()
{

}

void ProcessNode::closeButtonClicked()
{
    deleteLater();
}

void ProcessNode::passThroughClicked()
{

}

void ProcessNode::viewImageClicked()
{

}

void ProcessNode::viewParametersClicked()
{

}

void ProcessNode::playClicked()
{
    m_operator->play();
}

void ProcessNode::abortClicked()
{
    m_operator->abort();
}
