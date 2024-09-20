#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , httpManager_(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    connect(ui->get_pushButton,&QPushButton::clicked,this,&MainWindow::get_request);
    connect(ui->post_pushButton,&QPushButton::clicked,this,&MainWindow::post_request);
    connect(this,&MainWindow::http_finished,this,&MainWindow::process_reply);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::get_request(){
    // 构建 HTTP 请求
    QNetworkRequest request_(QUrl("http://api.qingyunke.com/api.php?key=free&appid=0&msg=鹅鹅鹅"));
    request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送 HTTP GET 请求
    QNetworkReply* response_ = httpManager_->get(request_);

    // 如果发出 QNetworkReply::finished信号 服务器的响应已经接收到，网络请求（无论成功还是失败）都已经结束
    // 因此，我们在该信号之后应该先检查网络请求是成功还是失败
    connect(response_,&QNetworkReply::finished,[response_,this](){
        if(response_->error() == QNetworkReply::NoError){   // 没有发送错误
            ui->message_label->setText(tr("Get success"));
            ui->message_label->setStyleSheet("color:green");
            emit http_finished(response_);
        }else{
            ui->message_label->setText(tr("Get faile"));
            ui->message_label->setStyleSheet("color:red");
        }
    });
}
void MainWindow::post_request(){
    // 构造 JSON 数据
    QJsonObject JsonData;
    JsonData.insert("studentID",13);
    JsonData.insert("name","xy");

    // 转换为 QByteArray，才可以用网络发出去
    QJsonDocument jsonDoc(JsonData);
    QByteArray data = jsonDoc.toJson();

    //--------------------下面的逻辑 和 get_request 基本相同，唯一不同的是 POST 请求要发送 JSON 数据--------------------//

    // 构建 HTTP 请求
    QNetworkRequest request_(QUrl("http://api.qingyunke.com/api.php?key=free&appid=0&msg=鹅鹅鹅"));
    request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送 HTTP POST 请求
    QNetworkReply* response_ = httpManager_->post(request_,data);

    connect(response_,&QNetworkReply::finished,[response_,this](){
        if(response_->error() == QNetworkReply::NoError){   // 没有发送错误
            ui->message_label->setText(tr("POST success"));
            ui->message_label->setStyleSheet("color:green");
            emit http_finished(response_);
        }else{
            ui->message_label->setText(tr("POST faile"));
            ui->message_label->setStyleSheet("color:red");
        }
    });
}

void MainWindow::process_reply(QNetworkReply *response)
{
    //auto data = QString::fromUtf8(response->readAll());    // 读取响应数据,并转换为 UTF-8 格式

    // 解析 JSON 数据
    auto data = response->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data,&jsonError);   // 将 JSON 解析为 UTF-8 编码的 JSON 文档，并从中创建 QJsonDocument
    if(jsonError.error != QJsonParseError::NoError){    // 检查解析是否错误
        qDebug()<<"Json parse error";
        return;
    }
    // 确保 JSON 解析没有出错，开始处理
    if(jsonDoc.isObject()){ // 确保是 一个 Object
        QJsonObject jsonData = jsonDoc.object();    // 转换为 JSON对象，即 key-value 形式的 JSON 数据
        if(jsonData.contains("content")){   // 判断 key 真实存在
            qDebug()<<"content = "<<jsonData.value("content").toString();   // 取得 key 对应的 value
        }
    }
}

