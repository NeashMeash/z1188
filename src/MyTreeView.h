#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H


#include <QTreeView>

class MyTreeView : public QTreeView
{
public:
    explicit MyTreeView(QWidget *parent = 0);
    ~MyTreeView();
protected:
    void drawRow(QPainter *painter,
                         const QStyleOptionViewItem &options,
                         const QModelIndex &index) const;
};

#endif // MYTREEVIEW_H
