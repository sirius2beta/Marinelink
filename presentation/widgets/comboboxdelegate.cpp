#include "comboboxdelegate.h"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>
#include <QDebug>

#include <iostream>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
:QItemDelegate(parent)
{
    Items.push_back("");
    Items.push_back("");
    Items.push_back("");
    Items.push_back("");
    Items.push_back("");
    Items.push_back("");
    Items.push_back("");
    Items.push_back("");

}


QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index ) const
{

  QComboBox* editor = new QComboBox(parent);
  if(SelectionType == 0){
      for(int i = 0; i<selectionMapping.size(); i++){
          if(mapping[index.row()].contains(QString::number(i))){
              editor->addItem(selectionMapping[i]);
          }
      }
  }else if(SelectionType == 1){
      for(int i = 0; i<selectionMapping.size(); i++){
          if(index.model()->index(index.row(),3).data(Qt::DisplayRole) == ""){

          }else{
              editor->addItem(selectionMapping[i]);
          }
      }
  }


  return editor;
}

void ComboBoxDelegate::init(int selectionType, QList<QString> mapping, QList<QString> selectionMapping)
{
    this->SelectionType = selectionType;
    this->selectionMapping = selectionMapping;
    this->mapping = mapping;

}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);

  int value = index.model()->data(index, Qt::EditRole).toUInt();
  comboBox->setCurrentIndex(value);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  //QStyleOptionViewItemV4 myOption = option;
  QString text = Items[index.row()].c_str();

  //myOption.text = text;

  //QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
