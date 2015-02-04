#include "MyHeaderView.h"
#include <QGraphicsItem>
#include <private/qheaderview_p.h>

MyHeaderView::MyHeaderView(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView( orientation,parent)
{

}

MyHeaderView::~MyHeaderView()
{

}

void MyHeaderView::mouseMoveEvent(QMouseEvent *e)
{
    /*Q_D(QHeaderView);*/
    QHeaderViewPrivate * d =  reinterpret_cast<QHeaderViewPrivate *>(qGetPtrHelper(d_ptr));

       int pos = d->orientation == Qt::Horizontal ? e->x() : e->y();
       if (pos < 0)
           return;
       if ( d->state ==  QHeaderViewPrivate::NoState ) {
                    //section
                   int handle = sectionHandleAt(pos);
                   bool hasCursor = testAttribute(Qt::WA_SetCursor);
                   if (handle != -1 && (sectionResizeMode(handle) == Interactive)) {
                       if (!hasCursor)
                           setCursor(d->orientation == Qt::Horizontal ? Qt::SizeHorCursor : Qt::SizeVerCursor);
                   } else if (hasCursor) {
                       unsetCursor();
                   }

                   return;
       }
       return QHeaderView::mouseMoveEvent(e);
}

int MyHeaderView::sectionHandleAt(int position)
{
    QHeaderViewPrivate * d =  reinterpret_cast<QHeaderViewPrivate *>(qGetPtrHelper(d_ptr));

    QHeaderView *q = this;
    int visual = q->visualIndexAt(position);
       if (visual == -1)
           return -1;
       int log = logicalIndex(visual);
       int pos = q->sectionViewportPosition(log);
       int grip = q->style()->pixelMetric(QStyle::PM_HeaderGripMargin, 0, q);

       bool atLeft = position < pos + grip;
       bool atRight = (position > pos + q->sectionSize(log) - grip);
       if (d->reverse())
           qSwap(atLeft, atRight);

       if (atLeft) {
           //grip at the beginning of the section
           while(visual > -1) {
               int logical = q->logicalIndex(--visual);
               if (!q->isSectionHidden(logical))
                   return logical;
           }
       } else if (atRight) {
           //grip at the end of the section
           return log;
       }
       return -1;

}

bool MyHeaderView::reverse()
{
    QHeaderViewPrivate * d =  reinterpret_cast<QHeaderViewPrivate *>(qGetPtrHelper(d_ptr));
    return d->orientation == Qt::Horizontal && isRightToLeft();
}

