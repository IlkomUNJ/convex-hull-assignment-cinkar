#include "convexhullwidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QDebug>

ConvexHullWidget::ConvexHullWidget(QWidget *parent)
    : QWidget{parent}
{
    // Mengaktifkan Mouse Tracking agar mousePressEvent bisa menangkap klik
    setMouseTracking(true);
}

void ConvexHullWidget::mousePressEvent(QMouseEvent *event)
{
    // Hanya menerima input titik jika Convex Hull belum dihitung
    if (!m_hullCalculated && event->button() == Qt::LeftButton) {
        m_points.append(event->pos());
        // Memaksa widget untuk menggambar ulang (memanggil paintEvent)
        update();
    }
}

void ConvexHullWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. Menggambar semua Titik yang dimasukkan
    painter.setBrush(Qt::SolidPattern);
    painter.setPen(QPen(Qt::black, 5));
    for (const QPoint &point : m_points) {
        painter.drawPoint(point);
    }

    // 2. Menghubungkan titik-titik dengan garis biasa (sesuai permintaan)
    if (m_points.size() > 1 && !m_hullCalculated) {
        painter.setPen(QPen(Qt::gray, 1, Qt::DotLine));
        for (int i = 0; i < m_points.size() - 1; ++i) {
            painter.drawLine(m_points[i], m_points[i+1]);
        }
    }

    // 3. Menggambar Convex Hull jika sudah dihitung
    if (m_hullCalculated && m_convexHull.size() > 1) {
        painter.setPen(QPen(Qt::red, 3, Qt::SolidLine));
        painter.setBrush(Qt::NoBrush);

        // Menggambar garis yang menghubungkan titik-titik Convex Hull
        QPolygon hullPolygon;
        for (const QPoint& p : m_convexHull) {
            hullPolygon << p;
        }
        painter.drawPolygon(hullPolygon);

        // Menggambar ulang titik-titik Convex Hull dengan warna berbeda (opsional)
        painter.setPen(QPen(Qt::red, 7));
        for (const QPoint &point : m_convexHull) {
            painter.drawPoint(point);
        }
    }
}

// Fungsi pembantu untuk menentukan orientasi 3 titik (collinear, clockwise, counterclockwise)
// < 0: Counter-clockwise, = 0: Collinear, > 0: Clockwise
static int orientation(QPoint p, QPoint q, QPoint r)
{
    // (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y)
    qreal val = (q.y() - p.y()) * (r.x() - q.x()) -
                (q.x() - p.x()) * (r.y() - q.y());

    if (qFuzzyIsNull(val)) return 0;  // Collinear
    return (val > 0) ? 1 : 2; // Clockwise atau Counter-clockwise
}

// Implementasi Algoritma Convex Hull (Jarvis March / Gift Wrapping)
QVector<QPoint> ConvexHullWidget::jarvisMarch(QVector<QPoint> points)
{
    int n = points.size();
    if (n < 3) return points; // Tidak bisa membentuk hull jika < 3 titik

    // Cari titik paling kiri (pasti termasuk dalam Convex Hull)
    int l = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x() < points[l].x()) {
            l = i;
        }
    }

    QVector<QPoint> hull;
    int p = l, q;

    do {
        hull.append(points[p]);

        // Cari titik 'q' sedemikian rupa sehingga orientasi(p, q, r)
        // selalu berlawanan arah jarum jam (counter-clockwise) untuk setiap titik 'r'.
        q = (p + 1) % n;
        for (int i = 0; i < n; i++) {
            // Jika i lebih counter-clockwise dari q saat ini, jadikan i sebagai q baru
            if (orientation(points[p], points[i], points[q]) == 2) {
                q = i;
            }
        }

        p = q; // Pindah ke titik hull berikutnya
    } while (p != l); // Berhenti ketika kembali ke titik awal

    return hull;
}

void ConvexHullWidget::calculateAndDrawHull()
{
    // Pastikan minimal ada 3 titik
    if (m_points.size() < 3) {
        m_convexHull.clear();
        m_hullCalculated = false;
        update();
        return;
    }

    // Hanya hitung jika belum dihitung dan ada titik
    if (!m_hullCalculated) {
        m_convexHull = jarvisMarch(m_points);
        m_hullCalculated = true;
        update(); // Gambar ulang dengan Convex Hull
    }
}

void ConvexHullWidget::clearPoints()
{
    m_points.clear();
    m_convexHull.clear();
    m_hullCalculated = false;
    update(); // Gambar ulang (kosong)
}
