
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#ifndef NETWORKACCESSMANAGER_HPP
#define NETWORKACCESSMANAGER_HPP

#include <QNetworkAccessManager>
#include <QUrl>

struct HttpConversation {
    QNetworkAccessManager::Operation op;
    QUrl url;
    int status;
    QByteArray requestData;
    QByteArray replyData;
};

class HttpAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit HttpAccessManager(QObject *parent = 0);

    void setUserAgentString(const QString& string);

signals:
    void got(const QUrl &url, int status, const QByteArray &replyData);
    void posted(const QUrl &url, int status, const QByteArray &requestData, const QByteArray &replyData);

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);

private slots:
    void peekSomeReply();
    void peekReplyFinished();

private:
    QString m_userAgentString;
    QMap<QNetworkReply*, HttpConversation> m_conversations;
};

#endif // NETWORKACCESSMANAGER_HPP
