#include "MyTreeView.h"
#include "HomeUsersModel.h"
#include <QStyleOptionViewItem>
#include <QDebug>

MyTreeView::MyTreeView(QWidget *parent):QTreeView(parent)
{

}

MyTreeView::~MyTreeView()
{

}

void MyTreeView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = options;
    opt.decorationAlignment = Qt::AlignCenter;
    opt.decorationPosition = QStyleOptionViewItem::Top;
    return QTreeView::drawRow(painter,opt,index);
}

