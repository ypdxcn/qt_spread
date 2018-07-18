#pragma once

#include <QDialog>
#include "ui_frmGetGoodsDlg.h"
#include "ArrayListMsg.h"
#include <vector>
#include <QStandardItemModel>
#include "HashtableMsg.h"
#include  "frmFramelessDialogBase.h"

// 仓库信息
struct StoreInfoEx
{
	string       stor_id; //仓库代码    
	string       stor_name; //仓库名称    
	string       addr; //仓库地址    
	string       tel; //联系电话    
	string       fax; //联系传真    
	string       link_man; //联系人    
	string       zip_code; //邮编    
	string       use_variety_type; //支持的提货品种    每个品种类型之间使用”,”分隔
	string       is_take; //是否允许提货    yes_no
	string       city_code; //所属城市    多个城市代码之间使用”,”分隔
};


// 城市信息
struct CityInfo
{
	string       city_code; //城市代码    
	string       city_name; //城市名称    
	string       all_stor_id; //仓库代码    2*,3*
	string       stor_id_default; //默认提货仓库    2*,3*
};


class frmGetGoodsDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmGetGoodsDlg(QWidget *parent = Q_NULLPTR);
	~frmGetGoodsDlg();


	bool IsShowTakeSheetData();



	void IniVarietyComboBox();

	void OnCbnSelchangeComboPerson();

	ArrayListMsg* GetTakeShhetDate();
	void GetTakeSheetDateFromServer();
	void LoadTakeSheetDateToComBox();
	bool loadStore();
	void IniStoreInfo(HashtableMsg &htm);
	bool loadCityFromFile();
	void IniCityInfo(HashtableMsg &htm);
	void IniCityComboBox();

	bool CheckInput();

	string GetStoreId(const string &sName);

	bool IsWeightRight();
	bool CheckPassword();
	bool CheckIDCard();
	bool CheckIDCard15(QString Id);
	bool CheckIDCard18(QString Id);
	bool IsRightDate(QString datestr);
	string GetStoreName(const string &sID);
public slots:
	void OnApplyGoods();
	void OnSearch();
	void OnPrint();
	void OnCancel();
	void OnChangeCity(QString city);
	void OnChangeType(QString city);
private:
	Ui::frmGetGoodsDlg ui;

	bool m_bShowTakeSheetDate;
	int m_nTakeDayDis;

	ArrayListMsg m_alm_take_sheet_date; // 最近提货日
	vector<CityInfo> m_vCity;  //城市


	QStandardItemModel*   tableModel;

	vector<StoreInfoEx> m_vStore;//仓库

};
