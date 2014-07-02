/*
 * Copyright 2011-2014 Nikhil Marathe <nsm.nikhil@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
///////////////////////////////////////////////////////////////////////////////
#ifndef Q_HTTP_RESPONSE_HPP
#define Q_HTTP_RESPONSE_HPP

///////////////////////////////////////////////////////////////////////////////
#include "qhttpserverapi.hpp"
#include "qhttpserverfwd.hpp"

#include <QObject>

///////////////////////////////////////////////////////////////////////////////

/** The QHttpResponse class handles sending data back to the client as a response to a request.
 * The steps to respond correctly are
 * <ol>
 * <li>Call setHeader() to set headers [optional]</li>
 * <li>Call writeHead() with the HTTP status code</li>
 * <li>Call write() zero or more times for body data.</li>
 * <li>Call end() when the resonse can be sent back</li>
 * </ol>
 */
class QHTTPSERVER_API QHttpResponse : public QObject
{
    Q_OBJECT

public:
    virtual     ~QHttpResponse();

public:
    /** Set the value of a HTTP header.
     * @note You must call this with all your custom headers
     *  before calling writeHead(), write() or end(). */
    void        setHeader(const QByteArray& field, const QByteArray& value);

    /** Writes the header section of the response.
     * @param statusCode Status code for the response.
     *  @note Any headers should be set before invoking this function with setHeader(). */
    void        writeHead(TStatusCode statusCode);

    /** Writes a block of data to the client.
     * @note writeHead() must be called before this function. */
    void        write(const QByteArray &data);

    /** Ends (finishes) the response.
     * Data will be flushed to the underlying socket and the connection itself will be closed if
     * this is the last response.
     *
     * This will emit done() and queue this object for deletion.
     * For details see \ref memorymanagement.
     * @param data Optional data to be written before finishing. */
    void        end(const QByteArray &data = QByteArray());

signals:
    /** Emitted when all the data has been sent.
     * This signal indicates that the underlaying socket has transmitted all
     *  of it's buffered data. It is possible to implement memory-efficient
     *  file transfers by calling \ref write() for a block of data only after
     *  receiving this signal. */
    void        allBytesWritten();

    /** Emitted when the response is finished and reports if it was the last response.
     * if it was the last response (google for "Connection: keep-alive / close")
     *  the http connection (socket) will be closed automatically. */
    void        done(bool wasTheLastResponse);

private:
    class       Private;
    Private     *pimp;

    explicit    QHttpResponse(QTcpSocket*);

    friend class QHttpConnection;
};

///////////////////////////////////////////////////////////////////////////////
#endif // define Q_HTTP_RESPONSE_HPP
