#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(960,423);
    this->setWindowTitle("Life");
    isStart = false;
    tempColor = new QColor*[CellCount];
    for(int row = 0; row < CellCount ; row++)
    {
        tempColor[row] = new QColor[2*CellCount];
    }
    currentColor = new QColor(Qt::black);


    QHBoxLayout* layout = new QHBoxLayout;
    table = new QTableWidget;
    table->setRowCount(CellCount);
    table->setColumnCount(2*CellCount);

    for(int row = 0; row < table->rowCount() ; row++)
    {
        for(int col = 0; col < table->columnCount(); col++)
        {
            QTableWidgetItem* it = new QTableWidgetItem();
            it->setFlags((it->flags() & ~Qt::ItemIsEditable));
            table->setItem(row, col, it);
            it->setBackgroundColor(QColor(Qt::white));

        }
    }

    for(int i = 0; i < table->rowCount(); i++)
    {
        table->setRowHeight(i, CellSize);
    }
    for(int i = 0; i < table->columnCount(); i++)
    {
        table->setColumnWidth(i,CellSize);
    }

    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    dial = new QDial;
    dial->setRange(50,350);
    dial->setValue(50);
    timer = new QTimer;
    numberDelay = new QLCDNumber;
    numberDelay->display(dial->value());
    numberGen = new QLCDNumber;
    start = new QPushButton("Start");
    reset = new QPushButton("Reset");

    classic = new QRadioButton("Classic");
    classic->setChecked(true);
    earth = new QRadioButton("Earth");
    QVBoxLayout* modeLayout = new QVBoxLayout;
    modeLayout->addWidget(classic);
    modeLayout->addWidget(earth);
    QGroupBox* modeGroup = new QGroupBox("Mode");
    modeGroup->setLayout(modeLayout);

    black  = new QRadioButton("Civilization");
    black->setChecked(true);
    green = new QRadioButton("Nature");
    QVBoxLayout* earthLayout = new QVBoxLayout;
    earthLayout->addWidget(black);
    earthLayout->addWidget(green);
    green->setEnabled(false);
    black->setEnabled(false);
    QGroupBox* earthGroup = new QGroupBox("Earth");
    earthGroup->setLayout(earthLayout);


    layout->addWidget(table);
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(start);
    vlayout->addWidget(reset);
    vlayout->addWidget(dial);

    vlayout->addWidget(numberDelay);
    vlayout->addWidget(modeGroup);
    vlayout->addWidget(earthGroup);
    vlayout->addWidget(numberGen);


    layout->addLayout(vlayout);
    this->setLayout(layout);

    QObject::connect(dial, SIGNAL(valueChanged(int)), numberDelay, SLOT(display(int)));
    QObject::connect(start, SIGNAL(clicked(bool)), this, SLOT(startSlot()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(newGeneration()));
    QObject::connect(table, SIGNAL(cellEntered(int,int)),this,SLOT(changeBackGround(int,int)));
    QObject::connect(table, SIGNAL(cellClicked(int,int)),this,SLOT(changeBackGround(int,int)));
    QObject::connect(reset, SIGNAL(clicked(bool)), this, SLOT(resetSlot()));
    QObject::connect(classic,SIGNAL(clicked(bool)),this, SLOT(activateClassic()));
    QObject::connect(earth,SIGNAL(clicked(bool)),this, SLOT(activateEarth()));
    QObject::connect(black,SIGNAL(clicked(bool)),this, SLOT(setDarkColor()));
    QObject::connect(green,SIGNAL(clicked(bool)),this, SLOT(setLightColor()));
}

Widget::~Widget()
{
    delete ui;
}

int Widget::countNeighbours(int i, int j)
{
    int counter = 0;

    for(int m = i-1; m <= i+1; m++)
    {
        for(int n = j-1; n <= j+1; n++)
        {
            int x = m;
            int y = n;

            if(x >= CellCount)
            {
                x -= CellCount;
            }

            if(y >= 2*CellCount)
            {
                y -= 2*CellCount;
            }

            if(x < 0)
            {
                x += CellCount;
            }

            if (y < 0)
            {
                y += 2*CellCount;
            }

            if((x != i || y != j) && x >= 0 && y >= 0 && x < CellCount && y < 2*CellCount
                    && table->item(x,y)->backgroundColor() == darkColor)
            {
                counter++;
            }

            else if((x != i || y != j) && x >= 0 && y >= 0 && x < CellCount && y < 2*CellCount
                    && table->item(x,y)->backgroundColor() == lightColor)
            {
                counter--;
            }
        }
    }

    return counter;
}

void Widget::checkGenEqual()
{
    bool result = true;
    for(int row = 0; row < table->rowCount() ; row++)
    {
        for(int col = 0; col < table->columnCount(); col++)
        {
            if (tempColor[row][col] != table->item(row,col)->backgroundColor())
            {
                result = false;
            }
        }
    }

}

void Widget::changeBackGround(int row, int column)
{
    QTableWidgetItem *it = table->item(row,column);
    it->setTextAlignment(Qt::AlignCenter);
    it->setFlags((it->flags() & ~Qt::ItemIsEditable));

    if(it->backgroundColor() == *(currentColor))
    {
        it->setBackgroundColor(QColor(Qt::white));
    }
    else if(it->backgroundColor() != *(currentColor) )
    {
        it->setBackgroundColor(*(currentColor));
    }
}

void Widget::resetSlot()
{
    for(int row = 0; row < table->rowCount() ; row++)
    {
        for(int col = 0; col < table->columnCount(); col++)
        {
            table->item(row,col)->setBackgroundColor(QColor(Qt::white));
        }
    }

    numberGen->display(0);
}

void Widget::setDarkColor()
{
    *(currentColor) = QColor(Qt::black);
}

void Widget::setLightColor()
{
    *(currentColor) = QColor(Qt::green);
}

void Widget::activateClassic()
{
    *(currentColor) = QColor(Qt::black);
    green->setEnabled(false);
    black->setEnabled(false);
}

void Widget::activateEarth()
{
    *(currentColor) = QColor(Qt::green);
    green->setEnabled(true);
    black->setEnabled(true);
    green->setChecked(true);
}

void Widget::startSlot()
{
    qDebug() << this->size();
    if(isStart == false)
    {
        isStart = true;
        reset->setEnabled(false);
        classic->setEnabled(false);
        earth->setEnabled(false);
        green->setEnabled(false);
        black->setEnabled(false);
        timer->start(dial->value());
        start->setText("Stop");

    }
    else
    {
        isStart = false;
        reset->setEnabled(true);
        classic->setEnabled(true);
        earth->setEnabled(true);
        if(earth->isChecked())
        {
            green->setEnabled(true);
            black->setEnabled(true);
        }
        timer->stop();
        start->setText("Start");
    }
}

void Widget::addValue()
{
    double value = numberGen->value() + 1;
    numberGen->display(value);
    timer->setInterval(dial->value());
}

void Widget::newGeneration()
{
    for( int i = 0; i < CellCount; i++ )
    {
        for( int j = 0; j < 2*CellCount; j++ )
        {
            tempColor[i][j] = QColor(Qt::white);
        }
    }

    for( int i = 0; i < CellCount; i++)
    {
        for( int j = 0; j < 2*CellCount; j++ )
        {
            int counter = countNeighbours(i,j);

            switch (counter) {
            case 2:

                tempColor[i][j] = table->item(i,j)->backgroundColor();
                break;

            case 3:

                tempColor[i][j] = QColor(Qt::black);
                break;

            case -2:

                tempColor[i][j] = table->item(i,j)->backgroundColor();
                break;

            case -3:

                tempColor[i][j] = QColor(Qt::green);
                break;

            default:

                tempColor[i][j] = QColor(Qt::white);
                break;
            }
        }
    }

    checkGenEqual();

    for(int row = 0; row < CellCount ; row++)
    {
        for(int col = 0; col < 2*CellCount; col++)
        {
            table->item(row,col)->setBackgroundColor(tempColor[row][col]);
        }
    }

    addValue();
}
