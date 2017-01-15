#ifndef WIDGET_H
#define WIDGET_H


#include <QtWidgets>

const int CellCount = 20;
const int CellSize = 20;
const QColor  lightColor = Qt::green;
const QColor  darkColor = Qt::black;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
private:
    int countNeighbours(int i, int j);
    void checkGenEqual();
private:
    bool isStart;

private:
    QPushButton* start;
    QPushButton* reset;
    QTableWidget* table;
    QDial* dial;
    QLCDNumber* numberDelay;
    QLCDNumber* numberGen;
    QRadioButton* classic;
    QRadioButton* earth;
    QRadioButton* black;
    QRadioButton* green;
    QTimer* timer;
    QColor** tempColor;
    QColor* currentColor;

public slots:
    void startSlot();
    void addValue();
    void newGeneration();
    void changeBackGround(int row, int column);
    void resetSlot();
    void setDarkColor();
    void setLightColor();
    void activateClassic();
    void activateEarth();
};

#endif // WIDGET_H
