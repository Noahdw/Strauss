#include "pianorollhelperview.h"
#include "Controllers/controlchangecontainer.h"

PianoRollHelperView::PianoRollHelperView(ControlChangeContainer *controlChangeContainer)
{
    setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed );
    setMinimumHeight(200);
    _controlChangeContainer = controlChangeContainer;

    vLayout = new QVBoxLayout;
    comboBox = new QComboBox;
    switchLayoutButton = new QPushButton("Switch view");
    comboBox->setMaxVisibleItems(16);
    setLayout(vLayout);
    initializeComboBox();
    vLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    vLayout->addWidget(comboBox);
    vLayout->addWidget(switchLayoutButton);
    connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index){ comboBoxIdChanged(index);}); // wtf
    connect(switchLayoutButton,&QPushButton::clicked,this,&PianoRollHelperView::switchView);
}

void PianoRollHelperView::comboBoxIdChanged(int index)
{
    qDebug() << index;
    _controlChangeContainer->switchControlChange(index);
   // _controlChangeContainer->switchControlChangeContainer(true);
}

void PianoRollHelperView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(QColor(96,96,96));
    painter.setBrush(brush);
    painter.drawRect(0,0,width() - 1,height()-1);
    brush.setColor((QColor(100, 50, 137)));
    painter.setBrush(brush);
    painter.drawRect(0,0,width() - 1,height() - vLayout->contentsRect().height() + vLayout->spacing()*2);
}

void PianoRollHelperView::initializeComboBox()
{
    for (int i = 0; i < 128; ++i)
    {
        QString type = "";
        switch (i)
        {
        case 0:
            type = "Bank Select";
            break;
        case 1:
            type = "Modulation";
            break;
        case 2:
            type = "Breath Controller";
            break;
        case 3:
            type = "";
            break;
        case 4:
            type = "Foot Controller";
            break;
        case 5:
            type = "Portamento Time";
            break;
        case 6:
            type = "MSB";
            break;
        case 7:
            type = "Volume";
            break;
        case 8:
            type = "Balance";
            break;
        case 9:
            type = "";
            break;
        case 10:
            type = "Pan";
            break;
        case 11:
            type = "Expression";
            break;
        default:
            break;
        }
        comboBox->addItem(QString::number(i) + " " + type);
    }
}

void PianoRollHelperView::switchView()
{
    //_controlChangeContainer->switchControlChangeContainer();
}

