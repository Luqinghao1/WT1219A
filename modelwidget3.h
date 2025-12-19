#ifndef MODELWIDGET3_H
#define MODELWIDGET3_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <functional>
#include <tuple>
#include <QLineEdit>

#include "mousezoom.h"
#include "chartsetting1.h"

// 定义数据类型: <时间t, 压力p, 导数dp>
typedef std::tuple<QVector<double>, QVector<double>, QVector<double>> ModelCurveData;

namespace Ui {
class ModelWidget3;
}

class ModelWidget3 : public QWidget
{
    Q_OBJECT

public:
    explicit ModelWidget3(QWidget *parent = nullptr);
    ~ModelWidget3();

    ModelCurveData calculateTheoreticalCurve(const QMap<QString, double>& params,
                                             const QVector<double>& providedTime = QVector<double>());

    void setHighPrecision(bool high);

private slots:
    void onCalculateClicked();
    void onResetParameters();
    void onExportData();
    void onExportImage();
    void onResetView();
    void onFitToData();
    void onChartSettings();
    void onDependentParamsChanged();
    void onShowPointsToggled(bool checked);

signals:
    void calculationCompleted(const QString &analysisType, const QMap<QString, double> &results);

private:
    void initChart();
    void setupConnections();

    QVector<double> parseInput(const QString& text);
    void setInputText(QLineEdit* edit, double value);
    void runCalculation();
    void plotCurve(const ModelCurveData& data, const QString& name, QColor color, bool isSensitivity);

    // --- 数学核心 ---
    void calculatePDandDeriv(const QVector<double>& tD, const QMap<QString, double>& params,
                             std::function<double(double, const QMap<QString, double>&)> laplaceFunc,
                             QVector<double>& outPD, QVector<double>& outDeriv);

    double flaplace_composite(double z, const QMap<QString, double>& p);

    // [修改] 增加 reD 参数用于封闭边界计算
    double PWD_inf(double z, double fs1, double fs2, double M12, double LfD, double rmD, double reD, int nf, const QVector<double>& xwD);

    double scaled_besseli(int v, double x);
    double adaptiveGauss(std::function<double(double)> f, double a, double b, double eps, int depth, int maxDepth);
    double gauss15(std::function<double(double)> f, double a, double b);
    double stefestCoefficient(int i, int N);
    double factorial(int n);

private:
    Ui::ModelWidget3 *ui;
    MouseZoom *m_plot;
    QCPTextElement *m_plotTitle;
    bool m_highPrecision;

    QVector<double> res_tD;
    QVector<double> res_pD;
    QVector<double> res_dpD;

    QList<QColor> m_colorList;
};

#endif // MODELWIDGET3_H
