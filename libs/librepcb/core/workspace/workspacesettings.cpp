/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "workspacesettings.h"

#include "../fileio/fileutils.h"
#include "../serialization/sexpression.h"

#include <QtCore>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

WorkspaceSettings::WorkspaceSettings(const FilePath& fp,
                                     const Version& fileFormat, QObject* parent)
  : QObject(parent),
    mFilePath(fp),
    mFileContent(),
    // Initialize settings items. Their constructor will register them as
    // child objects of this object, this way we will access them later.
    userName("user", "", this),
    applicationLocale("application_locale", "", this),
    defaultLengthUnit("default_length_unit", LengthUnit::millimeters(), this),
    projectAutosaveIntervalSeconds("project_autosave_interval", 600U, this),
    useOpenGl("use_opengl", false, this),
    libraryLocaleOrder("library_locale_order", "locale", QStringList(), this),
    libraryNormOrder("library_norm_order", "norm", QStringList(), this),
    repositoryUrls("repositories", "repository",
                   QList<QUrl>{QUrl("https://api.librepcb.org")}, this),
    useCustomPdfReader("use_custom_pdf_reader", false, this),
    pdfReaderCommand("pdf_custom_reader_command", "", this),
    pdfOpenBehavior("pdf_open_behavior", PdfOpenBehavior::ALWAYS, this) {
  // load settings if the settings file exists
  if (mFilePath.isExistingFile()) {
    qDebug("Load workspace settings...");
    SExpression root =
        SExpression::parse(FileUtils::readFile(mFilePath), mFilePath);
    foreach (const SExpression& child,
             root.getChildren(SExpression::Type::List)) {
      mFileContent.insert(child.getName(), child);
    }
    foreach (WorkspaceSettingsItem* item, getAllItems()) {
      try {
        if (mFileContent.contains(item->getKey())) {
          item->load(mFileContent[item->getKey()], fileFormat);  // can throw
        }
      } catch (const Exception& e) {
        qCritical() << "Could not load workspace settings item:" << e.getMsg();
      }
    }
    qDebug("Workspace settings loaded.");
  } else {
    qInfo("Workspace settings file not found, default settings will be used.");
  }
}

WorkspaceSettings::~WorkspaceSettings() noexcept {
}

/*******************************************************************************
 *  Public Methods
 ******************************************************************************/

void WorkspaceSettings::restoreDefaults() noexcept {
  foreach (WorkspaceSettingsItem* item, getAllItems()) {
    item->restoreDefault();
  }
  mFileContent.clear();  // Remove even unknown settings!
}

QByteArray WorkspaceSettings::saveToByteArray() {
  foreach (const WorkspaceSettingsItem* item, getAllItems()) {
    if (item->isEdited()) {
      if (item->isDefaultValue()) {
        mFileContent.remove(item->getKey());
      } else {
        SExpression node = SExpression::createList(item->getKey());
        item->serialize(node);  // can throw
        mFileContent.insert(item->getKey(), node);
      }
    }
  }

  SExpression root = SExpression::createList("librepcb_workspace_settings");
  foreach (const SExpression& child, mFileContent) {
    root.ensureLineBreak();
    root.appendChild(child);
  }
  root.ensureLineBreakIfMultiLine();
  return root.toByteArray();
}

void WorkspaceSettings::saveToFile() {
  FileUtils::writeFile(mFilePath, saveToByteArray());  // can throw
}

/*******************************************************************************
 *  Private Methods
 ******************************************************************************/

QList<WorkspaceSettingsItem*> WorkspaceSettings::getAllItems() const noexcept {
  return findChildren<WorkspaceSettingsItem*>();
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace librepcb