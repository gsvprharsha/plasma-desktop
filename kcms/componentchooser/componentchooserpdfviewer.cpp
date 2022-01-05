/*
    SPDX-FileCopyrightText: 2021 Thiago Sueto <herzenschein@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "componentchooserpdfviewer.h"

ComponentChooserPdfViewer::ComponentChooserPdfViewer(QObject *parent)
    : ComponentChooser(parent,
                       QStringLiteral("application/pdf"),
                       QStringLiteral("Viewer"),
                       QStringLiteral("okularApplication_pdf.desktop"),
                       i18n("Select default PDF viewer"))
{
}

void ComponentChooserPdfViewer::save()
{
    saveMimeTypeAssociation(QStringLiteral("application/pdf"), m_applications[m_index].toMap()[QStringLiteral("storageId")].toString());
}
