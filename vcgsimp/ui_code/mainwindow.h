#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QWidget>
#include <QObject>
#include <QScrollBar>
#include <iostream>
#include <string>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString fileInput;
    QString outputDir;
    QString outputFile_pre;
    QString outputFile_suf;
    QString outputFile;
    double multiple;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnfile_clicked();

    void on_btndir_clicked();

    void on_btnOK_clicked();

    void slotProcessCall(int exitCode);

    void slotProcessCall_1(int exitCode);
    void on_txteditLog_updateRequest(const QRect &rect, int dy);

private:
    Ui::MainWindow *ui;
    void slotProcessOutputRedirect();
    bool flag;

};
#endif // MAINWINDOW_H
