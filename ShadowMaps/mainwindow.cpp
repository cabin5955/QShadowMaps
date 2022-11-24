#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    setLayout(m_layout);
    m_combo_box = new QComboBox(this);
    m_gl = new ShadowMapWidget(this);

    m_layout->addWidget(m_gl);
    m_layout->addWidget(m_combo_box);

    m_combo_box->addItem("Red");
    m_combo_box->addItem("Blue");
    m_combo_box->addItem("Yellow");
    m_combo_box->addItem("Green");
}

MainWindow::~MainWindow()
{
}

