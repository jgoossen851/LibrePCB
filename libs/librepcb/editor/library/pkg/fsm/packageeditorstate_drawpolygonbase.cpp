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
#include "packageeditorstate_drawpolygonbase.h"

#include "../../../cmd/cmdpolygonedit.h"
#include "../../../widgets/angleedit.h"
#include "../../../widgets/graphicslayercombobox.h"
#include "../../../widgets/graphicsview.h"
#include "../../../widgets/unsignedlengthedit.h"
#include "../packageeditorwidget.h"

#include <librepcb/core/geometry/polygon.h>
#include <librepcb/core/graphics/graphicslayer.h>
#include <librepcb/core/graphics/graphicsscene.h>
#include <librepcb/core/graphics/polygongraphicsitem.h>
#include <librepcb/core/library/pkg/footprint.h>
#include <librepcb/core/library/pkg/footprintgraphicsitem.h>

#include <QtCore>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace editor {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

PackageEditorState_DrawPolygonBase::PackageEditorState_DrawPolygonBase(
    Context& context, Mode mode) noexcept
  : PackageEditorState(context),
    mMode(mode),
    mIsUndoCmdActive(false),
    mCurrentPolygon(nullptr),
    mArcCenter(),
    mArcInSecondState(false),
    mCurrentGraphicsItem(nullptr),
    mLastLayerName(GraphicsLayer::sTopPlacement),  // Most important layer
    mLastLineWidth(200000),  // Typical width according library conventions
    mLastAngle(0),
    mLastFill(false),  // Fill is needed very rarely
    mLastGrabArea(false)  // Avoid creating annoying grab areas "by accident"
{
}

PackageEditorState_DrawPolygonBase::
    ~PackageEditorState_DrawPolygonBase() noexcept {
}

/*******************************************************************************
 *  General Methods
 ******************************************************************************/

bool PackageEditorState_DrawPolygonBase::entry() noexcept {
  Q_ASSERT(!mIsUndoCmdActive);

  mContext.graphicsScene.setSelectionArea(QPainterPath());  // clear selection
  mContext.graphicsView.setCursor(Qt::CrossCursor);

  // populate command toolbar
  mContext.commandToolBar.addLabel(tr("Layer:"));
  std::unique_ptr<GraphicsLayerComboBox> layerComboBox(
      new GraphicsLayerComboBox());
  layerComboBox->setLayers(getAllowedCircleAndPolygonLayers());
  layerComboBox->setCurrentLayer(mLastLayerName);
  connect(layerComboBox.get(), &GraphicsLayerComboBox::currentLayerChanged,
          this, &PackageEditorState_DrawPolygonBase::layerComboBoxValueChanged);
  mContext.commandToolBar.addWidget(std::move(layerComboBox));

  mContext.commandToolBar.addLabel(tr("Line Width:"), 10);
  std::unique_ptr<UnsignedLengthEdit> edtLineWidth(new UnsignedLengthEdit());
  edtLineWidth->configure(getDefaultLengthUnit(),
                          LengthEditBase::Steps::generic(),
                          "package_editor/draw_polygon/line_width");
  edtLineWidth->setValue(mLastLineWidth);
  connect(edtLineWidth.get(), &UnsignedLengthEdit::valueChanged, this,
          &PackageEditorState_DrawPolygonBase::lineWidthEditValueChanged);
  mContext.commandToolBar.addWidget(std::move(edtLineWidth));

  if ((mMode == Mode::LINE) || (mMode == Mode::POLYGON)) {
    mContext.commandToolBar.addLabel(tr("Arc Angle:"), 10);
    std::unique_ptr<AngleEdit> edtAngle(new AngleEdit());
    edtAngle->setSingleStep(90.0);  // [°]
    edtAngle->setValue(mLastAngle);
    connect(edtAngle.get(), &AngleEdit::valueChanged, this,
            &PackageEditorState_DrawPolygonBase::angleEditValueChanged);
    mContext.commandToolBar.addWidget(std::move(edtAngle));
  }

  if ((mMode == Mode::RECT) || (mMode == Mode::POLYGON)) {
    std::unique_ptr<QCheckBox> fillCheckBox(new QCheckBox(tr("Fill")));
    fillCheckBox->setChecked(mLastFill);
    connect(fillCheckBox.get(), &QCheckBox::toggled, this,
            &PackageEditorState_DrawPolygonBase::fillCheckBoxCheckedChanged);
    mContext.commandToolBar.addWidget(std::move(fillCheckBox), 10);
  }

  if ((mMode == Mode::RECT) || (mMode == Mode::POLYGON)) {
    std::unique_ptr<QCheckBox> grabAreaCheckBox(new QCheckBox(tr("Grab Area")));
    grabAreaCheckBox->setChecked(mLastGrabArea);
    connect(
        grabAreaCheckBox.get(), &QCheckBox::toggled, this,
        &PackageEditorState_DrawPolygonBase::grabAreaCheckBoxCheckedChanged);
    mContext.commandToolBar.addWidget(std::move(grabAreaCheckBox));
  }

  return true;
}

bool PackageEditorState_DrawPolygonBase::exit() noexcept {
  if (!abort()) {
    return false;
  }

  // cleanup command toolbar
  mContext.commandToolBar.clear();

  mContext.graphicsView.setCursor(Qt::ArrowCursor);
  return true;
}

/*******************************************************************************
 *  Event Handlers
 ******************************************************************************/

bool PackageEditorState_DrawPolygonBase::processGraphicsSceneMouseMoved(
    QGraphicsSceneMouseEvent& e) noexcept {
  if (mIsUndoCmdActive) {
    Point currentPos =
        Point::fromPx(e.scenePos()).mappedToGrid(getGridInterval());
    return updateCurrentPosition(currentPos);
  } else {
    return true;
  }
}

bool PackageEditorState_DrawPolygonBase::
    processGraphicsSceneLeftMouseButtonPressed(
        QGraphicsSceneMouseEvent& e) noexcept {
  Point currentPos =
      Point::fromPx(e.scenePos()).mappedToGrid(getGridInterval());
  if (mIsUndoCmdActive) {
    return addNextSegment(currentPos);
  } else {
    return start(currentPos);
  }
}

bool PackageEditorState_DrawPolygonBase::
    processGraphicsSceneLeftMouseButtonDoubleClicked(
        QGraphicsSceneMouseEvent& e) noexcept {
  return processGraphicsSceneLeftMouseButtonPressed(
      e);  // handle like single click
}

bool PackageEditorState_DrawPolygonBase::processAbortCommand() noexcept {
  if (mIsUndoCmdActive) {
    return abort();
  } else {
    return false;
  }
}

/*******************************************************************************
 *  Private Methods
 ******************************************************************************/

bool PackageEditorState_DrawPolygonBase::start(const Point& pos) noexcept {
  try {
    // Reset members.
    if (mMode == Mode::ARC) {
      mLastAngle = Angle(0);
      mArcCenter = pos;
      mArcInSecondState = false;
    }

    // Create inital path.
    Path path;
    if (mMode == Mode::ARC) {
      for (int i = 0; i < 3; ++i) {
        path.addVertex(pos);
      }
    } else {
      for (int i = 0; i < ((mMode == Mode::RECT) ? 5 : 2); ++i) {
        path.addVertex(pos, (i == 0) ? mLastAngle : Angle::deg0());
      }
    }

    // Add polygon.
    mContext.undoStack.beginCmdGroup(tr("Add footprint polygon"));
    mIsUndoCmdActive = true;
    mCurrentPolygon.reset(new Polygon(Uuid::createRandom(), mLastLayerName,
                                      mLastLineWidth, mLastFill, mLastGrabArea,
                                      path));
    mContext.undoStack.appendToCmdGroup(new CmdPolygonInsert(
        mContext.currentFootprint->getPolygons(), mCurrentPolygon));
    mEditCmd.reset(new CmdPolygonEdit(*mCurrentPolygon));
    mCurrentGraphicsItem =
        mContext.currentGraphicsItem->getPolygonGraphicsItem(*mCurrentPolygon);
    Q_ASSERT(mCurrentGraphicsItem);
    mCurrentGraphicsItem->setSelected(true);
    return true;
  } catch (const Exception& e) {
    QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getMsg());
    abort(false);
    return false;
  }
}

bool PackageEditorState_DrawPolygonBase::abort(bool showErrMsgBox) noexcept {
  try {
    if (mCurrentGraphicsItem) {
      mCurrentGraphicsItem->setSelected(false);
      mCurrentGraphicsItem = nullptr;
    }
    mEditCmd.reset();
    mCurrentPolygon.reset();
    if (mIsUndoCmdActive) {
      mContext.undoStack.abortCmdGroup();
      mIsUndoCmdActive = false;
    }
    return true;
  } catch (const Exception& e) {
    if (showErrMsgBox) {
      QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getMsg());
    }
    return false;
  }
}

bool PackageEditorState_DrawPolygonBase::addNextSegment(
    const Point& pos) noexcept {
  try {
    // Make sure the path is up to date for the current cursor position.
    updateCurrentPosition(pos);

    // If no line was drawn, abort now.
    QVector<Vertex> vertices = mCurrentPolygon->getPath().getVertices();
    bool isEmpty = false;
    if (mMode == Mode::RECT) {
      // Take rect size into account.
      Point size =
          vertices[vertices.count() - 3].getPos() - vertices[0].getPos();
      isEmpty = (size.getX() == 0) || (size.getY() == 0);
    } else {
      // Only take the last line segment into account.
      isEmpty = (vertices[vertices.count() - 1].getPos() ==
                 vertices[vertices.count() - 2].getPos());
    }
    if (isEmpty) {
      return abort();
    }

    // If the first half of an arc was drawn, start the second half now.
    if ((mMode == Mode::ARC) && (!mArcInSecondState)) {
      mArcInSecondState = true;
      updateCurrentPosition(pos);
      return true;
    }

    // Commit current polygon segment.
    mEditCmd->setPath(Path(vertices), true);
    mContext.undoStack.appendToCmdGroup(mEditCmd.take());
    mContext.undoStack.commitCmdGroup();
    mIsUndoCmdActive = false;

    // If the polygon is completed, abort now.
    if ((mMode == Mode::RECT) || (mMode == Mode::ARC)) {
      return abort();
    }

    // Add next polygon segment.
    mContext.undoStack.beginCmdGroup(tr("Add footprint polygon"));
    mIsUndoCmdActive = true;
    mEditCmd.reset(new CmdPolygonEdit(*mCurrentPolygon));
    vertices.last().setAngle(mLastAngle);
    vertices.append(Vertex(pos, Angle::deg0()));
    mEditCmd->setPath(Path(vertices), true);
    return true;
  } catch (const Exception& e) {
    QMessageBox::critical(&mContext.editorWidget, tr("Error"), e.getMsg());
    return false;
  }
}

bool PackageEditorState_DrawPolygonBase::updateCurrentPosition(
    const Point& pos) noexcept {
  if ((!mCurrentPolygon) || (!mEditCmd)) return false;
  QVector<Vertex> vertices = mCurrentPolygon->getPath().getVertices();
  int count = vertices.count();
  if (mMode == Mode::RECT) {
    Q_ASSERT(count >= 5);
    vertices[count - 4].setPos(
        Point(pos.getX(), vertices[count - 5].getPos().getY()));
    vertices[count - 3].setPos(pos);
    vertices[count - 2].setPos(
        Point(vertices[count - 5].getPos().getX(), pos.getY()));
  } else if (mMode == Mode::ARC) {
    if (!mArcInSecondState) {
      // Draw 2 arcs with 180° each to result in an accurate 360° circle.
      // This circle helps the user to place the start point of the arc.
      Q_ASSERT(count == 3);
      vertices[2] = Vertex(pos);
      vertices[1] =
          Vertex(pos.rotated(Angle::deg180(), mArcCenter), Angle::deg180());
      vertices[0] = Vertex(pos, Angle::deg180());
    } else {
      // Now place the end point of the arc. The only degree of freedom is the
      // angle. This angle is determined by the current cursor position and
      // the position where the cursor was before to determine the arc's
      // direction.
      Point arcStart = vertices[0].getPos();
      Angle angle =
          Toolbox::arcAngle(arcStart, pos, mArcCenter).mappedTo180deg();
      if (((mLastAngle > Angle::deg90()) && (angle < 0)) ||
          ((mLastAngle < -Angle::deg90()) && (angle > 0))) {
        angle.invert();
      }
      // Remove the old arc segments.
      while (vertices.count() > 1) {
        vertices.removeLast();
      }
      if (angle.abs() > Angle::deg270()) {
        // The angle is > 270°, so let's create two separate arc segments to
        // avoid mathematical inaccuracy due to too high angle.
        Angle halfAngle = angle / 2;
        vertices[0].setAngle(angle - halfAngle);
        vertices.append(
            Vertex(arcStart.rotated(halfAngle, mArcCenter), halfAngle));
        vertices.append(Vertex(arcStart.rotated(angle, mArcCenter)));
      } else {
        // The angle is small enough to be implemented by a single arc segment.
        vertices[0].setAngle(angle);
        vertices.append(Vertex(arcStart.rotated(angle, mArcCenter)));
      }
      mLastAngle = angle;
    }
  } else {
    Q_ASSERT(count >= 2);
    vertices[count - 1].setPos(pos);
  }
  mEditCmd->setPath(Path(vertices), true);
  return true;
}

void PackageEditorState_DrawPolygonBase::layerComboBoxValueChanged(
    const GraphicsLayerName& layerName) noexcept {
  mLastLayerName = layerName;
  if (mEditCmd) {
    mEditCmd->setLayerName(mLastLayerName, true);
  }
}

void PackageEditorState_DrawPolygonBase::lineWidthEditValueChanged(
    const UnsignedLength& value) noexcept {
  mLastLineWidth = value;
  if (mEditCmd) {
    mEditCmd->setLineWidth(mLastLineWidth, true);
  }
}

void PackageEditorState_DrawPolygonBase::angleEditValueChanged(
    const Angle& value) noexcept {
  mLastAngle = value;
  if (mCurrentPolygon && mEditCmd) {
    Path path = mCurrentPolygon->getPath();
    if (path.getVertices().count() > 1) {
      path.getVertices()[path.getVertices().count() - 2].setAngle(mLastAngle);
      mEditCmd->setPath(path, true);
    }
  }
}

void PackageEditorState_DrawPolygonBase::fillCheckBoxCheckedChanged(
    bool checked) noexcept {
  mLastFill = checked;
  if (mEditCmd) {
    mEditCmd->setIsFilled(mLastFill, true);
  }
}

void PackageEditorState_DrawPolygonBase::grabAreaCheckBoxCheckedChanged(
    bool checked) noexcept {
  mLastGrabArea = checked;
  if (mEditCmd) {
    mEditCmd->setIsGrabArea(mLastGrabArea, true);
  }
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb
