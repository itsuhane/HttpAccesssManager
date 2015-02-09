
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#include "MainWindow.hpp"
#include <QWebView>
#include <QDebug>
#include "HttpAccessManager.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    HttpAccessManager *manager = new HttpAccessManager(this);

    QWebPage *page = new QWebPage;
    page->setNetworkAccessManager(manager);

    QWebView *view = new QWebView(this);
    view->setPage(page);
    setCentralWidget(view);

    connect(manager, SIGNAL(posted(QUrl,int,QByteArray,QByteArray)), this, SLOT(onPosted(QUrl,int,QByteArray,QByteArray)));

    view->setUrl(QUrl("http://httpbin.org/forms/post"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::onPosted(const QUrl &url, int status, const QByteArray &requestData, const QByteArray &replyData)
{
    qDebug()<<url;
    qDebug()<<status;
    qDebug()<<requestData;
    qDebug()<<replyData;
}
