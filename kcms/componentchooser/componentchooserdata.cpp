/*
    SPDX-FileCopyrightText: 2020 Cyril Rossi <cyril.rossi@enioka.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "componentchooserdata.h"

#include "componentchooserbrowser.h"
#include "componentchooseremail.h"
#include "componentchooserfilemanager.h"
#include "componentchoosergeo.h"
#include "componentchooserterminal.h"
#include "componentchoosertexteditor.h"
#include "componentchooserimageviewer.h"

ComponentChooserData::ComponentChooserData(QObject *parent, const QVariantList &args)
    : KCModuleData(parent, args)
    , m_browsers(new ComponentChooserBrowser(this))
    , m_fileManagers(new ComponentChooserFileManager(this))
    , m_terminalEmulators(new ComponentChooserTerminal(this))
    , m_emailClients(new ComponentChooserEmail(this))
    , m_geoUriHandlers(new ComponentChooserGeo(this))
    , m_textEditors(new ComponentChooserTextEditor(this))
    , m_imageViewers(new ComponentChooserImageViewer(this))
{
    load();
}

void ComponentChooserData::load()
{
    m_browsers->load();
    m_fileManagers->load();
    m_terminalEmulators->load();
    m_emailClients->load();
    m_geoUriHandlers->load();
    m_textEditors->load();
    m_imageViewers->load();
}

void ComponentChooserData::save()
{
    m_browsers->save();
    m_fileManagers->save();
    m_terminalEmulators->save();
    m_emailClients->save();
    m_geoUriHandlers->save();
    m_textEditors->save();
    m_imageViewers->save();
}

void ComponentChooserData::defaults()
{
    m_browsers->defaults();
    m_fileManagers->defaults();
    m_terminalEmulators->defaults();
    m_emailClients->defaults();
    m_geoUriHandlers->defaults();
    m_textEditors->defaults();
    m_imageViewers->defaults();
}

bool ComponentChooserData::isDefaults() const
{
    return m_browsers->isDefaults() && m_fileManagers->isDefaults() && m_terminalEmulators->isDefaults() && m_emailClients->isDefaults()
        && m_geoUriHandlers->isDefaults();
        && m_textEditors->isDefaults() && m_imageViewers->isDefaults();
}

bool ComponentChooserData::isSaveNeeded() const
{
    return m_browsers->isSaveNeeded() || m_fileManagers->isSaveNeeded() || m_terminalEmulators->isSaveNeeded() || m_emailClients->isSaveNeeded()
        || m_geoUriHandlers->isSaveNeeded();
        || m_textEditors->isSaveNeeded() || m_imageViewers->isSaveNeeded();
}

ComponentChooser *ComponentChooserData::browsers() const
{
    return m_browsers;
}

ComponentChooser *ComponentChooserData::fileManagers() const
{
    return m_fileManagers;
}

ComponentChooser *ComponentChooserData::terminalEmulators() const
{
    return m_terminalEmulators;
}

ComponentChooser *ComponentChooserData::emailClients() const
{
    return m_emailClients;
}

ComponentChooser *ComponentChooserData::geoUriHandlers() const
ComponentChooser *ComponentChooserData::textEditors() const
{
    return m_geoUriHandlers;
    return m_textEditors;
}

ComponentChooser *ComponentChooserData::imageViewers() const
{
    return m_imageViewers;
}
