#include <QPainter>
#include "TableItemDelegate.h"

TableItemDelegate::TableItemDelegate(QTableView *tableView)
{
	//kenny  绘制隔线
	/*int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
	QColor gridColor = static_cast<QRgb>(gridHint);
	mPen = QPen(gridColor, 0, tableView->gridStyle());*/
}

void TableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem  viewOption(option);

    //高亮显示与普通显示时的前景色一致（即选中行和为选中时候的文字颜色一样）
    viewOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::TextColorRole).value<QColor>());//HighlightedText
   

	//	取消选中时，焦点的单元格虚线
	if (viewOption.state & QStyle::State_HasFocus)
		viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

	//kenny  绘制隔线
	//QPen oldPen = painter->pen();
	//painter->setPen(mPen);
	//painter->drawLine(viewOption.rect.bottomLeft(), viewOption.rect.bottomRight());
	//painter->setPen(oldPen);


	QItemDelegate::paint(painter, viewOption, index);
}
