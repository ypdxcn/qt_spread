#ifndef LXTABLEITEMDELEGATE_H
#define LXTABLEITEMDELEGATE_H

#include <QItemDelegate>
#include <QPen>
#include <QTableView>


class TableItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
	TableItemDelegate(QTableView *tableView);
protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;


private:
		QPen         mPen;
		QTableView*  mView;
};

#endif // LXTABLEITEMDELEGATE_H
