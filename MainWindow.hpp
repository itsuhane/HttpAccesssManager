
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onPosted(const QUrl &url, int status, const QByteArray &requestData, const QByteArray &replyData);
};

#endif // MAINWINDOW_HPP
