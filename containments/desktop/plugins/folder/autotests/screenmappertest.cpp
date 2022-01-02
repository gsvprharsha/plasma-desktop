/*
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group company <info@kdab.com>

    Work sponsored by the LiMux project of the city of Munich.
    SPDX-FileContributor: Andras Mantia <andras.mantia@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "screenmappertest.h"
#include "screenmapper.h"

#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(ScreenMapperTest)

void ScreenMapperTest::initTestCase()
{
    m_screenMapper = ScreenMapper::instance();
    m_currentActivity = QStringLiteral("00000000-0000-0000-0000-000000000000");
}

void ScreenMapperTest::init()
{
    m_screenMapper->cleanup();
}

void ScreenMapperTest::tst_addScreens()
{
    const auto path = ScreenMapper::stringToUrl(QStringLiteral("desktop:/"));
    QSignalSpy s(m_screenMapper, &ScreenMapper::screensChanged);
    m_screenMapper->addScreen(-1, m_currentActivity, path);
    QCOMPARE(s.count(), 0);
    m_screenMapper->addScreen(1, m_currentActivity, path);
    QCOMPARE(s.count(), 1);
    m_screenMapper->addScreen(0, m_currentActivity, path);
    QCOMPARE(s.count(), 2);
    m_screenMapper->addScreen(1, m_currentActivity, path);
    QCOMPARE(s.count(), 2);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path, m_currentActivity), 0);
}

void ScreenMapperTest::tst_removeScreens()
{
    const auto path = ScreenMapper::stringToUrl(QStringLiteral("desktop:/"));
    addScreens(path);
    QSignalSpy s(m_screenMapper, &ScreenMapper::screensChanged);
    m_screenMapper->removeScreen(-1, m_currentActivity, path);
    QCOMPARE(s.count(), 0);
    m_screenMapper->removeScreen(1, m_currentActivity, path);
    QCOMPARE(s.count(), 1);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path, m_currentActivity), 0);
    m_screenMapper->removeScreen(1, m_currentActivity, path);
    QCOMPARE(s.count(), 1);
    m_screenMapper->addScreen(3, m_currentActivity, path);
    QCOMPARE(s.count(), 2);
    m_screenMapper->removeScreen(0, m_currentActivity, path);
    QCOMPARE(s.count(), 3);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path, m_currentActivity), 2);
}

void ScreenMapperTest::tst_addMapping()
{
    const auto path = ScreenMapper::stringToUrl(QStringLiteral("desktop:/"));
    addScreens(path);
    QSignalSpy s(m_screenMapper, &ScreenMapper::screenMappingChanged);
    QString file(QStringLiteral("desktop:/foo%1.txt"));

    for (int i = 0; i < 3; i++) {
        const QUrl url = ScreenMapper::stringToUrl(file.arg(i));
        m_screenMapper->addMapping(url, i, m_currentActivity);
        QCOMPARE(s.count(), i + 1);
        QCOMPARE(m_screenMapper->screenForItem(url, m_currentActivity), i);
    }
}

void ScreenMapperTest::tst_addRemoveScreenWithItems()
{
    const auto path = ScreenMapper::stringToUrl(QStringLiteral("desktop:/"));
    addScreens(path);
    QString file(QStringLiteral("desktop:/foo%1.txt"));

    for (int i = 0; i < 3; i++) {
        const QUrl url = ScreenMapper::stringToUrl(file.arg(i));
        m_screenMapper->addMapping(url, i, m_currentActivity);
    }

    // remove one screen
    m_screenMapper->removeScreen(1, m_currentActivity, path);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(0)), m_currentActivity), 0);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(1)), m_currentActivity), -1);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(2)), m_currentActivity), 2);

    // add removed screen back, items screen is restored
    m_screenMapper->addScreen(1, m_currentActivity, path);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(0)), m_currentActivity), 0);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(1)), m_currentActivity), 1);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(2)), m_currentActivity), 2);

    // remove all screens, firstAvailableScreen changes
    m_screenMapper->removeScreen(0, m_currentActivity, path);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path, m_currentActivity), 1);
    m_screenMapper->removeScreen(1, m_currentActivity, path);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path, m_currentActivity), 2);
    m_screenMapper->removeScreen(2, m_currentActivity, path);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path, m_currentActivity), -1);

    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(0)), m_currentActivity), -1);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(1)), m_currentActivity), -1);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(2)), m_currentActivity), -1);

    // add all screens back, all item's screen is restored
    addScreens(path);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(0)), m_currentActivity), 0);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(1)), m_currentActivity), 1);
    QCOMPARE(m_screenMapper->screenForItem(ScreenMapper::stringToUrl(file.arg(2)), m_currentActivity), 2);

    // remove one screen and move its item
    const QUrl movedItem = ScreenMapper::stringToUrl(file.arg(1));
    m_screenMapper->removeScreen(1, m_currentActivity, path);
    QCOMPARE(m_screenMapper->screenForItem(movedItem, m_currentActivity), -1);
    m_screenMapper->addMapping(movedItem, 0, m_currentActivity);
    QCOMPARE(m_screenMapper->screenForItem(movedItem, m_currentActivity), 0);

    // add back the screen, item goes back to the original place
    m_screenMapper->addScreen(1, m_currentActivity, path);
    QCOMPARE(m_screenMapper->screenForItem(movedItem, m_currentActivity), 1);
}

void ScreenMapperTest::tst_addRemoveScreenDifferentPaths()
{
    const auto path = ScreenMapper::stringToUrl(QStringLiteral("desktop:/Foo"));
    const auto path2 = ScreenMapper::stringToUrl(QStringLiteral("desktop:/Foo2"));
    m_screenMapper->addScreen(0, m_currentActivity, path);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path, m_currentActivity), 0);
    QCOMPARE(m_screenMapper->firstAvailableScreen(path2, m_currentActivity), -1);
}

void ScreenMapperTest::addScreens(const QUrl &path)
{
    m_screenMapper->addScreen(0, m_currentActivity, path);
    m_screenMapper->addScreen(1, m_currentActivity, path);
    m_screenMapper->addScreen(2, m_currentActivity, path);
}
