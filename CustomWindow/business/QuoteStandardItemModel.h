#ifndef LXSTANDARDITEMMODEL_H
#define LXSTANDARDITEMMODEL_H

#include <QMap>
#include <QVariant>
#include <QModelIndex>
#include <QStandardItemModel>

//#include "../Table/MarketTableView.h"
#define RESERVE_ROW_COUNT 25

//
typedef struct _tagTableRow
{
	QVariant arrVar[32];
	short arrColor[32];
	int nDecimal;
	_tagTableRow()
	{
		memset(arrVar, 0, sizeof(QVariant) * 32);
		memset(arrColor, 0, sizeof(short) * 32);
		nDecimal = 0;
	}
}GcnTableRow;

class QuoteStandardItemModel : public QStandardItemModel
{
	Q_OBJECT
public:

    explicit QuoteStandardItemModel(QObject *parent = NULL);
public:
    QVariant data(const QModelIndex & index,  int role=Qt::DisplayRole) const;
    QVariant headerData(int section,
		                Qt::Orientation orientation,
                        int role=Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	bool setItem2(const int &row, const int  &column, const QVariant& value);//自定义数据

	void ClearAllData();
	void setVerticalHeaderLabels(const QStringList &labels);

	void SetMarketData(std::vector<GcnTableRow> vecData, int nStartSel);
    //void SetMarketDataTextColor(std::vector<RowColor> vecColor);
    void setUpDownColor(const QColor &up,const QColor &down,const QColor &none,const QColor &normal)
    {
        m_upColor = up;
        m_downColor = down;
        m_noneColor = none;
        m_normalColor = normal;
    }

private:
	//自己存储数据
	std::vector<GcnTableRow> m_vecData;
	int    m_nStartSel;
    const char *m_cDecimal;
    std::string m_strDecimal;

    QColor m_upColor;//上涨颜色
    QColor m_downColor;//下跌颜色
    QColor m_noneColor;//不涨不跌的颜色

    QColor m_normalColor;//普通字段颜色

private:
    void InitData();
	
};

#endif // LXSTANDARDITEMMODEL_H
