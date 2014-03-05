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
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "web_engine_visited_links_manager.h"

#include "content_browser_client_qt.h"
#include "browser_context_qt.h"
#include "type_conversion.h"

#include "base/memory/scoped_ptr.h"
#include "components/visitedlink/browser/visitedlink_delegate.h"
#include "components/visitedlink/browser/visitedlink_master.h"

namespace {
class BasicUrlIterator : public visitedlink::VisitedLinkMaster::URLIterator {
public:
    BasicUrlIterator(const QList<QUrl> &urls) : m_urls(urls) {}
    virtual const GURL& NextURL() { m_currentUrl = toGurl(m_urls.takeFirst()); return m_currentUrl; }
    virtual bool HasNextURL() const { return !m_urls.isEmpty(); }
private:
    QList<QUrl> m_urls;
    GURL m_currentUrl;

};
} // Anonymous namespace

// Due to the design of the visitedLink component, it seems safer to provide a
// basic delegate that will simply rebuild an empty visitedLink table if needed
// from the application history rather than crashing. This is functionality
// that was covered by QWebHistoryInterface in QtWebKitWidgets.

class VisitedLinkDelegateQt : public visitedlink::VisitedLinkDelegate
{
public:
    ~VisitedLinkDelegateQt() {}
    void RebuildTable(const scoped_refptr<URLEnumerator>& enumerator) { enumerator->OnComplete(true); }
};

void WebEngineVisitedLinksManager::deleteAllVisitedLinkData()
{
    m_visitedLinkMaster->DeleteAllURLs();
}

void WebEngineVisitedLinksManager::deleteVisitedLinkDataForUrls(const QList<QUrl> &urlsToDelete)
{
    BasicUrlIterator iterator(urlsToDelete);
    m_visitedLinkMaster->DeleteURLs(&iterator);
}

WebEngineVisitedLinksManager::WebEngineVisitedLinksManager()
    : m_delegate(new VisitedLinkDelegateQt)
{
    Q_ASSERT(ContentBrowserClientQt::Get() && ContentBrowserClientQt::Get()->browser_context());
    BrowserContextQt *browserContext = ContentBrowserClientQt::Get()->browser_context();
    m_visitedLinkMaster.reset(new visitedlink::VisitedLinkMaster(browserContext, m_delegate.data(), /* persist to disk = */true));
    m_visitedLinkMaster->Init();
}

WebEngineVisitedLinksManager::~WebEngineVisitedLinksManager()
{
}

void WebEngineVisitedLinksManager::addUrl(const GURL &urlToAdd)
{
    Q_ASSERT(m_visitedLinkMaster);
    m_visitedLinkMaster->AddURL(urlToAdd);
}
