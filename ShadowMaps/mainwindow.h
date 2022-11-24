#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include "qpaintingwidget.h"
#include "shadowmapwidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QVBoxLayout *m_layout;
    QComboBox *m_combo_box;
    ShadowMapWidget *m_gl;
};
#endif // MAINWINDOW_H
