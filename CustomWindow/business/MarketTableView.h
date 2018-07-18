#ifndef MARKETTABLEVIEW_H
#define MARKETTABLEVIEW_H

#include <QtWidgets>
#include <vector>
#include "HJConstant.h"

using namespace std;


struct QUOTATION;
class QuoteStandardItemModel;
//typedef struct _TableRow
//{
//	QVariant    arrVar[32];
//	short     arrColor[32];
//    int       nDecimal;
//	_TableRow()
//	{
//		memset(arrVar, 0, sizeof(QVariant) * 32);
//		memset(arrColor, 0, sizeof(short) * 32);
//        nDecimal = 0;
//	}
//}TableRow;
//
//
//enum SORT_DIRECTOR
//{
//	SORT_UP = 0,
//	SORT_DOWN,
//	SOFT_NO
//};

class MarketTableView : public QTableView
{
    Q_OBJECT
public:

    typedef struct 
	{
        int index;
        Qt::SortOrder order;
    } LastSortInfo;
    struct PreCal
    {
        double dbLast; //最新价
        double dblHighest;//最高
        double dblLowest; //最低
        short  pre;
        PreCal()
        {
            memset(this, 0, sizeof(PreCal));
        }
    };

public:
    explicit MarketTableView(QSplitter *spliter,QAbstractItemModel* model = 0, QWidget* parent = 0);
	//MarketTableView(QSplitter *spliter);
    virtual ~MarketTableView();


private slots:
    void SelMenu(QAction* pAction);

    void RowClickSelected(const QModelIndex & index);
    void RowRightClicked(const QModelIndex & index);
   	//void rowDoubleClicked(const QModelIndex& index);
    void slotRowDoubleClicked(const QModelIndex& index);
	void LastSortIndex(int index, Qt::SortOrder order);
	void HeaderSectionMoved(int l, int o, int n);
    void HeaderActived(const QModelIndex & index);

    void OnMenu(QAction* pAction);
	void responseTimer();
	void ClickedInterval();
	void onColorChange();
	void showMarketChangeDetails(const QModelIndex &current, const QModelIndex &previous);
public:
    void SetTableColor();
    LastSortInfo m_LastSortIndex;
	void updateQuotation(QUOTATION *quote);
	void SortMarketData(std::vector<TableRow> &vecData, int nCol, SORT_DIRECTOR dir);
	void ClearData();
	void  InitColumn();
	void  InitRowData();
	void setMainWinId(QWidget* id);
	void    updateCell(int rowIndex, int columnIndex, QString& val);

	//void setColor(QuoteStandardItemModel *p, int  rowIndex,  int  colIndex, int val, int oldval);
private:
	void Connect();
	void init();
	void CloseTimer();
	void StartTimer();
    void verticalScrollbarValueChanged(int value) override;
    void horizontalScrollbarValueChanged(int value) override;
    void horizontalScrollbarAction(int action) Q_DECL_OVERRIDE;
	void SwapRow(std::vector<TableRow> &vecData, int nSrc, int nDest);
	//void TransData(std::vector<TSecurity*>* pVecSecurity, std::vector<TableRow> &vecTmp, int nStart, int nEnd, bool bSoft = false, bool bAll = false);
	void setModelColor(const QColor &up, const QColor &down, const QColor &none, const QColor &normal);
	void setColorFromTheme();
	void GetHeadNames();

	QString FormatPrice(unsigned int uiPrice);
	QString GetQuotationValue(const QString &lpszInsId, const QUOTATION &quotation, const int rowIndex, const int &iPos);


	//void  UpdateQuotationImp(QUOTATION * quote);
signals:
    void ContractSwitch(QString ConstractName);
    void ContractLocked(QString ConstractName);
    void StateChanged(bool bLast);
    void rightClicked(const QModelIndex& index);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
	QTimer *updateTimer;
	//std::vector<TSecurity*>* m_pVecSecurity;

	std::vector<TableRow> m_vecAll;
	SORT_DIRECTOR m_dir;

	bool	m_bResetData;
	int		m_nSrcollValue;
	int     m_nHSrcollValue;
	int		m_nSoftCol;
	bool	m_bResetSel;
	bool	m_bSoft;
	int     m_nResize;


	QPushButton*   Option;
	QStringList   m_NameList;
	int     m_ClickRow;
	QAbstractItemModel*   m_pmodel;
	QTimer*   m_interval;
	int       m_bDoubleClick;
	QModelIndex    m_CurIndex;
	QMenu*         m_pMenu;
	QString        m_Contract;
	QWidget*       m_mainWndid;
	QVector<int>   m_vecColumnIndexs;
};

#endif
