/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtWebEngine module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qwebengineprofile.h"

#include "qwebenginepage.h"
#include "qwebengineprofile_p.h"

#include "browser_context_adapter.h"

QT_BEGIN_NAMESPACE

/*!
    \class QWebEngineProfile
    \brief The QWebEngineProfile class provides a web-engine profile shared by multiple pages.
    \since 5.5

    \inmodule QtWebEngineWidgets

    QWebEngineProfile contains settings and history shared by all QWebEnginePages that belong
    to the profile.

    A default profile is built-in that all web pages not specifically created with another profile
    belongs to.
*/

/*!
    \enum QWebEngineProfile::HttpCacheType

    This enum describes the HTTP cache types QtWebEngine can be configured to use.

    \value MemoryHttpCache Use a in-memory cache. This is the only setting possible if off-the-record is set or no cache path is available.
    \value DiskHttpCache Use a disk cache. This is the default.
*/

/*!
    \enum QWebEngineProfile::PersistentCookiesPolicy

    This enum describes policy for cookie persistency.

    \value NoPersistentCookies Both session and persistent cookies are stored in memory. This is the only setting possible if off-the-record is set or no persistent data path is available.
    \value AllowPersistentCookies Cookies marked persistent are save and restored from disk, session cookies are only stored to disk for crash recovery. This is the default setting.
    \value ForcePersistentCookies Both session and persistent cookies are save and restored from disk.
*/

QWebEngineProfilePrivate::QWebEngineProfilePrivate(BrowserContextAdapter* browserContext, bool ownsContext)
        : m_browserContext(browserContext)
{
    if (ownsContext)
        m_browserContextRef = browserContext;
}

QWebEngineProfilePrivate::~QWebEngineProfilePrivate()
{
}

/*!
    Constructs a new off-the-record profile.

    An off-the-record profile leaves no record on the local machine, and has no persistent data or cache.
    Thus, the HTTP cache can only be in memory and the cookies only be non-persistent, trying to change
    these settings will have no effect.

    \sa isOffTheRecord()
*/
QWebEngineProfile::QWebEngineProfile(QObject *parent)
    : QObject(parent)
    , d_ptr(new QWebEngineProfilePrivate(new BrowserContextAdapter(true), true))
{
}

/*!
    Constructs a new profile with storage name \a storageName.

    The storage name must be unique.

    A disk-based QWebEngineProfile should be destroyed on or before application exit, otherwise the cache
    and persistent data may not be fully flushed to disk.

    \sa storageName()
*/
QWebEngineProfile::QWebEngineProfile(const QString &storageName, QObject *parent)
    : QObject(parent)
    , d_ptr(new QWebEngineProfilePrivate(new BrowserContextAdapter(storageName), true))
{
}

/*! \internal
*/
QWebEngineProfile::QWebEngineProfile(QWebEngineProfilePrivate *privatePtr)
    : d_ptr(privatePtr)
{
}

/*! \internal
*/
QWebEngineProfile::~QWebEngineProfile()
{
}

/*!
    Returns the storage name for the profile.

    The storage name is used to give each profile that uses the disk separate subdirectories for persistent data and cache.
*/
QString QWebEngineProfile::storageName() const
{
    const Q_D(QWebEngineProfile);
    return d->browserContext()->storageName();
}

/*!
    Returns true if this is an off-the-record profile that leaves no record on the computer.

    This will force cookies and HTTP cache to be in memory, but also force all other normally
    persistent data to be stored in memory.
*/
bool QWebEngineProfile::isOffTheRecord() const
{
    const Q_D(QWebEngineProfile);
    return d->browserContext()->isOffTheRecord();
}

/*!
    Returns the path used to store persistent data for the browser and web content.

    Persistent data includes persistent cookies, HTML5 local storage and visited links.

    By default this is below QStandardPaths::writableLocation(QStandardPaths::DataLocation) in a storage name specific directory.

    \sa setPersistentStoragePath(), storageName(), QStandardPaths::writableLocation()
*/
QString QWebEngineProfile::persistentStoragePath() const
{
    const Q_D(QWebEngineProfile);
    return d->browserContext()->dataPath();
}

/*!
    Overrides the default path used to store persistent web engine data.

    If set to the null string, the default path is restored.

    \sa persistentStoragePath()
*/
void QWebEngineProfile::setPersistentStoragePath(const QString &path)
{
    const Q_D(QWebEngineProfile);
    d->browserContext()->setDataPath(path);
}

/*!
    Returns the path used for caches.

    By default this is below QStandardPaths::writableLocation(QStandardPaths::CacheLocation) in a storage name specific directory.

    \sa setCachePath(), storageName(), QStandardPaths::writableLocation()
*/
QString QWebEngineProfile::cachePath() const
{
    const Q_D(QWebEngineProfile);
    return d->browserContext()->cachePath();
}

/*!
    Overrides the default path used for disk caches.

    If set to the null string, the default path is restored.

    \sa cachePath()
*/
void QWebEngineProfile::setCachePath(const QString &path)
{
    Q_D(QWebEngineProfile);
    d->browserContext()->setCachePath(path);
}

/*!
    Returns the user-agent string send with HTTP to identify the browser.

    \sa setHttpUserAgent()
*/
QString QWebEngineProfile::httpUserAgent() const
{
    const Q_D(QWebEngineProfile);
    return d->browserContext()->httpUserAgent();
}

/*!
    Overrides the default user-agent string, setting it to \a userAgent.

    \sa httpUserAgent()
*/
void QWebEngineProfile::setHttpUserAgent(const QString &userAgent)
{
    Q_D(QWebEngineProfile);
    d->browserContext()->setHttpUserAgent(userAgent);
}

/*!
    Returns the type of HTTP cache used.

    If the profile is off-the-record MemoryHttpCache is returned.

    \sa setHttpCacheType(), cachePath()
*/
QWebEngineProfile::HttpCacheType QWebEngineProfile::httpCacheType() const
{
    const Q_D(QWebEngineProfile);
    return QWebEngineProfile::HttpCacheType(d->browserContext()->httpCacheType());
}

/*!
    Sets the HTTP cache type to \a httpCacheType.

    \sa httpCacheType(), setCachePath()
*/
void QWebEngineProfile::setHttpCacheType(QWebEngineProfile::HttpCacheType httpCacheType)
{
    Q_D(QWebEngineProfile);
    d->browserContext()->setHttpCacheType(BrowserContextAdapter::HttpCacheType(httpCacheType));
}

/*!
    Returns the current policy for persistent cookies.

    If the profile is off-the-record NoPersistentCookies is returned.

    \sa setPersistentCookiesPolicy()
*/
QWebEngineProfile::PersistentCookiesPolicy QWebEngineProfile::persistentCookiesPolicy() const
{
    const Q_D(QWebEngineProfile);
    return QWebEngineProfile::PersistentCookiesPolicy(d->browserContext()->persistentCookiesPolicy());
}

/*!
    Sets the policy for persistent cookies to \a newPersistentCookiesPolicy.

    \sa persistentCookiesPolicy()
*/
void QWebEngineProfile::setPersistentCookiesPolicy(QWebEngineProfile::PersistentCookiesPolicy newPersistentCookiesPolicy)
{
    Q_D(QWebEngineProfile);
    d->browserContext()->setPersistentCookiesPolicy(BrowserContextAdapter::PersistentCookiesPolicy(newPersistentCookiesPolicy));
}

/*!
    Returns the maximum size of the HTTP size.

    Will return 0 if the size is automatically controlled by QtWebEngine.

    \sa setHttpCacheMaximumSize(), httpCacheType()
*/
int QWebEngineProfile::httpCacheMaximumSize() const
{
    const Q_D(QWebEngineProfile);
    return d->browserContext()->httpCacheMaxSize();
}

/*!
    Sets the maximum size of the HTTP cache to \a maxSize.

    Setting it to 0 means the size will be controlled automatically by QtWebEngine.

    \sa httpCacheMaximumSize(), setHttpCacheType()
*/
void QWebEngineProfile::setHttpCacheMaximumSize(int maxSize)
{
    Q_D(QWebEngineProfile);
    d->browserContext()->setHttpCacheMaxSize(maxSize);
}

/*!
    Returns the default profile.

    The default profile uses the storage name "Default".

    \sa storageName()
*/
QWebEngineProfile *QWebEngineProfile::defaultProfile()
{
    static QWebEngineProfile profile(new QWebEngineProfilePrivate(BrowserContextAdapter::defaultContext(), false));
    return &profile;
}

QT_END_NAMESPACE
