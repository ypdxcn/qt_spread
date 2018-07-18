#pragma once

#include <QStandardItemModel>
#include  <QString>
#include  <vector>
#include  "app.h"

using   namespace  std;


class CommonStandardItemModel : public QStandardItemModel
{
	Q_OBJECT
public:

    explicit CommonStandardItemModel(QObject *parent = NULL);
	virtual ~CommonStandardItemModel();

	QVariant data(const QModelIndex & index, int role) const;
	void LoadList(const QString &csPath);
	//void SplitStrToVector(QString Str, QString SplitWord, vector<int> &Vec, bool bAddSplit = true, bool bRemoveBlank = true);
	//void SplitStrToVector(QString Str, QString SplitWord, vector<QString> &Vec, bool bAddSplit = true, bool bRemoveBlank = true);

};
