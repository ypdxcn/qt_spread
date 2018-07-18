#include "QuoteStandardItemModel.h"

#include "Macrodefs.h"

QuoteStandardItemModel::QuoteStandardItemModel(QObject *parent)    : QStandardItemModel(parent)
{
	m_nStartSel   = 0;
    m_cDecimal    = NULL;

	//只是初始化，有函数进行设置
    m_upColor     = QColor("#ff786e");
    m_downColor   = QColor("#6cad3c");
    m_noneColor   = QColor("#e8e8e8");
    m_normalColor = QColor("#e8e8e8");

	//InitData();
}

QVariant QuoteStandardItemModel::data(const QModelIndex & index, int role) const
{
	QString strTmp;
	int nSel = 0;
	nSel = m_nStartSel - RESERVE_ROW_COUNT;
	if (nSel < 0)
		nSel = 0;
	int nStart;
    int column = index.column();
    int row = index.row();

	if (nSel > 0)
		nStart = row - m_nStartSel + RESERVE_ROW_COUNT;
	else
		nStart = row;

	
    if (role == Qt::DisplayRole)
    {
        //20180305  屏蔽一个BUG
        if (  m_vecData.size()  <= 0)
              return  " ";
        if(nStart >= (m_vecData.size()-1 ))
            nStart =m_vecData.size()-1 ;
        return  m_vecData[nStart].arrVar[column];

		//if (column == 3)
		//{

		//	double val = m_vecData[nStart].arrVar[column].toDouble();
		//	GcnTableRow table = m_vecData[nStart];
		//	if (val >= 0)
		//		table.arrColor[column] = 1;
		//	else
		//		table.arrColor[column] = -1;

		//	strTmp.sprintf("%0.2f"/*strDecimal.toStdString().c_str()*/, m_vecData[nStart].arrVar[column].toDouble());

		//	return  strTmp;
		//}

        // 涨幅和涨跌幅，需要转化为百分比
		if (column == 3 || column == 4 || column ==5)
		{
			if (column == 4)
			{
				int test = 0;
			}

			// 百分比 0.5是四舍五入
			if (nStart < m_vecData.size())
			{
				//int nDecimal = m_vecData[nStart].nDecimal;
				//double dDec = 0;
				//if (nDecimal >= 1)
				//	dDec = 5 / qPow(10, nDecimal + 1);
				if (m_vecData[nStart].arrVar[column].toString() == "--")
					return "--";
				else
				{
					int nDecimal = 2;//m_vecData[nStart].nDecimal;
                    //QString strDecimal;
                    //strDecimal.sprintf("%%0.%df%", nDecimal);
                    strTmp.sprintf("%0.2f%"/*strDecimal.toStdString().c_str()*/, m_vecData[nStart].arrVar[column].toDouble()/* + dDec*/);
                    //if (strTmp == "nan%")
                    //	strTmp = m_vecData[nStart].arrVar[column].toString();
                    if(strTmp == "0.00%" || strTmp == "-0.00%")
                        strTmp = "0.00%";
				
                
					double val = m_vecData[nStart].arrVar[column].toDouble();
					GcnTableRow table = m_vecData[nStart];

					if (strTmp.left(1) == "0")
					{
						QString strCom = "0";
						QString strCom2 = "-0";
						switch (nDecimal) {
						case 1:
							strCom = "0.0";
							strCom2 = "-0.0";
							break;
						case 2:
							strCom = "0.00";
							strCom2 = "-0.00";
							break;
						case 3:
							strCom = "0.000";
							strCom2 = "-0.00";
							break;
						default:
							strCom = "0";
							strCom2 = "0";
							break;
						}
					
			

                        if(strTmp == strCom || strTmp == strCom2)
                        {
                           strTmp = strCom;
						   table.arrColor[column] = 1;
						   return strTmp;
                        }
			            
					
					
					}

				   if (val >= 0)
						table.arrColor[column] = 1;
					else
						table.arrColor[column] = -1;

				 return strTmp;
				}
				
			}
		}
        else if (column != 32)
		{ 
            // 需要格式化的
            if ( column == 26 /*|| column == 8 */|| column == 18 || column == 19 || column == 20
                || column == 21 || column == 25 ||column == 26 || column == 27 || column == 28 || column == 29 || column == 30)
            {
                if (nStart < m_vecData.size())
				{
                    if(m_vecData[nStart].arrVar[column].toString() == "--")
                        return "--";
					int nDecimal = m_vecData[nStart].nDecimal;
					QString strDecimal;
					strDecimal.sprintf("%%0.%df", nDecimal);
					strTmp.sprintf(strDecimal.toStdString().c_str(), m_vecData[nStart].arrVar[column].toDouble()/* + dDec*/);
                    if(strTmp.left(1) == "0" || strTmp.left(2) == "-0")
                    {
                        QString strCom = "0";
                        QString strCom2 = "-0";
                        switch (nDecimal) {
                        case 1:
                            strCom = "0.0";
                            strCom2 = "-0.0";
                            break;
                        case 2:
                            strCom = "0.00";
                            strCom2 = "-0.00";
                            break;
                        case 3:
                            strCom = "0.000";
                            strCom2 = "-0.00";
                            break;
                        default:
                            strCom = "0";
                            strCom2 = "-0";
                            break;
                        }
                        if(strTmp == strCom || strTmp == strCom2)
                        {
                           strTmp = strCom;
                           //m_vecData[nStart].arrColor[column] = 0;
                        }
                    }
                    return strTmp;
				}
			}
            else if(column == 18)        // 成交额这列单独处理
            {
                if (nStart < m_vecData.size())
                {
                    if(m_vecData[nStart].arrVar[column] > 100000000)    // 超过一亿,两位小数点
                    {
                        strTmp.sprintf("%0.2f", m_vecData[nStart].arrVar[column].toDouble() / 100000000);
                        strTmp += _MY_TEXT("亿");
                        return strTmp;
                    }
                    else
                    {
                        double dNum = m_vecData[nStart].arrVar[column].toDouble();
                        if(dNum > 10000)        // 大于10000
                        {
                            strTmp.sprintf("%0.0f", dNum / 10000);
                            strTmp += _MY_TEXT("万");
                            return strTmp;
                        }
                        else                // 小于10000
                        {
                            strTmp.sprintf("%0.0f", dNum);
                            return strTmp;
                        }
                    }
                }
            }
			else
			{
				if (nStart < m_vecData.size())
					return m_vecData[nStart].arrVar[column];
			}
		}
        else
            return "";
    }
	if (Qt::TextAlignmentRole == role)
	{
//		switch (column)
//		{
//		case 0:
//			return Qt::AlignCenter;
//			break;
//		case 1:
//		case 2:
//		case 3:
//		case 4:
//            return Qt::AlignLeft | Qt::AlignVCenter;
//            break;
//		case 15:
//		case 16:
//		case 17:
//            return Qt::AlignCenter;
//			break;
//		default:
//			return Qt::AlignRight | Qt::AlignVCenter;
//			break;
//		}

       // return (int)(Qt::AlignRight | Qt::AlignVCenter);
		return (int)(Qt::AlignCenter);
	}

	//设置显示的颜色
    if (Qt::TextColorRole == role)
    {
        switch (column) {
		case 0:
			return  QColor("#dab96b");
		//case 1:
		//case 2:
		//	return m_normalColor;
		case 3:
		case 4:
		case 5:        

			if (nStart > m_vecData.size() - 1 || m_vecData.size() < 1)
                return m_noneColor;

			if (m_vecData[nStart ].arrColor[column] == 1)
            {
                return m_upColor;
            }
			else if (m_vecData[nStart ].arrColor[3] == -1)
            {
                return m_downColor;
            }
            else
                return m_noneColor;
            break;


		//case 6:
		//case 8:
		//case 12:
		//case 13:
		//case 14:
		//case 15:

		//	if (nStart > m_vecData.size() - 1 || m_vecData.size() < 1)
		//		return m_noneColor;

		//	if (m_vecData[nStart].arrColor[column] > m_vecData[nStart].arrColor[10])
		//	{
		//		return m_upColor;
		//	}
		//	else if (m_vecData[nStart].arrColor[column] < m_vecData[nStart].arrColor[10])
		//	{
		//		return m_downColor;
		//	}
		//	else
		//		return m_noneColor;
		//	break;
        default:
            return m_normalColor;
            break;
        }
    }
    return QStandardItemModel::data(index, role);
}


//表头单元格，文字靠右对齐
QVariant QuoteStandardItemModel::headerData(int section,
        Qt::Orientation orientation,
        int role) const
{
// 	if (section == 0 && orientation == Qt::Vertical)
//    {
//        if(role == Qt::DisplayRole)
//            return section;
//    }

	
    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal)
      {
          //return int(Qt::AlignLeft | Qt::AlignVCenter);
		   return int(Qt::AlignCenter);
      }
    return QStandardItemModel::headerData(section,orientation,role);
}

bool QuoteStandardItemModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
    if(index.row() < 0 || index.column() < 0)
        return false;

    m_vecData[index.row()].arrVar[index.column()] = value;
    return true;
}

bool QuoteStandardItemModel::setItem2(const int &row, const int  &column,const QVariant& value)
{
	if (row < 0 || column < 0)
		return false;

	//quotation.ini固定位置的定义
	if (column == 3 || column == 4 || column == 5)
	{
		if(value.toFloat() >= 0)
			m_vecData[row].arrColor[column] = 1;
		else
			m_vecData[row].arrColor[column] = -1;

	}

	
	if (m_vecData.size() <= row)
	{
		GcnTableRow tablrow;	
		tablrow.arrVar[column] = value;
		m_vecData.push_back(tablrow);
	}
	else
	{
		m_vecData[row].arrVar[column] = value;
	}


	return true;
}

// 初始化值
void QuoteStandardItemModel::InitData()
{
    QFile file(_RES_DATADB_PATH + _MY_TEXT("grades.txt"));

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);
        QString line = stream.readLine();
        QStringList list = line.split(",", QString::SkipEmptyParts);

        int row = 0;
        while (!stream.atEnd())
        {
            line = stream.readLine();
            if (!line.startsWith("#") && line.contains(","))
            {
                list= line.split(",");
				
				GcnTableRow tablrow;
                for (int col = 0; col < list.length(); ++col)
                {
					tablrow.arrVar[col] = list.at(col);
					//if (col == 4 || col == 5)
					if (col >= 3 && col <= 15)
					{
						tablrow.arrVar[col] = "--";
						if (tablrow.arrVar[col] >= 0)
						{
							tablrow.arrColor[col] = 1;//值大于0，显示红色
						}
						else
						{
							tablrow.arrColor[col] = -1;
						}
					}
                }
                
				m_vecData.push_back(tablrow);

            }
        }
    }
    file.close();
}

void QuoteStandardItemModel::ClearAllData()
{
	m_vecData.clear();
}

void QuoteStandardItemModel::setVerticalHeaderLabels(const QStringList &labels)
{
	return QStandardItemModel::setVerticalHeaderLabels(labels);
}

void QuoteStandardItemModel::SetMarketData(std::vector<GcnTableRow> vecData, int nStartSel)
{
	m_vecData.clear();
    m_vecData.swap(vecData);
	m_nStartSel = nStartSel;

}
