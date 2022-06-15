#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->outputFile->setText("test_out");
    ui->multiplyinput->setText("2.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnfile_clicked()
{
    fileInput = QFileDialog::getOpenFileName(NULL,"查找文件",".","OBJ Files(*.obj);;PLY Files(*.ply);;FBX Files(*.fbx)");
    ui->fileSysteminput->setText(fileInput);
    //qDebug()<<fileInput;
}

void MainWindow::on_btndir_clicked()
{
    outputDir = QFileDialog::getExistingDirectory();
    ui->outputDir->setText(outputDir);
    //qDebug()<<outputDir;
}

void MainWindow::slotProcessOutputRedirect(){
    QProcess *pProcess = (QProcess*) sender();
    QString outputStdOut = QString::fromLocal8Bit(pProcess->readAllStandardOutput());
    QString outputStdErr = QString::fromLocal8Bit(pProcess->readAllStandardError());
    QPlainTextEdit *textEditLog = ui->txteditLog;
    textEditLog->appendPlainText(outputStdOut);
    textEditLog->appendPlainText(outputStdErr);
    QScrollBar *scrollbar = textEditLog->verticalScrollBar();
    if(scrollbar)
        scrollbar->setSliderPosition(scrollbar->maximum());
}

void MainWindow::slotProcessCall(int exitCode){
    qDebug() << "weeee";
    if(exitCode!=0){
        QPlainTextEdit *textEditLog = ui->txteditLog;
        textEditLog->appendPlainText("Other Process Wrong");
        return;
    }
    QProcess *pProcess = (QProcess*) sender();
    QString outputStdOut = QString::fromLocal8Bit(pProcess->readAllStandardOutput());
    QString outputStdErr = QString::fromLocal8Bit(pProcess->readAllStandardError());
    QPlainTextEdit *textEditLog = ui->txteditLog;
    textEditLog->appendPlainText(outputStdOut);
    textEditLog->appendPlainText(outputStdErr);
    QScrollBar *scrollbar = textEditLog->verticalScrollBar();
    if(scrollbar)
        scrollbar->setSliderPosition(scrollbar->maximum());
    QFile tempfile(this->outputDir+"\\"+this->outputFile_pre+".obj");
    if(tempfile.exists()){
        tempfile.remove();
    }else
    {
        qDebug() << "filepath error";
    }

    QFile tempfile1(this->outputDir+"\\"+this->outputFile_pre+".obj"+".mtl");
    if(tempfile1.exists()){
        tempfile1.remove();
    }else{
        qDebug() << "filepath error";
    }
}

void MainWindow::on_btnOK_clicked()
{
    this->fileInput = ui->fileSysteminput->text();
    this->multiple = ui->multiplyinput->text().toDouble();
    this->outputDir = ui->outputDir->text();
    if(this->outputDir.isEmpty()){
        std::string input = fileInput.toStdString();
        std::string oPath = input.substr(0,input.find_last_of("/"));
        outputDir = QString::fromLocal8Bit(oPath.c_str());
        //qDebug()<<outputDir;
    }
//    bool flag = false;
    flag = false;
    this->outputFile_pre = ui->outputFile->text();
    this->outputFile_suf = ui->outputCombo->currentText();

    //新建一个输出文件夹
    QDir dir;
    dir.cd(this->outputDir);
    if(!dir.exists(this->outputFile_suf)){
        dir.mkdir(this->outputFile_suf);
    }
    this->outputDir+=("\\"+this->outputFile_suf);
    qDebug()<<this->outputDir;

    if(this->outputFile_suf==".fbx"){
        qDebug()<<"outputFile format FBX";
        flag = true;
        // outputFile_suf = ".obj"; 此处不进行修改
    }
    this->outputFile = outputFile_pre+outputFile_suf;
    //qDebug()<<this->outputFile;

    this->fileInput.replace("/","\\");
    this->outputDir.replace("/","\\");
    //for debug
//    qDebug()<<fileInput;
//    qDebug()<<outputDir;
//    qDebug()<<multiple;
//    qDebug()<<outputFile;

    QString curdir = QCoreApplication::applicationDirPath();    //applicationDirPath可以得到生成的.exe的相对路径
    QString program = curdir + "/..\\simplify\\vcgsimp.exe"; //将要连接的.exe文件
    //qDebug() << program;
    QProcess *pro = new QProcess();
    QStringList list;
    list<<"-i";
    list<<this->fileInput;
    list<<"-m";
    list<<QString::number(this->multiple);
    list<<"-p";
    list<<this->outputDir;
    list<<"-o";
    list<<this->outputFile;
    //qDebug()<<list;
    connect(pro, &QProcess::readyRead, this, &MainWindow::slotProcessOutputRedirect);
    connect(pro, SIGNAL(finished(int)), this, SLOT(slotProcessCall_1(int)));
    pro->start(program,list);

}

void MainWindow::slotProcessCall_1(int exitCode)
{
    if(flag){
        QStringList list1;
        qDebug()<<"into flag1";
        QString outputOBJ = this->outputDir+"\\"+this->outputFile_pre+".obj";
        QString curdir = QCoreApplication::applicationDirPath();    //applicationDirPath可以得到生成的.exe的相对路径
        QString program1 = curdir + "/..\\simplify\\FbxConverter.exe"; //连接FBXConverter.exe文件
        qDebug()<<outputOBJ;
        qDebug()<<program1;
        list1<<outputOBJ;
        list1<<this->outputDir+"\\"+this->outputFile;
        QString command = program1 + " " + outputOBJ + " " + this->outputDir+"\\"+this->outputFile;
        qDebug() << command;
//        qDebug()<<list1;
        QProcess *Fpro = new QProcess(this);
        connect(Fpro,SIGNAL(finished(int)), this, SLOT(slotProcessCall(int)));
//        Fpro->start(program1,list1);
        Fpro->start(command);
        qDebug() << Fpro->state();
    }
}
