

#ifndef WINDOWDRAGGER_H
#define WINDOWDRAGGER_H

#include <QWidget>
#include <QMouseEvent>
//用作dialog的可拖拉标题
class WindowDragger : public QWidget
{
  Q_OBJECT

public:
  explicit WindowDragger(QWidget *parent = 0);

signals:
  void doubleClicked();

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);

protected:
  bool   mousePressed;
  QPoint mousePos;
  QPoint wndPos;
};

#endif // WINDOWDRAGGER_H
