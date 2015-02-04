#ifndef MYHEADERVIEW_H
#define MYHEADERVIEW_H

#include <QObject>
#include <QHeaderView>

class MyHeaderView : public QHeaderView
{
    Q_OBJECT
public:
     explicit MyHeaderView(Qt::Orientation orientation, QWidget *parent = 0);
    ~MyHeaderView();
    void mouseMoveEvent(QMouseEvent *e);
    int sectionHandleAt(int position);
    bool reverse();

signals:

public slots:
private:
   //Q_DECLARE_PRIVATE( MyHeaderView );
};

#endif // MYHEADERVIEW_H
