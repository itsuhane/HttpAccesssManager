
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#include "HttpAccessManager.hpp"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QBuffer>

HttpAccessManager::HttpAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{}

void HttpAccessManager::setUserAgentString(const QString &string)
{
    m_userAgentString = string;
}

QNetworkReply *HttpAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkRequest newRequest = request;
    if(!m_userAgentString.isNull()) {
        newRequest.setHeader(QNetworkRequest::UserAgentHeader, m_userAgentString);
    }

    QByteArray requestData;

    if(outgoingData) {
        QBuffer *buffer = new QBuffer(outgoingData);
        requestData = outgoingData->readAll();
        buffer->setData(requestData);
        outgoingData = buffer;
    }

    QNetworkReply *reply = QNetworkAccessManager::createRequest(op, newRequest, outgoingData);

    m_conversations[reply].op = op;
    m_conversations[reply].url = reply->url(); // here we use reply->url() because it would be normalized
    m_conversations[reply].requestData = requestData;
    connect(reply, SIGNAL(readyRead()), this, SLOT(peekSomeReply()));
    connect(reply, SIGNAL(finished()), this, SLOT(peekReplyFinished()));
    return reply;
}

void HttpAccessManager::peekSomeReply()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply*>(sender());
    HttpConversation& conversation = m_conversations[reply];
    conversation.replyData.append(reply->peek(reply->bytesAvailable()));
}

void HttpAccessManager::peekReplyFinished()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply*>(sender());
    HttpConversation& conversation = m_conversations[reply];
    conversation.replyData.append(reply->peek(reply->bytesAvailable()));

    conversation.status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch(conversation.op) {
    case QNetworkAccessManager::GetOperation:
        emit got(conversation.url, conversation.status, conversation.replyData);
        break;
    case QNetworkAccessManager::PostOperation:
        emit posted(conversation.url, conversation.status, conversation.requestData, conversation.replyData);
        break;
    default:
        break;
    }
}

