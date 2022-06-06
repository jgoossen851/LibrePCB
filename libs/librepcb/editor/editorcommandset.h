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

#ifndef LIBREPCB_EDITOR_EDITORCOMMANDSET_H
#define LIBREPCB_EDITOR_EDITORCOMMANDSET_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "editorcommand.h"
#include "editorcommandcategory.h"

#include <QtCore>
#include <QtWidgets>

/*******************************************************************************
 *  Namespace / Forward Declarations
 ******************************************************************************/
namespace librepcb {
namespace editor {

/*******************************************************************************
 *  Class EditorCommandSet
 ******************************************************************************/

/**
 * @brief Collection of all commands across all editors
 *
 * @see ::librepcb::editor::EditorCommand
 * @see https://en.wikipedia.org/wiki/Table_of_keyboard_shortcuts
 * @see https://librepcb.discourse.group/t/hotkeys-anyone/229
 */
class EditorCommandSet final {
  Q_DECLARE_TR_FUNCTIONS(EditorCommandSet)

private:
  EditorCommandCategory categoryRoot{"categoryRoot", QString()};

  EditorCommandSet() noexcept {}
  ~EditorCommandSet() noexcept {}

public:
  // General Methods
  static EditorCommandSet& instance() noexcept {
    static EditorCommandSet obj;
    return obj;
  }
  QList<EditorCommandCategory*> getCategories() noexcept {
    return categoryRoot.findChildren<EditorCommandCategory*>();
  }
  QList<EditorCommand*> getCommands(
      const EditorCommandCategory* category) noexcept {
    Q_ASSERT(category);
    return category->findChildren<EditorCommand*>();
  }

  EditorCommandCategory categoryEditor{"categoryEditor", tr("Editor"),
                                       &categoryRoot};
  EditorCommand itemNew{
      "item_new",  // clang-format break
      tr("New"),
      tr("Add a new item"),
      QIcon(":/img/actions/new.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::New)},
      &categoryEditor,
  };
  EditorCommand itemOpen{
      "item_open",  // clang-format break
      tr("Open"),
      tr("Open the selected item(s)"),
      QIcon(":/img/actions/open.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_Return)},
      &categoryEditor,
  };
  EditorCommand save{
      "save",  // clang-format break
      tr("Save"),
      tr("Save changes to filesystem"),
      QIcon(":/img/actions/save.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Save)},
      &categoryEditor,
  };
  EditorCommand saveAll{
      "save_all",  // clang-format break
      tr("Save All"),
      tr("Save all elements to filesystem"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S)},
      &categoryEditor,
  };
  EditorCommand selectAll{
      "select_all",  // clang-format break
      tr("Select All"),
      tr("Select all visible objects"),
      QIcon(":/img/actions/select_all.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::SelectAll)},
      &categoryEditor,
  };
  EditorCommand find{
      "find",  // clang-format break
      tr("Find"),
      tr("Find or filter objects"),
      QIcon(":/img/actions/search.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Find)},
      &categoryEditor,
  };
  EditorCommand findNext{
      "find_next",  // clang-format break
      tr("Find Next"),
      tr("Go to the next found object"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::FindNext)},
      &categoryEditor,
  };
  EditorCommand findPrevious{
      "find_previous",  // clang-format break
      tr("Find Previous"),
      tr("Go to the previous found object"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::FindPrevious)},
      &categoryEditor,
  };
  EditorCommand fileManager{
      "file_manager",  // clang-format break
      tr("Show in File Manager"),
      tr("Open the directory in the file manager"),
      QIcon(":/img/places/folder.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryEditor,
  };
  EditorCommand controlPanel{
      "control_panel",  // clang-format break
      tr("Control Panel"),
      tr("Bring the control panel window to front"),
      QIcon(":/img/actions/home.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Home)},
      &categoryEditor,
  };
  EditorCommand workspaceSwitch{
      "workspace_switch",  // clang-format break
      tr("Switch Workspace"),
      tr("Choose another workspace to open"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryEditor,
  };
  EditorCommand workspaceSettings{
      "workspace_settings",  // clang-format break
      tr("Workspace Settings"),
      tr("Open the workspace settings dialog"),
      QIcon(":/img/actions/settings.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable |
          EditorCommand::Flag::PreferencesRole,
      {QKeySequence(Qt::CTRL + Qt::Key_Comma)},
      &categoryEditor,
  };
  EditorCommand workspaceLibrariesRescan{
      "workspace_libraries_rescan",  // clang-format break
      tr("Rescan Libraries"),
      tr("Scan all workspace libraries to update the cache"),
      QIcon(":/img/actions/refresh.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F5)},
      &categoryEditor,
  };
  EditorCommand libraryManager{
      "library_manager",  // clang-format break
      tr("Library Manager"),
      tr("Open the library manager window"),
      QIcon(":/img/library/package.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_M)},
      &categoryEditor,
  };
  EditorCommand favoriteAdd{
      "favorite_add",  // clang-format break
      tr("Add To Favorites"),
      tr("Add project to favorites"),
      QIcon(":/img/actions/bookmark_gray.png"),
      EditorCommand::Flags(),
      {},
      &categoryEditor,
  };
  EditorCommand favoriteRemove{
      "favorite_remove",  // clang-format break
      tr("Remove From Favorites"),
      tr("Remove project from favorites"),
      QIcon(":/img/actions/bookmark.png"),
      EditorCommand::Flags(),
      {},
      &categoryEditor,
  };
  EditorCommand libraryElementNew{
      "library_element_new",  // clang-format break
      tr("New Library Element"),
      tr("Create a new library element"),
      QIcon(":/img/actions/new.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::New)},
      &categoryEditor,
  };
  EditorCommand libraryElementDuplicate{
      "library_element_duplicate",  // clang-format break
      tr("Duplicate"),
      tr("Create a new element by duplicating this one"),
      QIcon(":/img/actions/clone.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_D)},
      &categoryEditor,
  };
  EditorCommand projectNew{
      "project_new",  // clang-format break
      tr("New Project"),
      tr("Create a new project"),
      QIcon(":/img/actions/new.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::New)},
      &categoryEditor,
  };
  EditorCommand projectOpen{
      "project_open",  // clang-format break
      tr("Open Project"),
      tr("Open an existing project"),
      QIcon(":/img/actions/open.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Open)},
      &categoryEditor,
  };
  EditorCommand projectSave{
      "project_save",  // clang-format break
      tr("Save Project"),
      tr("Save the currently opened project"),
      QIcon(":/img/actions/save.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Save)},
      &categoryEditor,
  };
  EditorCommand projectSettings{
      "project_settings",  // clang-format break
      tr("Project Settings"),
      tr("View/modify the project settings"),
      QIcon(":/img/actions/settings.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F6)},
      &categoryEditor,
  };
  EditorCommand projectProperties{
      "project_properties",  // clang-format break
      tr("Project Properties"),
      tr("View/modify the project properties"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F7)},
      &categoryEditor,
  };
  EditorCommand gridProperties{
      "grid_properties",  // clang-format break
      tr("Grid Properties"),
      tr("View/modify the grid properties"),
      QIcon(":/img/actions/grid.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F8)},
      &categoryEditor,
  };
  EditorCommand netClasses{
      "net_classes",  // clang-format break
      tr("Net Classes"),
      tr("View/modify the net classes"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F9)},
      &categoryEditor,
  };
  EditorCommand layerStack{
      "layer_stack",  // clang-format break
      tr("Layer Stack"),
      tr("View/modify the board layer stack setup"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F10)},
      &categoryEditor,
  };
  EditorCommand designRules{
      "design_rules",  // clang-format break
      tr("Design Rules"),
      tr("View/modify the board design rules"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F11)},
      &categoryEditor,
  };
  EditorCommand designRuleCheck{
      "design_rule_check",  // clang-format break
      tr("Design Rule Check"),
      tr("Run the design rule check (DRC)"),
      QIcon(":/img/actions/drc.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F12)},
      &categoryEditor,
  };
  EditorCommand projectLibraryUpdate{
      "project_library_update",  // clang-format break
      tr("Update Project Library"),
      tr("Update the project's library elements from workspace libraries"),
      QIcon(":/img/actions/refresh.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_F5)},
      &categoryEditor,
  };
  EditorCommand schematicEditor{
      "schematic_editor",  // clang-format break
      tr("Schematic Editor"),
      tr("Bring the schematic editor window to front"),
      QIcon(":/img/actions/schematic.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_S)},
      &categoryEditor,
  };
  EditorCommand sheetNew{
      "sheet_new",  // clang-format break
      tr("New Sheet"),
      tr("Add a new schematic sheet to the project"),
      QIcon(":/img/actions/new.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::New)},
      &categoryEditor,
  };
  EditorCommand sheetRename{
      "sheet_rename",  // clang-format break
      tr("Rename Sheet"),
      tr("Rename the current schematic sheet"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_F2)},
      &categoryEditor,
  };
  EditorCommand sheetRemove{
      "sheet_remove",  // clang-format break
      tr("Remove Sheet"),
      tr("Remove the current schematic sheet from the project"),
      QIcon(":/img/actions/delete.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_Delete)},
      &categoryEditor,
  };
  EditorCommand boardEditor{
      "board_editor",  // clang-format break
      tr("Board Editor"),
      tr("Bring the board editor window to front"),
      QIcon(":/img/actions/board_editor.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_B)},
      &categoryEditor,
  };
  EditorCommand boardNew{
      "board_new",  // clang-format break
      tr("New Board"),
      tr("Add a new board to the project"),
      QIcon(":/img/actions/new.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::New)},
      &categoryEditor,
  };
  EditorCommand boardCopy{
      "board_copy",  // clang-format break
      tr("Copy Board"),
      tr("Add a copy of the current board to the project"),
      QIcon(":/img/actions/copy.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_D)},
      &categoryEditor,
  };
  EditorCommand boardRemove{
      "board_remove",  // clang-format break
      tr("Remove Board"),
      tr("Remove the current board from the project"),
      QIcon(":/img/actions/delete.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_Delete)},
      &categoryEditor,
  };
  EditorCommand planeShowAll{
      "plane_show_all",  // clang-format break
      tr("Show All Planes"),
      tr("Make the filled areas of all planes visible"),
      QIcon(":/img/actions/show_planes.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryEditor,
  };
  EditorCommand planeHideAll{
      "plane_hide_all",  // clang-format break
      tr("Hide All Planes"),
      tr("Make the filled areas of all planes invisible"),
      QIcon(":/img/actions/hide_planes.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryEditor,
  };
  EditorCommand planeRebuildAll{
      "plane_rebuild_all",  // clang-format break
      tr("Rebuild All Planes"),
      tr("Re-calculate the filled areas of all planes"),
      QIcon(":/img/actions/rebuild_plane.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R)},
      &categoryEditor,
  };

  EditorCommandCategory categoryImportExport{
      "categoryImportExport", tr("Import/Export"), &categoryRoot};
  EditorCommand importDxf{
      "import_dxf",  // clang-format break
      tr("Import DXF"),
      tr("Import a 2D mechanical drawing"),
      QIcon(":/img/actions/export_svg.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryImportExport,
  };
  EditorCommand importEagleLibrary{
      "import_eagle_library",  // clang-format break
      tr("Import EAGLE Library"),
      tr("Import library elements from an EAGLE *.lbr file"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryImportExport,
  };
  EditorCommand exportLppz{
      "export_lppz",  // clang-format break
      tr("Export *.lppz Archive"),
      tr("Export the project as a self-contained *.lppz archive"),
      QIcon(":/img/actions/export_zip.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryImportExport,
  };
  EditorCommand exportImage{
      "export_image",  // clang-format break
      tr("Export Image"),
      tr("Export graphics as a pixmap"),
      QIcon(":/img/actions/export_pixmap.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_I)},
      &categoryImportExport,
  };
  EditorCommand exportPdf{
      "export_pdf",  // clang-format break
      tr("Export PDF"),
      tr("Export graphics as a PDF"),
      QIcon(":/img/actions/pdf.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P)},
      &categoryImportExport,
  };
  EditorCommand print{
      "print",  // clang-format break
      tr("Print"),
      tr("Send graphics to a printer"),
      QIcon(":/img/actions/print.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Print)},
      &categoryImportExport,
  };
  EditorCommand generateBom{
      "generate_bom",  // clang-format break
      tr("Generate Bill Of Materials"),
      tr("Generate bill of materials (BOM) file"),
      QIcon(":/img/actions/generate_bom.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryImportExport,
  };
  EditorCommand generateFabricationData{
      "generate_fabrication_data",  // clang-format break
      tr("Generate Fabrication Data"),
      tr("Generate Gerber/Excellon files for PCB fabrication"),
      QIcon(":/img/actions/export_gerber.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryImportExport,
  };
  EditorCommand generatePickPlace{
      "generate_pick_place",  // clang-format break
      tr("Generate Pick&&Place Files"),
      tr("Generate pick&place files for automated PCB assembly"),
      QIcon(":/img/actions/export_pick_place_file.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryImportExport,
  };
  EditorCommand orderPcb{
      "order_pcb",  // clang-format break
      tr("Order PCB"),
      tr("Start ordering the PCB online"),
      QIcon(":/img/actions/order_pcb.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_O)},
      &categoryImportExport,
  };

  EditorCommandCategory categoryModify{"categoryModify", tr("Modify"),
                                       &categoryRoot};
  EditorCommand undo{
      "undo",  // clang-format break
      tr("Undo"),
      tr("Revert the last modification"),
      QIcon(":/img/actions/undo.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      // Don't use QKeySequence::Undo for consistency with the Redo shortcuts.
      {QKeySequence(Qt::CTRL + Qt::Key_Z)},
      &categoryModify,
  };
  EditorCommand redo{
      "redo",  // clang-format break
      tr("Redo"),
      tr("Re-apply the last reverted modification"),
      QIcon(":/img/actions/redo.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_Y),
       QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z)},
      &categoryModify,
  };
  EditorCommand clipboardCut{
      "clipboard_cut",  // clang-format break
      tr("Cut"),
      tr("Cut the selected object(s) to clipboard"),
      QIcon(":/img/actions/cut.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Cut)},
      &categoryModify,
  };
  EditorCommand clipboardCopy{
      "clipboard_copy",  // clang-format break
      tr("Copy"),
      tr("Copy the selected object(s) to clipboard"),
      QIcon(":/img/actions/copy.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Copy)},
      &categoryModify,
  };
  EditorCommand clipboardPaste{
      "clipboard_paste",  // clang-format break
      tr("Paste"),
      tr("Paste object(s) from the clipboard"),
      QIcon(":/img/actions/paste.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Paste)},
      &categoryModify,
  };
  EditorCommand rotateCcw{
      "rotate_ccw",  // clang-format break
      tr("Rotate Counterclockwise"),
      tr("Rotate the selected object(s) counterclockwise"),
      QIcon(":/img/actions/rotate_left.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_R)},
      &categoryModify,
  };
  EditorCommand rotateCw{
      "rotate_cw",  // clang-format break
      tr("Rotate Clockwise"),
      tr("Rotate the selected object(s) clockwise"),
      QIcon(":/img/actions/rotate_right.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::SHIFT + Qt::Key_R)},
      &categoryModify,
  };
  EditorCommand mirrorHorizontal{
      "mirror_horizontal",  // clang-format break
      tr("Mirror Horizontally"),
      tr("Mirror the selected object(s) horizontally"),
      QIcon(":/img/actions/mirror_horizontal.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_M)},
      &categoryModify,
  };
  EditorCommand mirrorVertical{
      "mirror_vertical",  // clang-format break
      tr("Mirror Vertically"),
      tr("Mirror the selected object(s) vertically"),
      QIcon(":/img/actions/mirror_vertical.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::SHIFT + Qt::Key_M)},
      &categoryModify,
  };
  EditorCommand flipHorizontal{
      "flip_horizontal",  // clang-format break
      tr("Flip Horizontally"),
      tr("Flip the selected object(s) horizontally to the other board side"),
      QIcon(":/img/actions/flip_horizontal.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F)},
      &categoryModify,
  };
  EditorCommand flipVertical{
      "flip_vertical",  // clang-format break
      tr("Flip Vertically"),
      tr("Flip the selected object(s) vertically to the other board side"),
      QIcon(":/img/actions/flip_vertical.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::SHIFT + Qt::Key_F)},
      &categoryModify,
  };
  EditorCommand snapToGrid{
      "snap_to_grid",  // clang-format break
      tr("Snap to Grid"),
      tr("Move the selected object(s) to snap the grid"),
      QIcon(":/img/actions/grid.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_S)},
      &categoryModify,
  };
  EditorCommand deviceResetTextAll{
      "device_reset_text_all",  // clang-format break
      tr("Reset All Texts"),
      tr("Reset all texts of the footprint to their initial state"),
      QIcon(":/img/actions/undo.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryModify,
  };
  EditorCommand properties{
      "properties",  // clang-format break
      tr("Properties"),
      tr("View/modify the object properties"),
      QIcon(":/img/actions/settings.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_E)},
      &categoryModify,
  };
  EditorCommand rename{
      "rename",  // clang-format break
      tr("Rename"),
      tr("Rename the selected object"),
      QIcon(":/img/actions/edit.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_F2)},
      &categoryModify,
  };
  EditorCommand remove{
      "remove",  // clang-format break
      tr("Remove"),
      tr("Delete the selected object(s)"),
      QIcon(":/img/actions/delete.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::Delete)},
      &categoryModify,
  };

  EditorCommandCategory categoryView{"categoryView", tr("View"), &categoryRoot};
  EditorCommand zoomFitContent{
      "zoom_fit_content",  // clang-format break
      tr("Zoom to Fit Contents"),
      tr("Set the zoom level to fit the whole content"),
      QIcon(":/img/actions/zoom_all.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_Home)},
      &categoryView,
  };
  EditorCommand zoomIn{
      "zoom_in",  // clang-format break
      tr("Zoom In"),
      tr("Increase the zoom level"),
      QIcon(":/img/actions/zoom_in.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::ZoomIn)},
      &categoryView,
  };
  EditorCommand zoomOut{
      "zoom_out",  // clang-format break
      tr("Zoom Out"),
      tr("Decrease the zoom level"),
      QIcon(":/img/actions/zoom_out.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::ZoomOut)},
      &categoryView,
  };
  EditorCommand gridIncrease{
      "grid_increase",  // clang-format break
      tr("Increase Grid Interval"),
      tr("Increase the grid interval"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Plus)},
      &categoryView,
  };
  EditorCommand gridDecrease{
      "grid_decrease",  // clang-format break
      tr("Decrease Grid Interval"),
      tr("Decrease the grid interval"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Minus)},
      &categoryView,
  };

  EditorCommandCategory categoryTools{"categoryTools", tr("Tools"),
                                      &categoryRoot};
  EditorCommand toolSelect{
      "tool_select",  // clang-format break
      tr("Select"),
      tr("Select & modify existing objects"),
      QIcon(":/img/actions/select.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryTools,
  };
  EditorCommand toolLine{
      "tool_line",  // clang-format break
      tr("Draw Line"),
      tr("Draw graphical lines"),
      QIcon(":/img/actions/draw_line.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_L)},
      &categoryTools,
  };
  EditorCommand toolRect{
      "tool_rect",  // clang-format break
      tr("Draw Rectangle"),
      tr("Draw graphical rectangles"),
      QIcon(":/img/actions/draw_rectangle.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_G)},
      &categoryTools,
  };
  EditorCommand toolPolygon{
      "tool_polygon",  // clang-format break
      tr("Draw Polygon"),
      tr("Draw graphical polygons"),
      QIcon(":/img/actions/draw_polygon.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_P)},
      &categoryTools,
  };
  EditorCommand toolCircle{
      "tool_circle",  // clang-format break
      tr("Draw Circle"),
      tr("Draw graphical circles"),
      QIcon(":/img/actions/draw_circle.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_C)},
      &categoryTools,
  };
  EditorCommand toolText{
      "tool_text",  // clang-format break
      tr("Add Text"),
      tr("Add graphical text objects"),
      QIcon(":/img/actions/add_text.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_T)},
      &categoryTools,
  };
  EditorCommand toolName{
      "tool_name",  // clang-format break
      tr("Add Name"),
      tr("Add graphical text objects for %1").arg("{{NAME}}"),
      QIcon(":/img/actions/add_name.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_N)},
      &categoryTools,
  };
  EditorCommand toolValue{
      "tool_value",  // clang-format break
      tr("Add Value"),
      tr("Add graphical text objects for %1").arg("{{VALUE}}"),
      QIcon(":/img/actions/add_value.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_V)},
      &categoryTools,
  };
  EditorCommand toolPin{
      "tool_pin",  // clang-format break
      tr("Add Pin"),
      tr("Add symbol pins (electrical connections for schematics)"),
      QIcon(":/img/actions/add_symbol_pin.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_I)},
      &categoryTools,
  };
  EditorCommand toolPadSmt{
      "tool_pad_smt",  // clang-format break
      tr("Add SMT Pad"),
      tr("Add surface mounted (single layer) copper pads"),
      QIcon(":/img/actions/add_smt_pad.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_D)},
      &categoryTools,
  };
  EditorCommand toolPadTht{
      "tool_pad_tht",  // clang-format break
      tr("Add THT Pad"),
      tr("Add plated through-hole copper pads"),
      QIcon(":/img/actions/add_tht_pad.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_H)},
      &categoryTools,
  };
  EditorCommand toolHole{
      "tool_hole",  // clang-format break
      tr("Add Hole"),
      tr("Add non-plated holes (NPTH drills)"),
      QIcon(":/img/actions/add_hole.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_O)},
      &categoryTools,
  };
  EditorCommand toolWire{
      "tool_wire",  // clang-format break
      tr("Draw Wire"),
      tr("Draw wires to create electrical connections between symbol pins"),
      QIcon(":/img/actions/draw_wire.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_W)},
      &categoryTools,
  };
  EditorCommand toolNetLabel{
      "tool_netlabel",  // clang-format break
      tr("Add Net Label"),
      tr("Add net labels to explicitly specify the net of wires"),
      QIcon(":/img/actions/draw_netlabel.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_N)},
      &categoryTools,
  };
  EditorCommand toolComponent{
      "tool_component",  // clang-format break
      tr("Add Component"),
      tr("Insert components from the workspace libraries"),
      QIcon(":/img/actions/add_component.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_A)},
      &categoryTools,
  };
  EditorCommand toolTrace{
      "tool_trace",  // clang-format break
      tr("Draw Trace"),
      tr("Draw copper traces to interconnect devices"),
      QIcon(":/img/actions/draw_wire.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_W)},
      &categoryTools,
  };
  EditorCommand toolVia{
      "tool_via",  // clang-format break
      tr("Add Via"),
      tr("Add plated through-hole vias"),
      QIcon(":/img/actions/add_via.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_V)},
      &categoryTools,
  };
  EditorCommand toolPlane{
      "tool_plane",  // clang-format break
      tr("Draw Plane"),
      tr("Draw auto-filled copper areas to interconnect pads and vias"),
      QIcon(":/img/actions/add_plane.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_N)},
      &categoryTools,
  };

  EditorCommandCategory categoryCommands{"categoryCommands", tr("Commands"),
                                         &categoryRoot};
  EditorCommand abort{
      "abort",  // clang-format break
      tr("Abort Command"),
      tr("Abort the currently active command"),
      QIcon(":/img/actions/stop.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_Escape)},
      &categoryCommands,
  };
  EditorCommand layerUp{
      "layer_up",  // clang-format break
      tr("Layer Up"),
      tr("Switch to the next higher layer (bottom->top)"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_7)},
      &categoryCommands,
  };
  EditorCommand layerDown{
      "layer_down",  // clang-format break
      tr("Layer Down"),
      tr("Switch to the next lower layer (top->bottom)"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_1)},
      &categoryCommands,
  };
  EditorCommand lineWidthIncrease{
      "line_width_increase",  // clang-format break
      tr("Increase Line Width"),
      tr("Increase the line/trace width"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_Plus)},
      &categoryCommands,
  };
  EditorCommand lineWidthDecrease{
      "line_width_decrease",  // clang-format break
      tr("Decrease Line Width"),
      tr("Decrease the line/trace width"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_Minus)},
      &categoryCommands,
  };
  EditorCommand sizeIncrease{
      "size_increase",  // clang-format break
      tr("Increase Size"),
      tr("Increase the via/circle size"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_8)},
      &categoryCommands,
  };
  EditorCommand sizeDecrease{
      "size_decrease",  // clang-format break
      tr("Decrease Size"),
      tr("Decrease the via/circle size"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_2)},
      &categoryCommands,
  };
  EditorCommand drillIncrease{
      "drill_increase",  // clang-format break
      tr("Increase Drill"),
      tr("Increase the drill diameter"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_9)},
      &categoryCommands,
  };
  EditorCommand drillDecrease{
      "drill_decrease",  // clang-format break
      tr("Decrease Drill"),
      tr("Decrease the drill diameter"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_3)},
      &categoryCommands,
  };
  EditorCommand alignHorizontalLeft{
      "align_horizontal_left",  // clang-format break
      tr("Align Left"),
      tr("Horizontal alignment: Left"),
      QIcon(":/img/command_toolbars/align_horizontal_left.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand alignHorizontalCenter{
      "align_horizontal_center",  // clang-format break
      tr("Align Center"),
      tr("Horizontal alignment: Center"),
      QIcon(":/img/command_toolbars/align_horizontal_center.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand alignHorizontalRight{
      "align_horizontal_right",  // clang-format break
      tr("Align Right"),
      tr("Horizontal alignment: Right"),
      QIcon(":/img/command_toolbars/align_horizontal_right.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand alignVerticalTop{
      "align_vertical_top",  // clang-format break
      tr("Align Top"),
      tr("Vertical alignment: Top"),
      QIcon(":/img/command_toolbars/align_vertical_top.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand alignVerticalCenter{
      "align_vertical_center",  // clang-format break
      tr("Align Center"),
      tr("Vertical alignment: Center"),
      QIcon(":/img/command_toolbars/align_vertical_center.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand alignVerticalBottom{
      "align_vertical_bottom",  // clang-format break
      tr("Align Bottom"),
      tr("Vertical alignment: Bottom"),
      QIcon(":/img/command_toolbars/align_vertical_bottom.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand thtShapeRound{
      "tht_shape_round",  // clang-format break
      tr("Round"),
      tr("Shape: Round"),
      QIcon(":/img/command_toolbars/via_round.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_4)},
      &categoryCommands,
  };
  EditorCommand thtShapeRectangular{
      "tht_shape_rectangular",  // clang-format break
      tr("Rectangular"),
      tr("Shape: Rectangular"),
      QIcon(":/img/command_toolbars/via_square.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_5)},
      &categoryCommands,
  };
  EditorCommand thtShapeOctagonal{
      "tht_shape_octagonal",  // clang-format break
      tr("Octagonal"),
      tr("Shape: Octagonal"),
      QIcon(":/img/command_toolbars/via_octagon.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::Key_6)},
      &categoryCommands,
  };
  EditorCommand wireModeHV{
      "wire_mode_h_v",  // clang-format break
      tr("Horizontal - Vertical"),
      tr("Wire mode: First segment horizontal, second segment vertical"),
      QIcon(":/img/command_toolbars/wire_h_v.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand wireModeVH{
      "wire_mode_v_h",  // clang-format break
      tr("Vertical - Horizontal"),
      tr("Wire mode: First segment vertical, second segment horizontal"),
      QIcon(":/img/command_toolbars/wire_v_h.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand wireMode9045{
      "wire_mode_90_45",  // clang-format break
      tr("90° - 45°"),
      tr("Wire mode: First segment 90°, second segment 45°"),
      QIcon(":/img/command_toolbars/wire_90_45.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand wireMode4590{
      "wire_mode_45_90",  // clang-format break
      tr("45° - 90°"),
      tr("Wire mode: First segment 45°, second segment 90°"),
      QIcon(":/img/command_toolbars/wire_45_90.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };
  EditorCommand wireModeStraight{
      "wire_mode_straight",  // clang-format break
      tr("Straight"),
      tr("Wire mode: Straight line"),
      QIcon(":/img/command_toolbars/wire_straight.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryCommands,
  };

  EditorCommandCategory categoryComponents{"categoryComponents",
                                           tr("Components"), &categoryRoot};
  EditorCommand componentResistor{
      "component_resistor",  // clang-format break
      tr("Resistor"),
      tr("Add standard component: Resistor"),
      QIcon(":/img/library/resistor_eu.png"),
      EditorCommand::Flags(),
      {},
      &categoryComponents,
  };
  EditorCommand componentInductor{
      "component_inductor",  // clang-format break
      tr("Inductor"),
      tr("Add standard component: Inductor"),
      QIcon(":/img/library/inductor_eu.png"),
      EditorCommand::Flags(),
      {},
      &categoryComponents,
  };
  EditorCommand componentCapacitorBipolar{
      "component_capacitor_bipolar",  // clang-format break
      tr("Bipolar Capacitor"),
      tr("Add standard component: Bipolar capacitor"),
      QIcon(":/img/library/bipolar_capacitor_eu.png"),
      EditorCommand::Flags(),
      {},
      &categoryComponents,
  };
  EditorCommand componentCapacitorUnipolar{
      "component_capacitor_unipolar",  // clang-format break
      tr("Unipolar Capacitor"),
      tr("Add standard component: Unipolar capacitor"),
      QIcon(":/img/library/unipolar_capacitor_eu.png"),
      EditorCommand::Flags(),
      {},
      &categoryComponents,
  };
  EditorCommand componentGnd{
      "component_gnd",  // clang-format break
      tr("GND Supply"),
      tr("Add standard component: GND supply"),
      QIcon(":/img/library/gnd.png"),
      EditorCommand::Flags(),
      {},
      &categoryComponents,
  };
  EditorCommand componentVcc{
      "component_vcc",  // clang-format break
      tr("VCC Supply"),
      tr("Add standard component: VCC supply"),
      QIcon(":/img/library/vcc.png"),
      EditorCommand::Flags(),
      {},
      &categoryComponents,
  };

  EditorCommandCategory categoryDocks{"categoryDocks", tr("Docks"),
                                      &categoryRoot};
  EditorCommand dockPages{
      "dock_pages",  // clang-format break
      tr("Pages"),
      tr("Go to the pages dock"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_G)},
      &categoryDocks,
  };
  EditorCommand dockErc{
      "dock_erc",  // clang-format break
      tr("Electrical Rule Check (ERC)"),
      tr("Go to the ERC messages dock"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_E)},
      &categoryDocks,
  };
  EditorCommand dockDrc{
      "dock_drc",  // clang-format break
      tr("Design Rule Check (DRC)"),
      tr("Go to the DRC messages dock"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_D)},
      &categoryDocks,
  };
  EditorCommand dockLayers{
      "dock_layers",  // clang-format break
      tr("Layers"),
      tr("Go to the layers dock"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_L)},
      &categoryDocks,
  };
  EditorCommand dockPlaceDevices{
      "dock_place_devices",  // clang-format break
      tr("Place Devices"),
      tr("Go to the dock for placing devices"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_P)},
      &categoryDocks,
  };

  EditorCommandCategory categoryWindowManagement{
      "categoryWindowManagement", tr("Window Management"), &categoryRoot};
  EditorCommand pageNext{
      "page_next",  // clang-format break
      tr("Next Tab/Page"),
      tr("Navigate to the next tab or page"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_Tab)},
      &categoryWindowManagement,
  };
  EditorCommand pagePrevious{
      "page_previous",  // clang-format break
      tr("Previous Tab/Page"),
      tr("Navigate to the previous tab or page"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Tab)},
      &categoryWindowManagement,
  };
  EditorCommand tabClose{
      "tab_close",  // clang-format break
      tr("Close Tab"),
      tr("Close the currently opened tab"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_W)},
      &categoryWindowManagement,
  };
  EditorCommand tabCloseAll{
      "tab_close_all",  // clang-format break
      tr("Close All Tabs"),
      tr("Close all currently opened tabs"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W)},
      &categoryWindowManagement,
  };
  EditorCommand windowClose{
      "window_close",  // clang-format break
      tr("Close Window"),
      tr("Close this window"),
      QIcon(),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::ALT + Qt::Key_F4)},
      &categoryWindowManagement,
  };
  EditorCommand projectClose{
      "project_close",  // clang-format break
      tr("Close Project"),
      tr("Close the currently opened project"),
      QIcon(":/img/actions/close.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::Key_F4)},
      &categoryWindowManagement,
  };
  EditorCommand projectCloseAll{
      "project_close_all",  // clang-format break
      tr("Close All Projects"),
      tr("Close all currently opened projects"),
      QIcon(":/img/actions/close.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F4)},
      &categoryWindowManagement,
  };
  EditorCommand applicationQuit{
      "application_quit",  // clang-format break
      tr("Quit"),
      tr("Close the whole application"),
      QIcon(":/img/actions/quit.png"),
      EditorCommand::Flag::ShortcutsConfigurable |
          EditorCommand::Flag::QuitRole,
      {QKeySequence(QKeySequence::Quit)},
      &categoryWindowManagement,
  };

  EditorCommandCategory categoryHelp{"categoryHelp", tr("Help"), &categoryRoot};
  EditorCommand aboutLibrePcb{
      "about_librepcb",  // clang-format break
      tr("About LibrePCB"),
      tr("Show information about the application"),
      QIcon(":/img/logo/48x48.png"),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable |
          EditorCommand::Flag::AboutRole,
      {},
      &categoryHelp,
  };
  EditorCommand aboutQt{
      "about_qt",  // clang-format break
      tr("About Qt"),
      tr("Show information about Qt"),
      QIcon(),
      EditorCommand::Flag::OpensPopup |
          EditorCommand::Flag::ShortcutsConfigurable |
          EditorCommand::Flag::AboutQtRole,
      {},
      &categoryHelp,
  };
  EditorCommand website{
      "website",  // clang-format break
      tr("LibrePCB Website"),
      tr("Open the LibrePCB website in the web browser"),
      QIcon(":/img/actions/open_browser.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {},
      &categoryHelp,
  };
  EditorCommand documentationOnline{
      "documentation_online",  // clang-format break
      tr("Online Documentation"),
      tr("Open the documentation in the web browser"),
      QIcon(":/img/actions/help.png"),
      EditorCommand::Flag::ShortcutsConfigurable,
      {QKeySequence(QKeySequence::HelpContents)},
      &categoryHelp,
  };

  EditorCommandCategory categoryContextMenu{"categoryContextMenu",
                                            tr("Context Menu"), &categoryRoot};
  EditorCommand folderNew{
      "folder_new",  // clang-format break
      tr("New Folder"),
      tr("Create a new folder"),
      QIcon(":/img/actions/new_folder.png"),
      EditorCommand::Flags(),
      {},
      &categoryContextMenu,
  };
  EditorCommand vertexAdd{
      "vertex_add",  // clang-format break
      tr("Add Vertex"),
      tr("Insert a new vertex into the selected polygon edge"),
      QIcon(":/img/actions/add.png"),
      EditorCommand::Flags(),
      {},
      &categoryContextMenu,
  };
  EditorCommand vertexRemove{
      "vertex_remove",  // clang-format break
      tr("Remove Vertex"),
      tr("Remove the selected vertex from the polygon"),
      QIcon(":/img/actions/delete.png"),
      EditorCommand::Flags(),
      {},
      &categoryContextMenu,
  };
  EditorCommand traceSelectWhole{
      "trace_select_whole",  // clang-format break
      tr("Select Whole Trace"),
      tr("Select the whole trace"),
      QIcon(":/img/actions/bookmark.png"),
      EditorCommand::Flags(),
      {},
      &categoryContextMenu,
  };
  EditorCommand traceMeasureLength{
      "trace_measure_length",  // clang-format break
      tr("Measure Selected Segments Length"),
      tr("Measure the total length of all selected trace segments"),
      QIcon(":/img/actions/ruler.png"),
      EditorCommand::Flags(),
      {},
      &categoryContextMenu,
  };
  EditorCommand traceRemoveWhole{
      "trace_remove_whole",  // clang-format break
      tr("Remove Whole Trace"),
      tr("Remove the whole trace"),
      QIcon(":/img/actions/minus.png"),
      EditorCommand::Flags(),
      {},
      &categoryContextMenu,
  };
  EditorCommand visible{
      "visible",  // clang-format break
      tr("Visible"),
      tr("Toggle visibility"),
      QIcon(),
      EditorCommand::Flags(),
      {},
      &categoryContextMenu,
  };
};

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb

#endif
