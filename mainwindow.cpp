#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "convexhullwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Convex Hull Finder Qt");

    // Membuat Custom Widget
    m_hullWidget = new ConvexHullWidget(this);

    // Membuat Tombol
    QPushButton *startButton = new QPushButton("Start Convex Hull");
    QPushButton *clearButton = new QPushButton("Clear");

    // Menghubungkan sinyal tombol ke slot
    connect(startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);

    // Tata Letak
    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(clearButton);
    controlLayout->addStretch(); // Memastikan tombol tetap di atas

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_hullWidget, 4); // Memberi ruang lebih besar untuk widget gambar
    mainLayout->addLayout(controlLayout, 1);

    // Membuat widget sentral baru untuk menampung mainLayout
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Atur ukuran jendela awal
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    // Memanggil fungsi untuk menghitung dan menggambar Convex Hull
    m_hullWidget->calculateAndDrawHull();
}

void MainWindow::on_clearButton_clicked()
{
    // Memanggil fungsi untuk menghapus semua titik
    m_hullWidget->clearPoints();
}
