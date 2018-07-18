#include "CommonStandardItemModel.h"
#include "HJGlobalFun.h"

#pragma execution_character_set("utf-8")



CommonStandardItemModel::CommonStandardItemModel(QObject *parent)
	: QStandardItemModel(parent)
{

}

CommonStandardItemModel::~CommonStandardItemModel()
{
	//
}




// 加载list
void CommonStandardItemModel::LoadList(const QString &csPath)
{
	QVector<int> m_vecColumnIndexs;
	QVector<QString> vecAllNames;
	QVector<int>     vecAllWidths;
	QVector<int>     vecAllAligns;

	QString buf;

	// 读取用户选择列的ID，并且分割到vector
	App::GetPriProfileString(csPath, "Info", "UserColumnIndexs",buf);	
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);


	// 读取全部字段名称，并分割到vector
	App::GetPriProfileString(csPath, "Info", "AllColumnNames", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllNames);

	// 读取全部字段宽度，并分割到vector
	App::GetPriProfileString(csPath, "Info", "AllColumnWidths", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllWidths);
	// 读取全部字段对其方式，并分割到vector
	App::GetPriProfileString(csPath, "Info", "AllColumnAligns", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllAligns);


	QVector<QString> vecAllColumnIDs;
	QVector<QString> vecUserColumnIDs;

	App::GetPriProfileString(csPath, "Info", "AllColumnIDs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllColumnIDs);

	// 加载标题信息到list
	setColumnCount(m_vecColumnIndexs.size());
	for (int i = 0; i < m_vecColumnIndexs.size(); i++)
	{
		int iPos = m_vecColumnIndexs.at(i);

		//if (i == 0) // 将第一列的宽度加上一定量，解决第一列宽度比设置窄的问题
			//InsertColumn(i, vecAllNames.at(iPos), vecAllAligns.at(iPos), vecAllWidths.at(iPos) + 3);
	         //setHeaderData(i, Qt::Horizontal, vecAllNames.at(iPos));
		//else
			//InsertColumn(i, vecAllNames.at(iPos), vecAllAligns.at(iPos), vecAllWidths.at(iPos));
		   
		setHeaderData(i, Qt::Horizontal, vecAllNames.at(iPos));
		vecUserColumnIDs.push_back(vecAllColumnIDs.at(iPos));
	}

	// 初始化map
	//IniMap(vecUserColumnIDs);
}







QVariant CommonStandardItemModel::data(const QModelIndex & index, int role) const
{
	if (Qt::TextAlignmentRole == role)
	{
		return (int)(Qt::AlignCenter);
	}

	//设置显示的颜色
	if (Qt::TextColorRole == role)
	{

		return QColor("#b4b4b4");

	}
	return QStandardItemModel::data(index, role);
}
