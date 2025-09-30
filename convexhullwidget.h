#ifndef CONVEXHULLWIDGET_H
#define CONVEXHULLWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QVector>

class ConvexHullWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConvexHullWidget(QWidget *parent = nullptr);
    void clearPoints();
    void calculateAndDrawHull();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPoint> m_points;         // Titik-titik yang dimasukkan pengguna
    QVector<QPoint> m_convexHull;     // Titik-titik pembentuk Convex Hull
    bool m_hullCalculated = false;    // Status apakah Convex Hull sudah dihitung

    // Fungsi algoritma untuk Convex Hull (contoh menggunakan Jarvis March/Gift Wrapping)
    QVector<QPoint> jarvisMarch(QVector<QPoint> points);
};

#endif // CONVEXHULLWIDGET_H
